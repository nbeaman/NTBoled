#include <NTBoled.h>

NTBoled oled;

unsigned long lastUpdate = 0;
int batteryLevel = 0;
bool increasing = true;
const int UPDATE_INTERVAL = 25; // ms between updates

void setup() {
  Serial.begin(115200);
  if (!oled.begin()) {
    Serial.println(F("Failed to start OLED"));
  }
  oled.clear();
  oled.show();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Only update when interval has passed
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    // Update battery level
    if (increasing) {
      batteryLevel++;
      if (batteryLevel >= 100) {
        batteryLevel = 100;
        increasing = false;
        // Pause at full charge
        lastUpdate += 1000;
      }
    } else {
      batteryLevel--;
      if (batteryLevel <= 0) {
        batteryLevel = 0;
        increasing = true;
        // Pause at empty
        lastUpdate += 1000;
        Serial.print(F("ok "));
      }
    }
    
    // Update display
    oled.clear();
    oled.drawChargeBar(batteryLevel);
    oled.drawWifiSymbol(true);
    
    char buffer[10];
    sprintf(buffer, "%d%%", batteryLevel);
    oled.print(buffer, 2, 35, 20);
    
    oled.show();
  }
  
  // Your other code can run here without being blocked
}
