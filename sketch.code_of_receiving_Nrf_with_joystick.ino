#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AFMotor.h>

RF24 radio(9, 10); // NRF24L01 module pins , change CE, CSN pin according to your arduino board 

AF_DCMotor motorA(1); // Motor A object
AF_DCMotor motorB(2); // Motor B object

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Set the receiving address
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    int motorSpeeds[2] = {0, 0};
    radio.read(&motorSpeeds, sizeof(motorSpeeds));

    int motorASpeed = motorSpeeds[0];
    int motorBSpeed = motorSpeeds[1];

    // Check if the received signal indicates stop
    if (motorASpeed == 0 && motorBSpeed == 0) {
      motorA.setSpeed(0);
      motorB.setSpeed(0);
    } else {
      // Control motor speed and direction based on received values
      if (motorASpeed >= 0) {
        motorA.setSpeed(motorASpeed);
        motorA.run(FORWARD);
      } else {
        motorA.setSpeed(-motorASpeed);
        motorA.run(BACKWARD);
      }

      if (motorBSpeed >= 0) {
        motorB.setSpeed(motorBSpeed);
        motorB.run(FORWARD);
      } else {
        motorB.setSpeed(-motorBSpeed);
        motorB.run(BACKWARD);
      }
    }
  }
}
