#include <CustomStepper.h>

int numOfSteps = 64;

int IRReaderPin = A0;
int ledPin = 6;

int IREmitterPin = 7;
int IREmitterPin_1 = 7;

int distance;
int maxDist;
int minDist;

int lidOpen = STOP;
int fixedDistance = 75;

int timeToWait = 30000;
int waitStarted = 0;



CustomStepper myStepper(8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  myStepper.setRPM(300);
  myStepper.setSPR(numOfSteps);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
  myStepper.rotateDegrees(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (lidOpen == CCW && myStepper.isDone()) {
    lidOpen = STOP;
  }
  distance = readIR(5);
  checkDistance();
  myStepper.run();
  
  if(lidOpen == CW && myStepper.isDone()){
   delay(timeToWait); 
   waitStarted = 0;
  }
}
void checkDistance() {

  if (minDist > distance) {
    minDist = distance;
    Serial.println(distance);
  }

  if (maxDist < distance) {
    maxDist = distance;
    Serial.println(distance);
  }


  if (distance > fixedDistance) {
    analogWrite(ledPin, 255);
    if (myStepper.isDone() && lidOpen == STOP) {
      waitStarted = 1;
      setLidDirrction();
      myStepper.setDirection(lidOpen);
      myStepper.rotate(16);
    }
  } else {
    analogWrite(ledPin, 0);

    if (myStepper.isDone() && lidOpen == CW && waitStarted == 0) {
      setLidDirrction();
      myStepper.setDirection(lidOpen);
      myStepper.rotate(16);
    }
  }
}

void setLidDirrction() {
  if (lidOpen == CW) {
    lidOpen = CCW;
  } else {
    lidOpen =   CW;
  }
}

int readIR(int times) {
  int ambientIRReading;
  int obstacleIRReading;

  for (int i = 0; i < times; i++) {
    digitalWrite(IREmitterPin, HIGH);
    delay(1);
    obstacleIRReading = analogRead(IRReaderPin);

    distance += obstacleIRReading;
  }

  return distance / times;
}
