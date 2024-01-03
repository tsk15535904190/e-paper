捡垃圾：老王家购买的4.2寸墨水屏幕
硬件方案为TLSR8359，不在这个telink上做开发，仅仅用来做硬件区分
实现的硬件方案为：ESP32+gooddisplay万能驱动板+墨水屏幕

接线为
```
#define EPD_SCK_PIN  19
#define EPD_MOSI_PIN 18
#define EPD_CS_PIN   14
#define EPD_RST_PIN  26
#define EPD_DC_PIN   27
#define EPD_BUSY_PIN 25
```