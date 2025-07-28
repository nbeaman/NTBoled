#include "NTBoled.h"
#include <Wire.h>

// Define common screen properties
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (-1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address for 128x32

/*
 * Constructor
 */
NTBoled::NTBoled() : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

bool NTBoled::isI2CDevicePresent(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

bool NTBoled::isI2CFunctioning(){
  if (!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Display init failed"));
    return false;
  }
  Serial.println(F("Display initialized"));
  return true;
}
  
/*
 * begin()
 */
bool NTBoled::begin() {
  Wire.begin(); // Default SDA = GPIO21, SCL = GPIO22
  
  if (!isI2CDevicePresent(SCREEN_ADDRESS)) {
    Serial.println(F("OLED not found on I2C bus"));
    _initialized = false;
    return false;
  }
  
  if (!isI2CFunctioning()) {
    Serial.println(F("OLED found but failed to initialize"));
    _initialized = false;
    return false;
  }
  
  Serial.println(F("OLED ready"));
  _initialized = true;
  return true;
}

/*
 * clear()
 */
bool NTBoled::clear() {
  if (!_initialized) return false;
  _display.clearDisplay();
  return true;
}

/*
 * print() - Changed to accept const char* instead of String
 */
bool NTBoled::print(const char* message, int size, int x, int y) {
  if (!_initialized) return false;
  _display.setTextColor(WHITE);
  _display.setTextSize(size);
  _display.setCursor(x, y);
  _display.print(message);
  return true;
}

// Overload for printing integers directly
bool NTBoled::print(int value, int size, int x, int y) {
  if (!_initialized) return false;
  char buffer[12]; // Large enough for most integer values
  snprintf(buffer, sizeof(buffer), "%d", value);
  return print(buffer, size, x, y);
}

bool NTBoled::drawWifiSymbol(bool connected) {
  if (!_initialized) return false;
  _display.fillRect(0, 0, 16, 16, SSD1306_BLACK);

  // Draw three lines as signal arcs
  _display.drawLine(2, 12, 4, 12, SSD1306_WHITE);
  _display.drawLine(1, 10, 5, 10, SSD1306_WHITE);
  _display.drawLine(0, 8, 6, 8, SSD1306_WHITE);

  // Connected dot
  if (connected) {
    _display.fillRect(3, 14, 2, 2, SSD1306_WHITE);
  }
  return true;
}

bool NTBoled::drawNoWifiSymbol() {
  if (!_initialized) return false;

  // Clear icon space
  _display.fillRect(0, 0, 16, 16, SSD1306_WHITE);

  // Draw thick "X" for disconnection symbol
  for (int i = 0; i < 3; i++) {
    _display.drawLine(3, 3, 12, 12, SSD1306_BLACK);
    _display.drawLine(3, 12, 12, 3, SSD1306_BLACK);
  }
  return true;
}

bool NTBoled::drawChargeBar(int percentage) {
  if (!_initialized) return false;
  
  // Define the geometry and position of the battery bar
  const int BAR_WIDTH = 28;
  const int BAR_HEIGHT = 9;
  const int BAR_X = SCREEN_WIDTH - BAR_WIDTH - 2;
  const int BAR_Y = 2;

  // Keep the percentage within the valid 0-100 range
  percentage = constrain(percentage, 0, 100);

  // Erase the area behind the bar
  _display.fillRect(BAR_X, BAR_Y, BAR_WIDTH + 3, BAR_HEIGHT, SSD1306_BLACK);

  // Draw the outer frame of the battery
  _display.drawRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, SSD1306_WHITE);

  // Draw the small positive terminal 
  _display.fillRect(BAR_X + BAR_WIDTH, BAR_Y + 3, 2, BAR_HEIGHT - 6, SSD1306_WHITE);

  // Calculate how much of the bar to fill
  int fillWidth = map(percentage, 0, 100, 0, BAR_WIDTH - 2);

  // Draw the filled portion
  if (fillWidth > 0) {
    _display.fillRect(BAR_X + 1, BAR_Y + 1, fillWidth, BAR_HEIGHT - 2, SSD1306_WHITE);
  }
  return true;
}

bool NTBoled::show() {
  if (!_initialized) return false;
  _display.display();
  return true;
}
