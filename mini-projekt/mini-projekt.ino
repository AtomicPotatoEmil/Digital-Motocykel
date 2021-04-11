#include <Wire.h>

const int potentiometerPin = 13;
const int potentiometerLedPin = 12;

const int ignitionButtonPin = 11;
const int ignitionLedPin = 10;
int currentState = HIGH;
int readState;
int previousState = LOW;

const int accelerationButtonPin = 9;

const int MPU_ADDR = 0x68;
int16_t accelerometerX, accelerometerY, accelerometerZ; //not gonna use y and z but we need to read them nonetheless 
int16_t gyroX, gyroY, gyroZ; //same as above
int16_t temperature; //not important, but the arduino needs to read every wire seperately, which includes temperature
char temporaryString[7];

String ON_OFF_STATE = "OFF";
String IGNITION_ON_OFF_STATE = "OFF";
String ACCELERATION_STATE = "NOT MOVING";
String DIRECTION_STATE = "NO DIRECTION";

int gyroscopeValueContainer;

void setup() {
  // put your setup code here, to run once:
  pinMode(potentiometerPin, OUTPUT);
  pinMode(potentiometerLedPin, OUTPUT);

  pinMode(ignitionButtonPin, OUTPUT);
  pinMode(ignitionLedPin, OUTPUT);

  pinMode(accelerationButtonPin, OUTPUT);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  debugPotentiometer(false);
  debugPotentiometerLed(false);
  debugIgnitionButton(false);
  debugIgnitionLed(false);
  debugAccelerationButton(false);

  
  
  if(digitalRead(potentiometerPin) == HIGH){
    digitalWrite(potentiometerLedPin, HIGH);
    ON_OFF_STATE = "ON";
  }else{
    digitalWrite(potentiometerLedPin, LOW);
    ON_OFF_STATE = "OFF";
  }

  int stateButton = digitalRead(ignitionButtonPin);
  if(digitalRead(potentiometerLedPin) == HIGH){
    if(stateButton == HIGH){
      digitalWrite(ignitionLedPin, HIGH);
      IGNITION_ON_OFF_STATE = "ON";
    }
  }else{
      digitalWrite(ignitionLedPin, LOW);
      IGNITION_ON_OFF_STATE = "OFF";
    }

  if(digitalRead(ignitionLedPin) == HIGH){
    if(digitalRead(accelerationButtonPin) == HIGH){
      ACCELERATION_STATE = "MOVING";
    }else{
      ACCELERATION_STATE = "NOT MOVING";
    }
  }

  if(ACCELERATION_STATE != "NOT MOVING"){
    if(accelerometerX + 550 > 3000){
      DIRECTION_STATE = "LEFT";
      Serial.println(DIRECTION_STATE);
    }else if(accelerometerX + 550 < -3000){
      DIRECTION_STATE = "RIGHT";
      Serial.println(DIRECTION_STATE);
    }else{
      DIRECTION_STATE = "STRAIGHT";
    }
  }else{
    DIRECTION_STATE = "NO DIRECTION";
  }

  Serial.println("State on/off: "+ON_OFF_STATE);
  Serial.println("Ignition on/off state: "+IGNITION_ON_OFF_STATE);
  Serial.println("Acceleration state: "+ACCELERATION_STATE);
  Serial.println("Direction state: "+DIRECTION_STATE);
  Serial.println("\n\n\n");
  delay(1000);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7*2, true);

  accelerometerX = Wire.read()<<8 | Wire.read();
  accelerometerY = Wire.read()<<8 | Wire.read();
  accelerometerZ = Wire.read()<<8 | Wire.read();
  temperature = Wire.read()<<8 | Wire.read();
  gyroX = Wire.read()<<8 | Wire.read();
  gyroY = Wire.read()<<8 | Wire.read();
  gyroZ = Wire.read()<<8 | Wire.read();


  
}



void debugPotentiometer(boolean isDebugging){
  if(isDebugging){
    Serial.println(digitalRead(potentiometerPin));
  }
}

void debugPotentiometerLed(boolean isDebugging){
  if(isDebugging){
    Serial.println(digitalRead(potentiometerLedPin));
  }
}

void debugIgnitionButton(boolean isDebugging){
  if(isDebugging){
    Serial.println(digitalRead(ignitionButtonPin));
  }
}

void debugIgnitionLed(boolean isDebugging){
  if(isDebugging){
    Serial.println(digitalRead(ignitionLedPin));
  }
}

void debugAccelerationButton(boolean isDebugging){
  if(isDebugging){
      Serial.println(digitalRead(accelerationButtonPin));
  }
}
