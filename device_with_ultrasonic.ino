#include "RF24.h"
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(9, 10);

char DAT;
int BLUELED = 2;
const int triggerPin = 6;
const int echoPin = 5;

RF24 myRadio (7, 8);
byte addresses [][6] = {"0"};

char receivedData;
int maxDistance;
int distance;
int time;

void setup() {
  Serial .begin(115200);
  BTSerial.begin(9600);
  delay(100);
  myRadio.begin();
  myRadio.setChannel(108);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openWritingPipe(addresses[0]);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  pinMode(BLUELED, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  
  if ( myRadio.available()) {
    Serial.println("|-----------------------|");
    Serial.println("|    Receiving Data     |");
    Serial.println("|-----------------------|");
    while (myRadio.available()) {
      myRadio.read( &receivedData, sizeof(receivedData) );
    }
    
    maxDistance = String(receivedData).toInt() * 10;
    Serial.print("Max Distance: ");
    Serial.print(maxDistance);
    Serial.println(" cm");
  }

  if (maxDistance > 0) {
    // Clears the trigger pin.
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
  
    // Sets the trigger pin on HIGH state for 10 microseconds.
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
  
    // Reads the echo pin and returns the sound wave travel time in microseconds.
    time = pulseIn(echoPin, HIGH);
  
    /* Calculates the distance using d = v*t and divides by 2 because the the sound
    wave travel forward and bounce backward. */
    distance = (time * 0.034) / 2;
    
    if (distance <= maxDistance) {
      Serial.println("Object Detected");
      Serial.print("Distance = ");
      Serial.print(distance);
      Serial.println("cm\n");
      DAT = '1';
      myRadio.stopListening();
      myRadio.write(&DAT, sizeof(DAT));
      delay(1000);
      DAT = '0';
      myRadio.write(&DAT, sizeof(DAT));
      myRadio.startListening();
    }
  }
}
