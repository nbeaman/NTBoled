#include "NTBoled.h"
#include <Wire.h>

// Define common screen properties
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (-1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address for 128x32 (check your specific model)

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
		Serial.println(String("nope"));
    return false;
  }else {
  	Serial.println(String("yes"));
  	return true;
	}	
	
}
	
/*
 * begin()
 */
bool NTBoled::begin() {
	Wire.begin(); // Default SDA = GPIO21, SCL = GPIO22
	if(isI2CDevicePresent(SCREEN_ADDRESS)){
		isI2CFunctioning();
		Serial.println("YES ");
		_initialized = true;
		return true;
	} else {
		Serial.println("NO ");
		_initialized = false;
		return false;
	}
	
}

// setTextSize(1) – Default size (5×7 pixel font rendered in a 6×8 box)
// setTextSize(2) – 2× scale (12×16 box per character)
// setTextSize(3) – 3× scale (18×24 box per character)

void NTBoled::drawMessageBlock(int xt, int yt, int xb, int yb, const String& msg) {
		if (!_initialized) return;
	  // Erase the block area
	  _display.fillRect(xt, yt, xb - xt, yb - yt, SSD1306_BLACK);
	
	  // Set text color and size
	  _display.setTextColor(SSD1306_WHITE);
	  _display.setTextSize(2);
	  
	  // Set cursor to top-left corner of the block
	  _display.setCursor(xt + 2, yt + 2); // Add a little padding
	  
	  // Print the message
	  _display.print(msg);
	  
	  // Refresh the display
	  _display.display();
}


/*
 * clear()
 */
void NTBoled::clear() {
  if (!_initialized) return;
  _display.clearDisplay();
}

/*
 * print()
 */
void NTBoled::print(String message, int size, int x, int y) {
  if (!_initialized) return;
  _display.setTextColor(WHITE);
  _display.setTextSize(size);
  _display.setCursor(x, y);
  _display.print(message);
}

void NTBoled::drawWifiSymbol(bool connected) {
  if (!_initialized) return;
  _display.fillRect(0, 0, 16, 16, SSD1306_BLACK);

  // Draw three lines as signal arcs
  _display.drawLine(2, 12, 4, 12, SSD1306_WHITE);
  _display.drawLine(1, 10, 5, 10, SSD1306_WHITE);
  _display.drawLine(0, 8, 6, 8, SSD1306_WHITE);

  // Connected dot
  if (connected) {
    _display.fillRect(3, 14, 2, 2, SSD1306_WHITE);
  }
}

void NTBoled::drawNoWifiSymbol() {
  if (!_initialized) return;

  // Clear icon space
  _display.fillRect(0, 0, 16, 16, SSD1306_WHITE);

  // Draw thick "X" for disconnection symbol
  for (int i = 0; i < 3; i++) {
  // Diagonal: top-left to bottom-right
  _display.drawLine(3, 3, 12, 12, SSD1306_BLACK);

  // Diagonal: bottom-left to top-right
  _display.drawLine(3, 12, 12, 3, SSD1306_BLACK);
  }
}


/*
 * NEW FUNCTION: drawChargeBar()
 * Draws a battery-style bar in the top-right corner of the display.
 */
void NTBoled::drawChargeBar(int percentage) {
  if (!_initialized) return;
  // Define the geometry and position of the battery bar
  const int BAR_WIDTH = 28;
  const int BAR_HEIGHT = 9;
  // Position it in the top-right corner with a 2px margin
  const int BAR_X = SCREEN_WIDTH - BAR_WIDTH - 2;
  const int BAR_Y = 2;

  // Keep the percentage within the valid 0-100 range
  percentage = constrain(percentage, 0, 100);

  // Erase the area behind the bar first to prevent artifacts when the
  // percentage decreases. We add 3 pixels to the width to erase the terminal too.
  _display.fillRect(BAR_X, BAR_Y, BAR_WIDTH + 3, BAR_HEIGHT, SSD1306_BLACK);

  // Draw the outer frame of the battery
  _display.drawRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, SSD1306_WHITE);

  // Draw the small positive terminal on the right side of the battery
  _display.fillRect(BAR_X + BAR_WIDTH, BAR_Y + 3, 2, BAR_HEIGHT - 6, SSD1306_WHITE);

  // Calculate how much of the bar to fill.
  // The inner fillable width is BAR_WIDTH - 2 pixels (to account for the 1px border).
  int fillWidth = map(percentage, 0, 100, 0, BAR_WIDTH - 2);

  // Draw the filled portion representing the charge
  if (fillWidth > 0) {
    _display.fillRect(BAR_X + 1, BAR_Y + 1, fillWidth, BAR_HEIGHT - 2, SSD1306_WHITE);
  }
}


/*
 * show()
 */
void NTBoled::show() {
  if (!_initialized) return;
  _display.display();
}