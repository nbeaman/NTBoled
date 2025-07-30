#ifndef NTBoled_h
#define NTBoled_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define common screen properties
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class NTBoled {
  public:
    NTBoled();
    bool begin();
    void clear();
    void print(String message, int size, int x, int y);
    void drawWifiSymbol(bool connected);
    void drawNoWifiSymbol();
    void drawChargeBar(int percentage); // Based on sketch usage
    void show(); // Based on sketch usage
    void drawMessageBlock(int xt, int yt, int xb, int yb, const String& msg);
    
  private:
    Adafruit_SSD1306 _display;
    bool _initialized = false;
    bool isI2CDevicePresent(uint8_t address);
    bool isI2CFunctioning();
};

#endif