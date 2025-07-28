#ifndef NT_BOLED_H
#define NT_BOLED_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class NTBoled {
  public:
    NTBoled();
    bool begin();
    bool clear();
    bool print(const char* message, int size, int x, int y);
    bool print(int value, int size, int x, int y); // Overload for integers
    bool drawChargeBar(int percentage);
    bool drawWifiSymbol(bool connected);
    bool drawNoWifiSymbol();
    bool show();

  private:
    Adafruit_SSD1306 _display;
    bool _initialized = false;
    bool isI2CDevicePresent(uint8_t address);
    bool isI2CFunctioning();
};

#endif // NT_BOLED_H
