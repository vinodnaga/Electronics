#include <CustomStepper.h>

CustomStepper myStepper(8, 9, 10, 11);

void setup()
{
  Serial.begin(9600);

  myStepper.setRPM(300);
  myStepper.setSPR(64);
}

void loop()
{
  int obstacle = analogRead(A0);
  Serial.println(obstacle);
  if(obstacle > 0){
    if(myStepper.getDirection() == CCW){
      myStepper.setDirection(CW);
      myStepper.rotate(16 - myStepper.stepsToGo);
    }else{
      myStepper.setDirection(CW);
      myStepper.rotate(16);
    }
  }else{
    if(myStepper.getDirection() == CW){
      myStepper.setDirection(CCW);
      myStepper.rotate(16 - myStepper.stepsToGo);
    }
  }
  
  myStepper.run();
}

