#include <Arduino.h>

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

#define EPD_SCK_PIN 19
#define EPD_MOSI_PIN 18
#define EPD_CS_PIN 14
#define EPD_RST_PIN 26
#define EPD_DC_PIN 27
#define EPD_BUSY_PIN 25

class epd_dev
{
public:
    epd_dev(uint32_t sck = EPD_SCK_PIN, uint32_t data = EPD_MOSI_PIN, uint32_t cs = EPD_CS_PIN, uint32_t rst = EPD_RST_PIN, uint32_t dc = EPD_DC_PIN, uint32_t busy = EPD_BUSY_PIN)
        : sck_pin(sck), data_pin(data), cs_pin(cs), rst_pin(rst), dc_pin(dc), busy_pin(busy) {}
    void gpio_init(void);
    void set_pin(uint32_t pin, uint8_t state);
    uint8_t read_pin(uint32_t pin);
    void spi_write(uint8_t data);
    void delay_ms(uint32_t ms);

protected:
    uint32_t sck_pin, data_pin, cs_pin, rst_pin, dc_pin, busy_pin;
};