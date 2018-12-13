#include <Arduino.h>

#define LED 23
#define MOTOR_1 27
#define MOTOR_2 28
#define STRETCHBAND 4

const int numSamples = 20;
const float amp = 1.2;
int buf[numSamples];
int bufferIndex = 0;

void d_setup() {
    Serial.begin(9600);
    delay(10);

    pinMode(LED, OUTPUT);
    pinMode(MOTOR_1, OUTPUT);
    pinMode(MOTOR_2, OUTPUT);
    pinMode(STRETCHBAND, INPUT);

    digitalWrite(MOTOR_2, LOW);
}

void d_loop() {
    int val = analogRead(STRETCHBAND);
    buf[bufferIndex] = val;
    if(++bufferIndex >= numSamples) bufferIndex = 0;

    float output = 0;
    for(int i=0; i < numSamples; i++) {
    output += buf[i];
    }
    output /= numSamples;

    Serial.println(output);

    if(output > 3800) {
    digitalWrite(LED, HIGH);
    } else {
    digitalWrite(LED, LOW);
    }
}