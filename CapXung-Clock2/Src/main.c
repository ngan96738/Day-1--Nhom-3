#include <stdint.h>

#define RCC_BASE_ADDR     0x40023800UL
#define GPIOA_BASE_ADDR   0x40020000UL

int main(void) {

    uint32_t *pRccCrReg           = (uint32_t*)(RCC_BASE_ADDR + 0x00);
    uint32_t *pRccCfgrReg         = (uint32_t*)(RCC_BASE_ADDR + 0x08);
    uint32_t *pRCCAhb1Enr         = (uint32_t*)(RCC_BASE_ADDR + 0x30);
    uint32_t *pGPIOAModeReg       = (uint32_t*)(GPIOA_BASE_ADDR + 0x00);
    uint32_t *pGPIOAAltFunHighReg = (uint32_t*)(GPIOA_BASE_ADDR + 0x24);

    /* 1. Bật HSE */
    *pRccCrReg |= (1 << 16);

    /* 2. Chờ HSERDY với timeout — tránh treo nếu không có crystal */
    uint32_t timeout = 0x5000;
    while (!(*pRccCrReg & (1 << 17))) {
        if (--timeout == 0) break;
    }

    /* Chỉ chuyển clock nếu HSE thực sự ổn định */
    if (*pRccCrReg & (1 << 17)) {

        /* 3. Chuyển System Clock → HSE: clear SW[1:0], set SW=01 */
        *pRccCfgrReg &= ~(0x3 << 0);
        *pRccCfgrReg |=  (0x1 << 0);

        /* 4. Chờ xác nhận SWS = 01 (hardware confirm clock đã switch) */
        while ((*pRccCfgrReg & (0x3 << 2)) != (0x1 << 2));
    }

    /* 5. MCO1 source = HSE: clear MCO1[22:21], set = 10b */
    *pRccCfgrReg &= ~(0x3 << 21);
    *pRccCfgrReg |=  (0x2 << 21);

    /* 6. MCO1 prescaler = ÷4: clear MCO1PRE[26:24], set = 110b */
    *pRccCfgrReg &= ~(0x7 << 24);
    *pRccCfgrReg |=  (0x6 << 24);

    /* 7. Bật clock GPIOA */
    *pRCCAhb1Enr |= (1 << 0);

    /* 8. PA8 → Alternate Function mode: MODER8 = 10b */
    *pGPIOAModeReg &= ~(0x3 << 16);
    *pGPIOAModeReg |=  (0x2 << 16);

    /* 9. PA8 → AF0 (MCO1): AFRH8[3:0] = 0000b */
    *pGPIOAAltFunHighReg &= ~(0xF << 0);
    /* AF0 = 0000 → không cần set thêm */

    for (;;);
}
