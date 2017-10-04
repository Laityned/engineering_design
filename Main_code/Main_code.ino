#include <Servo.h>

typedef enum{
  REVERSE = 45,
  STILL=90,
  FORWARD=135
} cb_speed;

Servo gymbalX;
Servo gymbalY;

Servo cb_inside;
Servo cb_outside;

Servo tailgate;

//set pins
int motor1f = 7;
int motor1b = 8;
int motor2f = 9;
int motor2b = 10;
int musicPin = 11;
int lightPin1 = 12;
int lightPin2 = 13;
int gymbalPinX = 2;
int gymbalPinY = 3;
int cb_insidePin = 4;
int cb_outsidePin = 5;
int tailgatePin = 6;

//light control setup
bool lights = false;
bool lightstate = true;
long lightTimer = 0;
long highTone = 15000;
long lowTone = 10000;

//setup gymbal position
  int posY = 90;
  int posX = 90;

//setup tailegate
bool open_close = false;

//setup conveyor belt

void motorControl(char command)
{
  switch(command)
  {
    case 'w':
      digitalWrite(motor1f,HIGH);
      digitalWrite(motor1b,LOW);
      digitalWrite(motor2f,HIGH);
      digitalWrite(motor2b,LOW);
    break;
    case 's':
      digitalWrite(motor1f, LOW);
      digitalWrite(motor1b, HIGH);
      digitalWrite(motor2f, LOW);
      digitalWrite(motor2b, HIGH);
    break;
    case 'a':
      digitalWrite(motor1f,HIGH);
      digitalWrite(motor1b,LOW);
      digitalWrite(motor2f,LOW);
      digitalWrite(motor2b,HIGH);
    break;
    case 'd':
      digitalWrite(motor1f,LOW);
      digitalWrite(motor1b,HIGH);
      digitalWrite(motor2f,HIGH);
      digitalWrite(motor2b,LOW);
    break;
    default:
      digitalWrite(motor1f,LOW);
      digitalWrite(motor1b,LOW);
      digitalWrite(motor2f,LOW);
      digitalWrite(motor2b,LOW);
     break;
  }
  
}

void toggleLights(char command)
{
  if(command == 'l')
  {
    lights = !lights;
  }

  if(lights == true)
  {
    if(millis() > lightTimer + 250)
    {
      lightTimer = millis();
      digitalWrite(lightPin1 ,lightstate);
      digitalWrite(lightPin2, !lightstate);
      lightstate?tone(musicPin,highTone):tone(musicPin, lowTone); 
    }
  }
  else
  {
    digitalWrite(lightPin1,LOW); 
    digitalWrite(lightPin2,LOW);
  }
}

void gymbalControl(char command)
{
  switch(command)
  {
    case 'i':
     posY = (posY<175)? posY+5:180;
     gymbalY.write(posY);
    break;
    case 'k':
      posY = (posY>5)? posY-5:0;
      gymbalY.write(posY);
    break;
    case 'j':
      posX = (posX>5)? posX-5:0;
      gymbalX.write(posX);
     break;
     case 'l':
      posX = (posX<175)? posX+5:180;
      gymbalX.write(posX);
     break;
     case 'm':
      posX = 90;
      posY = 90;
      gymbalX.write(posX);
      gymbalY.write(posY);
     break;
  }
  
}

void tailgateControl(char command)
{
    switch(command)
    {
      case 'o':
        tailgate.write(FORWARD);
      break;
      case 'u':
        tailgate.write(REVERSE);
      break;
      default:
        tailgate.write(STILL);
      break;     
    }
}

void conveyorBeltControl(char command)
{
    switch(command)
    {
      case 'q':
        cb_outside.write(REVERSE);
      break;
      case 'r':
        cb_inside.write(FORWARD);
      case 'e':
        cb_outside.write(FORWARD);
      break;
      default:
        cb_outside.write(STILL);
        cb_inside.write(STILL);
      break;
    }

    
}

void setup() {
  //start internetcommunication
  
  Serial1.begin(115200);

  while(Serial1.available())
  {
    Serial1.read();
  }
  
  // dc motor setup
  pinMode(motor1f, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2f, OUTPUT);
  pinMode(motor2b, OUTPUT);

  digitalWrite(motor1f,LOW);
  digitalWrite(motor1b,LOW);
  digitalWrite(motor2f,LOW);
  digitalWrite(motor2b,LOW);

  //lights and music
  pinMode(lightPin1, OUTPUT);
  pinMode(lightPin2, OUTPUT);
  pinMode(musicPin, OUTPUT);
  noTone(musicPin);
  
  //gymbal setup
  gymbalX.attach(gymbalPinX);
  gymbalY.attach(gymbalPinY);

  gymbalX.write(posX);
  gymbalY.write(posY);

  //tailgate setup
  tailgate.attach(tailgatePin);
    
  //convery belt setup
  cb_inside.attach(cb_insidePin);
  cb_outside.attach(cb_outsidePin);

  cb_inside.write(STILL);
  cb_outside.write(STILL);
}

void loop() {
  // put your main code here, to run repeatedly:
  char command = Serial1.read();
  motorControl(command);
  toggleLights(command);
  gymbalControl(command);
  tailgateControl(command);
  conveyorBeltControl(command);
}

