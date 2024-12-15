#include <Arduino.h>
#include <WifiController.h>
// put function declarations here:
int myFunction(int, int);

void setup() {
  WifiController wifi_controller(25*1000);
  Serial.begin(115200);

  wifi_controller.begin();
}

void loop() {
  Serial.println("Looping");
  delay(1000);
}

