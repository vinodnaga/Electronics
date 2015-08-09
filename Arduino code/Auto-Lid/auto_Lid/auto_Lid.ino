#include <CustomStepper.h>

const int numOfStepsInmyStepper = 64;
const int default_rpm_for_myStepper = 500;
const int for_90_degree_steps = 16;

//describes the different states the device can be in
//In idle state, only half the number of emitters will be working. This is done to conserve power.
//In this state the frequency with which the emitters are turned on will be less.
//const int idle_state = 1;

//This state basically means a rise in ir reading which means a target is approaching closer to the device.
//const int approach_state = 2;


//Wait state is the first state of the device. In this state the device emits ir under certain time intervals (in bursts) to determine if it needs to move to the next state
const int wait_state = 1;

//From wait state the device moves to the approach state, where it constantly emits ir to check if a person/animal is on approach
//On getting a high enough reading, the state is changed to the next one
const int approach_state = 2;

//On activation of open state, the lid starts to open & subsequently the ir emitters and sensors are disabled for a minimum period of time.
//this period of time can be a few seconds. After this time period ends, the sensors are turned on and a reading is made to determine if the person / animal is still near the device
//If reading is low, the device starts closing & moves to the closing state
//If reading is high enough, the device stays in open state
const int open_state = 3;

//In closing state the device switches all the sensors on to determine if the person / animal will come back or is still near the device.
// On attaining a high reading, device stops closing and goes back to open state
// Otherwise the device continues closing. On completion of lid closing, state is moved to wait state
const int closing_state = 4;

const int ir_sensor_pin = A0;

//const int ir_emitter_pin = 7;

const int myStepper_pin_1 = 8;
const int myStepper_pin_2 = 9;
const int myStepper_pin_3 = 10;
const int myStepper_pin_4 = 11;

CustomStepper myStepper(myStepper_pin_1, myStepper_pin_2, myStepper_pin_3, myStepper_pin_4);

//for testing purposes & can be used in the final product as an indicator
//const int led_pin = 6;

const int wait_state_time_interval = 1000;
const int open_state_time_interval = 10000;

const int average_no_of_times = 5;

const int approach_distance = 200;
const int minimum_no_of_times_to_try_in_approach = 10;

const int minimum_number_for_target_away = 10;

float previousDistance;
float distance;
int numberOfTimesInApproachState;
int currentState;
int hasClosingStarted;
int isLidOpen;
int isTargetMovingAway;

void reset() {
  currentState = wait_state;
  
  //pinMode(led_pin, OUTPUT);
  
  myStepper.setRPM(default_rpm_for_myStepper);
  myStepper.setSPR(numOfStepsInmyStepper);
  myStepper.rotateDegrees(1);
  currentState = wait_state;
  
  hasClosingStarted = 0;
  isLidOpen = 0;
  isTargetMovingAway = 0;
}

void setup() {
  
  Serial.begin(9600);

  myStepper.setRPM(default_rpm_for_myStepper);
  myStepper.setSPR(numOfStepsInmyStepper);
  myStepper.rotateDegrees(1);
  currentState = wait_state;

  //pinMode(led_pin, OUTPUT);
  
  hasClosingStarted = 0;
  isLidOpen = 0;
  isTargetMovingAway = 0;

}

void loop() {

  myStepper.run();
  
  switch (currentState) {
    case wait_state:  handleWaitState();
      break;
    case approach_state:  handleApproachState();
      break;
    case open_state:  handleOpenState();
      break;
    case closing_state: if(hasClosingStarted == 0){  
                          handleClosingState();
                        }else if(myStepper.isDone()){
                          hasClosingStarted = 0;
                          currentState = wait_state;
                        }else{
                          readIRSensor(average_no_of_times);
                          if(distance > approach_distance){
                            currentState = open_state;
                          }
                        }
      break;
  }
  
  //printState();
  
}

//void printState(){
//   Serial.println(distance);
//   Serial.println(currentState);
//   Serial.println(myStepper.stepsToGo);
//}

void handleWaitState() {
  previousDistance = distance;
  delay(wait_state_time_interval);
  readIRSensor(average_no_of_times);
  
  if(previousDistance < distance){
    previousDistance = distance;
    currentState = approach_state;
  }
}

void handleApproachState() {
  readIRSensor(average_no_of_times);
  if(distance > approach_distance){
    currentState = open_state;
    numberOfTimesInApproachState = 0;
  }else{
    numberOfTimesInApproachState++;
    if(numberOfTimesInApproachState > minimum_no_of_times_to_try_in_approach){
      numberOfTimesInApproachState = 0;
      currentState = wait_state;  
    }
  }
}

void handleOpenState() {
  if(isLidOpen <= 1){
    openLid();
  }
  
  if(isLidOpen == 2){
    readIRSensor(average_no_of_times);
    if(distance < approach_distance){
      isTargetMovingAway++;
      if(isTargetMovingAway > minimum_number_for_target_away){
        Serial.println("----closing state");
        isTargetMovingAway = 0;
        currentState = closing_state;
      }
    }else{
      isTargetMovingAway = 0;
    }
  }
}

void handleClosingState() {
   closeLid();
}

void openLid(){
  if(isLidOpen == 0){
    
    int noOfSteps;
    if(hasClosingStarted > 0){
      hasClosingStarted = 0;
      noOfSteps = for_90_degree_steps - (for_90_degree_steps - myStepper.getStepsGone());
      myStepper.rotate(noOfSteps);
    }else{
      noOfSteps = for_90_degree_steps;
    }
    myStepper.setDirection(CW);
    myStepper.rotate(noOfSteps);
    isLidOpen = 1;
    Serial.println(noOfSteps);
    Serial.println("----in openlid:");
  }else{
    if(myStepper.isDone()){
      delay(open_state_time_interval);
      isLidOpen = 2;
      Serial.println("----opening done");
    }
  }
}

void closeLid(){
    isLidOpen = 0;
    hasClosingStarted = 1;
    myStepper.setDirection(CCW);
    myStepper.rotate(for_90_degree_steps);
}

void outputToLed(){
  //analogWrite(led_pin, distance); 
}

int readIRSensor(int times) {
  distance = 0;
  int obstacleIRReading = 0;
  
  //digitalWrite(ir_emitter_pin, LOW);

  for (int i = 0; i < times; i++) {
    //digitalWrite(ir_emitter_pin, HIGH);
    obstacleIRReading = analogRead(ir_sensor_pin);
    distance += obstacleIRReading;
    //digitalWrite(ir_emitter_pin, LOW);
  }
  //Serial.println(distance / times);
  return distance / times;
}
