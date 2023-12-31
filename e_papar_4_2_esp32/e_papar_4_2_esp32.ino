/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include <stdlib.h>

//创建两张图象的显存，分别对应红黑图像
static uint8_t IMG[2*EPD_4IN2_V2_WIDTH / 8 * EPD_4IN2_V2_HEIGHT] = {0};
static uint16_t IMG_index = 0 ;
void setup()
{
    printf("power on!\r\n");
    DEV_Module_Init();

#if 0 
    printf("Draw black image\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 70, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 80, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 50, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(50, 70, 20, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(60, 70, 90, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(125, 85, 15, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_CN(5, 15, "你好abc", &Font12CN, WHITE, BLACK);

    //2.Draw red image
    printf("Draw red image\r\n");
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 90, RED, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 100, RED, DOT_PIXEL_4X4, DOT_STYLE_DFT);
    Paint_DrawLine(125, 70, 125, 100, RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(110, 85, 140, 85, RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawRectangle(20, 70, 50, 100, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(165, 85, 15, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(5, 0, "waveshare Electronics", &Font12, BLACK, WHITE);
    Paint_DrawNum(5, 50, 987654321, &Font16, WHITE, RED);

    printf("EPD_Display\r\n");

    EPD_4IN2_V2_Display_Double_Color(BlackImage,RYImage);
#endif

}

/* The main loop -------------------------------------------------------------*/
typedef enum uart_data_process {
    IDLE,
    TRANING,
}uart_data_process_e;
static uart_data_process_e img_data_process = IDLE ;
static char temp_buffer[300];
static char comchar;
void loop()
{
    delay(2);
    uint16_t uart_num = Serial.available();
    if (uart_num > 0) {
        switch (uart_num)
        {
        case 1 :
            comchar = Serial.read();
            if(comchar == 'S')
            {
                memset(IMG,0,sizeof(IMG));
                IMG_index = 0 ;
                Serial.print('S');
                img_data_process = TRANING ;
            }
            else if(comchar == 'E')
            {
                if(IMG_index >= sizeof(IMG))
                {
                    IMG_index = 0 ;
                    Serial.print('E');
                    img_data_process = IDLE ;
                    EPD_4IN2_V2_Init();
                    // Paint_Clear(WHITE);
                    EPD_4IN2_V2_Display_Double_Color(&IMG[0],&IMG[15000]);
                    DEV_Delay_ms(2000);
                    EPD_4IN2_V2_Sleep();
                    Serial.println("OK!");
                }
                else
                {
                    Serial.println(IMG_index);
                }
            }
            break;
        default:
            if(TRANING == img_data_process)
            {
                Serial.readBytes(temp_buffer, uart_num);
                if(IMG_index < sizeof(IMG))
                {
                    memcpy(&IMG[IMG_index],temp_buffer,uart_num);
                    IMG_index += uart_num ;
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
