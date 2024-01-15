#include "EPD_4in2_V2.h"
epd myepd;
// 创建两张图象的显存，分别对应红黑图像
static uint8_t IMG[2 * 400/8 * 300] = {0};
static uint16_t IMG_index = 0;

void setup(void)
{
    Serial.begin(115200);
    myepd.EPD_4IN2_V2_Init();
    Serial.println("start");
}

typedef enum uart_data_process
{
    IDLE,
    TRANING,
} uart_data_process_e;
static uart_data_process_e img_data_process = IDLE;
static char temp_buffer[300];
static char comchar;
void loop(void)
{
    delay(2);
    uint16_t uart_num = Serial.available();
    if (uart_num > 0)
    {
        switch (uart_num)
        {
        case 1:
            comchar = Serial.read();
            if (comchar == 'S')
            {
                memset(IMG, 0, sizeof(IMG));
                IMG_index = 0;
                Serial.print('S');
                img_data_process = TRANING;
            }
            else if (comchar == 'E')
            {
                if (IMG_index >= sizeof(IMG))
                {
                    IMG_index = 0;
                    Serial.print('E');
                    img_data_process = IDLE;
                    myepd.EPD_4IN2_V2_Init();
                    // Paint_Clear(WHITE);
                    myepd.EPD_4IN2_V2_Display_Double_Color(&IMG[0], &IMG[15000]);
                    myepd.delay_ms(2000);
                    myepd.EPD_4IN2_V2_Sleep();
                    Serial.println("OK!");
                }
                else
                {
                    Serial.println(IMG_index);
                }
            }
            break;
        default:
            if (TRANING == img_data_process)
            {
                Serial.readBytes(temp_buffer, uart_num);
                if (IMG_index < sizeof(IMG))
                {
                    memcpy(&IMG[IMG_index], temp_buffer, uart_num);
                    IMG_index += uart_num;
                    Serial.print('T');
                    Serial.println(IMG_index);
                }
                else
                {
                    Serial.println("ERROR");
                }
            }
            break;
        }
    }
}
