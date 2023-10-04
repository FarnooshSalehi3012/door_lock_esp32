#include <Arduino.h>

#define BAUD_RATE            9600
#define PIN_NUMBER           16 // GPIO16 = RX2
#define debouncingTimeNeeded 250

//variables to keep track of the timing of recent interrupts
unsigned long currentTime     = 0;  
unsigned long lastTimePressed = 0; 

struct Button {
	const uint8_t PIN;
	uint32_t      pressedCounter;
	bool          isPressed;
};

Button button = {PIN_NUMBER, 0, false};

void IRAM_ATTR ISR() {

  currentTime = millis();

  if (currentTime - lastTimePressed > debouncingTimeNeeded) {
    button.pressedCounter ++;
    button.isPressed = true;
    lastTimePressed = currentTime;
  }
}

void setup() {
	Serial.begin(BAUD_RATE);
	pinMode(button.PIN, INPUT_PULLUP);
	attachInterrupt(button.PIN, ISR, FALLING);
}

void loop() {
	if (button.isPressed) {
		Serial.printf("Button has been pressed %u times\n", button.pressedCounter);
		button.isPressed = false;
	}
}