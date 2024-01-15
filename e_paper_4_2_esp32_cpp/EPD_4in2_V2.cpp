#include "EPD_4in2_V2.h"
#include "Debug.h"
void epd ::EPD_4IN2_V2_Clear(void)
{
    uint16_t Width, Height;
    Width = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_4IN2_V2_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_4IN2_V2_SendData(0xFF);
        }
    }

    EPD_4IN2_V2_SendCommand(0x26);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_4IN2_V2_SendData(0xFF);
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}

void epd ::EPD_4IN2_V2_Init(void)
{
    gpio_init();
    EPD_4IN2_V2_Reset();

    EPD_4IN2_V2_ReadBusy();
    EPD_4IN2_V2_SendCommand(0x12); // soft  reset
    EPD_4IN2_V2_ReadBusy();

    EPD_4IN2_V2_SendCommand(0x01); // Driver output control
    EPD_4IN2_V2_SendData(0x2b);
    EPD_4IN2_V2_SendData(0x01);
    EPD_4IN2_V2_SendData(0x00);

    EPD_4IN2_V2_SendCommand(0x21); //  Display update control
    EPD_4IN2_V2_SendData(0x00);
    EPD_4IN2_V2_SendData(0x80);

    EPD_4IN2_V2_SendCommand(0x3C); // BorderWavefrom
    EPD_4IN2_V2_SendData(0x05);

    EPD_4IN2_V2_SendCommand(0x11); // data  entry  mode
    EPD_4IN2_V2_SendData(0x03);    // X-mode

    EPD_4IN2_V2_SendCommand(0x18); // Read built-in temperature sensor
    EPD_4IN2_V2_SendData(0x80);

    EPD_4IN2_V2_SetWindows(0, 0, EPD_WIDTH - 1, EPD_HEIGHT - 1);

    EPD_4IN2_V2_SetCursor(0, 0);

    EPD_4IN2_V2_ReadBusy();
}

void epd ::EPD_4IN2_V2_Display_Double_Color(uint8_t *Image_black, uint8_t *Image_Red)
{
    uint16_t Width, Height;
    Width = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_4IN2_V2_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_4IN2_V2_SendData(Image_black[i + j * Width]);
        }
    }

    EPD_4IN2_V2_SendCommand(0x26);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_4IN2_V2_SendData(Image_Red[i + j * Width]);
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}
void epd ::EPD_4IN2_V2_Sleep(void)
{
    EPD_4IN2_V2_SendCommand(0x10); // DEEP_SLEEP
    EPD_4IN2_V2_SendData(0x01);
    delay_ms(200);
}
void epd ::EPD_4IN2_V2_TurnOnDisplay(void)
{
    EPD_4IN2_V2_SendCommand(0x20);
    EPD_4IN2_V2_ReadBusy();
}

void epd ::EPD_4IN2_V2_Reset(void)
{
    set_pin(rst_pin, 1);
    delay_ms(100);
    set_pin(rst_pin, 0);
    delay_ms(2);
    set_pin(rst_pin, 1);
    delay_ms(100);
}
void epd ::EPD_4IN2_V2_SendCommand(uint8_t Reg)
{
    set_pin(dc_pin, 0);
    set_pin(cs_pin, 0);
    spi_write(Reg);
    set_pin(cs_pin, 1);
}
void epd ::EPD_4IN2_V2_SendData(uint8_t Data)
{
    set_pin(dc_pin, 1);
    set_pin(cs_pin, 0);
    spi_write(Data);
    set_pin(cs_pin, 1);
}
void epd ::EPD_4IN2_V2_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while (read_pin(busy_pin) == 1)
    { // LOW: idle, HIGH: busy
        delay_ms(10);
    }
    Debug("e-Paper busy release\r\n");
}
void epd ::EPD_4IN2_V2_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    EPD_4IN2_V2_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_4IN2_V2_SendData((Xstart >> 3) & 0xFF);
    EPD_4IN2_V2_SendData((Xend >> 3) & 0xFF);

    EPD_4IN2_V2_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_4IN2_V2_SendData(Ystart & 0xFF);
    EPD_4IN2_V2_SendData((Ystart >> 8) & 0xFF);
    EPD_4IN2_V2_SendData(Yend & 0xFF);
    EPD_4IN2_V2_SendData((Yend >> 8) & 0xFF);
}
void epd ::EPD_4IN2_V2_SetCursor(uint16_t Xstart, uint16_t Ystart)
{
    EPD_4IN2_V2_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_4IN2_V2_SendData(Xstart & 0xFF);

    EPD_4IN2_V2_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_4IN2_V2_SendData(Ystart & 0xFF);
    EPD_4IN2_V2_SendData((Ystart >> 8) & 0xFF);
}
