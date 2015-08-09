//#include <CustomStepper.h>

int numOfSteps = 64;

int IRReaderPin = A0;
int ledPin = 11;

int IREmitterPin = 8;
int IREmitterPin_1 = 7;

int distance;
int maxDist;
int minDist;

//int lidOpen = STOP;

//CustomStepper myStepper(8,9,10,11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //  myStepper.setRPM(60);
  //  myStepper.setSPR(numOfSteps);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = readIR(5);
  checkDistance();
  //myStepper.run();

  //   if(lidOpen == CCW && myStepper.isDone()){
  //         lidOpen = STOP;
  //   }
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


  if (distance > 100) {
    analogWrite(ledPin, 255);
    //      if(myStepper.isDone() && lidOpen == STOP){
    //        setLidDirrction();
    //        myStepper.setDirection(lidOpen);
    //        myStepper.rotateDegrees(90);
    //      }
  } else {
    analogWrite(ledPin, 0);

    //      if(myStepper.isDone() && lidOpen == CW){
    //        setLidDirrction();
    //        myStepper.setDirection(lidOpen);
    //        myStepper.rotateDegrees(90);
    //      }
  }
}

//void setLidDirrction(){
// if(lidOpen == CW){
//  lidOpen = CCW;
// } else{
//   lidOpen = CW;
// }
//}

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
