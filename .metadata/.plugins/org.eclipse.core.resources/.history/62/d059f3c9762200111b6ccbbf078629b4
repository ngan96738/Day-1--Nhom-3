#include <system_stm32f4xx.h>
#include <stdint.h>
#include <timer.h>
#include <Ucglib.h>

static ucg_t ucg;

int main(void)
{
    SystemCoreClockUpdate();    // cập clock
    TimerInit();                // để khởi tạo chức năng timer.

    // Khởi tạo LCD
    Ucglib4WireSWSPI_begin(&ucg, UCG_FONT_MODE_SOLID);
    ucg_ClearScreen(&ucg);

    ucg_SetFont(&ucg, ucg_font_helvR08_tf); // lựa chọn font chữ và cỡ chữ
    ucg_SetColor(&ucg, 0, 255, 255, 255);   // chọn màu chữ là màu trắng
    ucg_SetColor(&ucg, 1, 0, 0, 0);         // màu background là màu đen
    ucg_SetRotate180(&ucg);                 // quay màn hình lại 180 độ
    ucg_DrawString(&ucg, 0, 12, 0, "I love Embedded"); // in dòng chữ ra LCD
    ucg_DrawString(&ucg, 0, 26, 0, "Programming");

    /* Loop forever */
    while(1)
    {
        processTimerScheduler();
    }
}
