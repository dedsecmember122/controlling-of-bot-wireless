#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // NRF24L01 module pins

int joystickXPin = A0; // Joystick X-axis analog pin
int joystickYPin = A1; // Joystick Y-axis analog pin

int motorStopThreshold = 100; // Threshold to consider as stop signal

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Set the transmitting address
}

void loop() {
  int joystickXValue = analogRead(joystickXPin); // Read X-axis value
  int joystickYValue = analogRead(joystickYPin); // Read Y-axis value

  // Check if joystick values are within the stop threshold
  if (abs(joystickXValue - 512) < motorStopThreshold && abs(joystickYValue - 512) < motorStopThreshold) {
    // Send stop signal
    int motorSpeeds[2] = {0, 0};
    radio.write(&motorSpeeds, sizeof(motorSpeeds));
  } else {
    // Map the joystick values to motor speeds
    int motorASpeed = map(joystickYValue, 0, 1023, -255, 255);
    int motorBSpeed = map(joystickXValue, 0, 1023, -255, 255);

    // Send the motor speeds to the receiver
    int motorSpeeds[2] = {motorASpeed, motorBSpeed};
    radio.write(&motorSpeeds, sizeof(motorSpeeds));
  }

  delay(100); // Adjust delay as needed
}
