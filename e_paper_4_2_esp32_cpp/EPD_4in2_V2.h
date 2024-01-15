#include "DEV_Config.h"
class epd : public epd_dev
{
public:
    uint16_t EPD_WIDTH, EPD_HEIGHT;
    epd()
    {
        EPD_WIDTH = 400;
        EPD_HEIGHT = 300;
    };
    void EPD_4IN2_V2_Init(void);
    void EPD_4IN2_V2_Clear(void);
    void EPD_4IN2_V2_Display_Double_Color(uint8_t *Image_black, uint8_t *Image_Red);
    void EPD_4IN2_V2_Sleep(void);

private:
    void EPD_4IN2_V2_TurnOnDisplay(void);
    void EPD_4IN2_V2_Reset(void);
    void EPD_4IN2_V2_SendCommand(uint8_t Reg);
    void EPD_4IN2_V2_SendData(uint8_t Data);
    void EPD_4IN2_V2_ReadBusy(void);
    void EPD_4IN2_V2_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
    void EPD_4IN2_V2_SetCursor(uint16_t Xstart, uint16_t Ystart);
};