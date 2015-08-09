
int redPin = 9;
int greenPin = 10;
int bluePin = 11;

void setup() {

//  pinMode(redPin, OUTPUT);
//  pinMode(greenPin, OUTPUT);
//  pinMode(bluePin, OUTPUT);

}

void loop() {

  setColor(255,153,0);
  //setColor(19,136,8);
  //setColor(0,0,128);

  //setColor(255 - 255,255 - 153,255 - 51);
  //setColor(255 - 19,255 - 136,255 - 8);
  //setColor(0,0,255 - 128);


}

void setColor(float r,float g, float b){

  analogWrite(redPin, 255 - r);
  analogWrite(greenPin, 255 - g);
  analogWrite(bluePin, 255 - b);

}

