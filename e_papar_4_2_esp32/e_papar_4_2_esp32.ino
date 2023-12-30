/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include <stdlib.h>

/* Entry point ----------------------------------------------------------------*/
static uint8_t IMG[EPD_4IN2_V2_WIDTH / 8 * EPD_4IN2_V2_HEIGHT] = {0};
static uint16_t IMG_index = 0 ;
void setup()
{
    printf("EPD_4IN2_V2_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    // EPD_4IN2_V2_Init();
    // EPD_4IN2_V2_Clear();
    // DEV_Delay_ms(500);

    // //Create a new image cache
    // UBYTE *BlackImage = IMG;
    // printf("Paint_NewImage\r\n");
    // Paint_NewImage(BlackImage, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, 0, WHITE);


#if 0  // show bmp
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_4in2);
    EPD_4IN2_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
    
#endif        

#if 0  // show image for array   
//    EPD_4IN2_V2_Init_Fast(Seconds_1_5S);
    EPD_4IN2_V2_Init_Fast(Seconds_1S);
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_4in2);
    EPD_4IN2_V2_Display_Fast(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 0   // Drawing on the image

    EPD_4IN2_V2_Init();
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(130, 0, " 你好abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

    printf("EPD_Display\r\n");
    // EPD_4IN2_V2_Display(BlackImage);
	EPD_4IN2_V2_Display(BlackImage);
	DEV_Delay_ms(2000);
#endif

#if 0
	printf("Partial refresh\r\n");
    Paint_NewImage(BlackImage, 120, 50, 0, WHITE);
	PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 10;
	for (;;) {
		sPaint_time.Sec = sPaint_time.Sec + 1;
		if (sPaint_time.Sec == 60) {
			sPaint_time.Min = sPaint_time.Min + 1;
			sPaint_time.Sec = 0;
			if (sPaint_time.Min == 60) {
				sPaint_time.Hour =  sPaint_time.Hour + 1;
				sPaint_time.Min = 0;
				if (sPaint_time.Hour == 24) {
					sPaint_time.Hour = 0;
					sPaint_time.Min = 0;
					sPaint_time.Sec = 0;
				}
			}
		}
		Paint_Clear(WHITE);
		Paint_DrawRectangle(1, 1, 120, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawTime(10, 15, &sPaint_time, &Font20, WHITE, BLACK);
		EPD_4IN2_V2_PartialDisplay(BlackImage, 200, 80, 320, 130);
		DEV_Delay_ms(500);//Analog clock 1s
		num = num - 1;
		if(num == 0) {
			break;
		}
    }
#endif


#if 0
    // EPD_4IN2_V2_Init();
    // EPD_4IN2_V2_Clear();
    EPD_4IN2_V2_Init_4Gray();
    printf("show Gray------------------------\r\n");
    free(BlackImage);
    BlackImage = NULL;
    Imagesize = ((EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 4 ): (EPD_4IN2_V2_WIDTH / 4 + 1)) * EPD_4IN2_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while (1);
    }
    Paint_NewImage(BlackImage, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, 0, WHITE);
    Paint_SetScale(4);
    Paint_Clear(WHITE);

    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(140, 0, "你好abc", &Font12CN, GRAY1, GRAY4);
    Paint_DrawString_CN(140, 40, "你好abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(140, 80, "你好abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(140, 120, "你好abc", &Font12CN, GRAY4, GRAY1);

    Paint_DrawString_CN(220, 0, "微雪电子", &Font24CN, GRAY1, GRAY4);
    Paint_DrawString_CN(220, 40, "微雪电子", &Font24CN, GRAY2, GRAY3);
    Paint_DrawString_CN(220, 80, "微雪电子", &Font24CN, GRAY3, GRAY2);
    Paint_DrawString_CN(220, 120, "微雪电子", &Font24CN, GRAY4, GRAY1);

    EPD_4IN2_V2_Display_4Gray(BlackImage);
    DEV_Delay_ms(2000);

    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_4in2_4Gray);
    EPD_4IN2_V2_Display_4Gray(BlackImage);
    DEV_Delay_ms(2000);

#endif

#if 0
    EPD_4IN2_V2_Init();
    EPD_4IN2_V2_Clear();
#endif

    // printf("Goto Sleep...\r\n");
    // EPD_4IN2_V2_Sleep();
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

                    EPD_4IN2_V2_Init_Fast(Seconds_1S);
                    printf("show image for array\r\n");
                    Paint_SelectImage(IMG);
                    Paint_Clear(WHITE);
                    EPD_4IN2_V2_Display_Fast(IMG);
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
