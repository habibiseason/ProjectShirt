#include "HeaderFiles/Sensors/CapacitiveSensor.h"
#include "HeaderFiles/CommonInterface.h"


CapacitiveSensor::CapacitiveSensor(string n, int pin1, int pin2) : Sensor(n), analogPin1(pin1), analogPin2(pin2)
{
	initSensor();
}

void CapacitiveSensor::initSensor() {
	pinMode(analogPin1, OUTPUT);
	pinMode(analogPin2, OUTPUT);
	cout << "Initialsing capacitive sensor (pin " << analogPin1 << ", " << analogPin2 << ") --> " << this->getName() << endl;
}

int CapacitiveSensor::getValue() {
	//insert capacitive measuring code
	int OUT_PIN = analogPin1;
	int IN_PIN = analogPin2;
	float IN_STRAY_CAP_TO_GND = 24.48;
	float IN_CAP_TO_GND = IN_STRAY_CAP_TO_GND;
	float R_PULLUP = 34.8; 
	float capacitance;
 	int MAX_ADC_VALUE = 4095;  //2^12
	//cout << "Capacitive data incomming...\n";

	pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);


	if (val < 1000)
    {
      pinMode(IN_PIN, OUTPUT);

      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

      //Serial.print(F("Capacitance Value = "));
      //Serial.println(capacitance, 3);
    }
    else
    {
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;

      do
      {
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } while ((digVal < 1) && (t < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);

      capacitance = -(float)t / R_PULLUP
                              / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

      capacitance *= CAPACITANCE_MULTIPLIER;
      //Serial.print(F("Capacitance Value = "));
      if (capacitance > 1000.0)
      {
        //Serial.print(capacitance / 1000.0, 2);
        //Serial.print(F(" uF"));
      }
      else
      {
        //Serial.print(capacitance, 2);
        //Serial.println(F(" nF"));
      }
    }

    if((millis() - counter) > getDelay()) {
      counter = millis();
      struct capacitive_struct strct = {
        .sensorId = (getName() == "CSB1") ? 1:2,
        .timestamp = millis(),
        .value = capacitance,
      };

      CommonInterface::capacitiveQueue.push(strct);
    }

	return capacitance;
}

