int switchPin = 8;
int ledPin = 11;
boolean stateButton = LOW;
boolean currentButton = LOW;
int ledOn = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

boolean debounce(boolean last){
  boolean current = digitalRead(switchPin);
  
  if(last != current){
   delay(5);
   current = digitalRead(switchPin); 
  }
  return current;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  currentButton = debounce(stateButton);
  
  if(stateButton == LOW && currentButton == HIGH){
    ledOn += 51;
  }
  stateButton = currentButton;
  
  if(ledOn > 255){
    ledOn = 0;
  }
  
  analogWrite(ledPin, ledOn);
}
