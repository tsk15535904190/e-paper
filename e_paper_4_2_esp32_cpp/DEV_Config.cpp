#include "DEV_Config.h"

void epd_dev::delay_ms(uint32_t ms)
{
    delay(ms);
}
void epd_dev::set_pin(uint32_t pin, uint8_t state)
{
    digitalWrite(pin, state == 0 ? LOW : HIGH);
}
uint8_t epd_dev::read_pin(uint32_t pin)
{
    return digitalRead(pin);
}
void epd_dev::gpio_init(void)
{
    pinMode(busy_pin, INPUT);
    pinMode(rst_pin, OUTPUT);
    pinMode(dc_pin, OUTPUT);

    pinMode(sck_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);
    pinMode(cs_pin, OUTPUT);

    digitalWrite(cs_pin, HIGH);
    digitalWrite(sck_pin, LOW);
}
void epd_dev::spi_write(uint8_t data)
{
    digitalWrite(cs_pin, GPIO_PIN_RESET);

    for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0)
            digitalWrite(data_pin, GPIO_PIN_RESET);
        else
            digitalWrite(data_pin, GPIO_PIN_SET);

        data <<= 1;
        digitalWrite(sck_pin, GPIO_PIN_SET);
        digitalWrite(sck_pin, GPIO_PIN_RESET);
    }
    digitalWrite(cs_pin, GPIO_PIN_SET);
}