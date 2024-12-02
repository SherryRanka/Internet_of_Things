#include <Arduino.h>

#define LED_PIN 4      // LED connected to pin 4
#define BUTTON_PIN 2   // Button connected to pin 2

bool ledState = false; // Variable to track LED state

void setup() {
  pinMode(LED_PIN, OUTPUT);      // Set LED pin as output
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) { // LOW indicates button pressed
    ledState = !ledState; // Toggle LED state
    digitalWrite(LED_PIN, ledState ? HIGH : LOW); // Set LED state
    delay(200); // Debounce delay to avoid multiple toggles
  }
}


