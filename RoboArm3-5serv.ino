/* meArm analog joysticks version 1.3.1 - UtilStudio.com Dec 2018
   Uses two analogue joysticks and four servos.

   In version 1.3 was improved recording of coordinates.
   Some bugs was removed.

   First joystick moves gripper forwards, backwards, left and right,
   button start/stop recording positions.

   Second joystick moves gripper up, down, and closes and opens,
   button start/stop playing recorded positions.
   Press button for 2 seconds to autoplay.

   Pins:
   Arduino    Stick1    Stick2    Base   Shoulder  Elbow    Gripper   Record/
      GND       GND       GND    Brown     Brown   Brown     Brown    Auto play
       5V       VCC       VCC      Red       Red     Red       Red    LED
       A0       HOR
       A1       VER
       PD2      BUTT
       A2                 HOR
       A3                 VER
       PD3                BUTT
       11                       Yellow
       10                                 Yellow
        9                                         Yellow
        6                                                   Yellow
       PD4                                                            X
*/
#include <Servo.h>

bool repeatePlaying = false; /* Repeatedly is running recorded cycle */
int delayBetweenCycles = 2000; /* Delay between cycles */

int basePin = 11;       /* Base servo */
int shoulderPin = 10;   /* Shoulder servo */
int elbowPin = 9;       /* Elbow servo */
int gripperPin = 6;     /* Gripper servo */
int wrist1Pin = 7;
int wrist2Pin = 5;
int presscnt = 1;

int xdirPin = 0;        /* Base - joystick1*/
int ydirPin = 1;        /* Shoulder - joystick1 */
int zdirPin = 3;        /* Elbow - joystick2 */
int gdirPin = 2;        /* Gripper - joystick2 */
int Mwrist1Pin = 4;
int Mwrist2Pin = 5;

//int pinRecord = A4;     /* Button record - backward compatibility */
//int pinPlay = A5;       /* Button play  - backward compatibility */
int pinRecord = PD2;     /* Button record - recommended (A4 is deprecated, will by used for additional joystick) */
int pinPlay = PD3;       /* Button play  - recommended (A5 is deprecated, will by used for additional joystick) */
int pinLedRecord = PD4;  /* LED - indicates recording (light) or auto play mode (blink one) */

bool useInternalPullUpResistors = false;

const int buffSize = 512; /* Size of recording buffer */

int startBase = 90;
int startShoulder = 90;
int startElbow = 90;
int startGripper = 0 ;
int startWrist = 0;
int startWrist2 = 90;

int posBase = 90;
int posShoulder = 90;
int posElbow = 90;
int posGripper = 0;
int posWrist = 0;
int posWrist2 = 90;

int lastBase = 90;
int lastShoulder = 90;
int lastElbow = 90;
int lastGripper = 90;
int lastWrist1 = 0;
int lastWrist2 = 0;

int minBase = 0;
int maxBase = 180;
int minShoulder = 0;
int maxShoulder = 79;
int minElbow = 0;
int maxElbow = 105;
int minGripper = 10;
int maxGripper = 130 ;
//Write
int minlastWrist1 = 0;
int minlastWrist2 = 90;
//Write
int minWrist1 = 0;
int minWrist2 = 90;

const int countServo = 4;
int buff[buffSize];
int buffAdd[countServo];
int recPos = 0;
int playPos = 0;

int buttonRecord = HIGH;
int buttonPlay = HIGH;

int buttonRecordLast = LOW;
int buttonPlayLast = LOW;

bool record = false;
bool play = false;
bool debug = false;

String command = "Manual";
int printPos = 0;

int buttonPlayDelay = 20;
int buttonPlayCount = 0;

bool ledLight = false;
int pan = 90;
int pany;
int panz = 180;
int pang = 0;
//int panw = 0;
int panw2 = 0;

Servo servoBase;
Servo servoShoulder;
Servo servoElbow;
Servo servoGripper;
//Servo servoWrist;
Servo servoWrist2;

int notPressed = 0;
int notPressed2 = 0;
const int inPressed = 2;
const int inPressed2 = 4;

void setup() {
  Serial.begin(9600);

  servoBase.attach(basePin);
  servoShoulder.attach(shoulderPin);
  servoElbow.attach(elbowPin);
  servoGripper.attach(gripperPin);
  pinMode(inPressed, INPUT_PULLUP); // we use a pullup-resistor for the button functionality
  pinMode(inPressed2, INPUT_PULLUP); // we use a pullup-resistor for the button functionality
//  servoWrist.attach(wrist1Pin);
  servoWrist2.attach(wrist2Pin);
  
  StartPosition();

}

void loop() {

  //  Serial.print(buttonRecord);
  //  Serial.print("\t");
  //  Serial.println(buttonPlay);
  //  for testing purposes
//
  float dx = map(analogRead(xdirPin), 0, 1023, 0, 180);
  float dy = map(analogRead(ydirPin), 0, 1023, 0, 180);
  float dz = map(analogRead(zdirPin), 0, 1023, 0, 180);
  float dg = map(analogRead(gdirPin), 0, 1023, 0, 180);
  //if (abs(dx) < 1.5) dx = 0;
  //if (abs(dy) < 1.5) dy = 0;
  //if (abs(dz) < 1.5) dz = 0;
  //if (abs(dg) < 1.5) dg = 0;

  //posBase += dx;
  //posShoulder += dy;
  //posElbow += dz;
  //posGripper += dg;

//  if (posBase > maxBase) posBase = maxBase;
//    if (posShoulder > maxShoulder) posShoulder = maxShoulder;
//  if (posElbow > maxElbow) posElbow = maxElbow;
//  if (posGripper > maxGripper) posGripper = maxGripper;

//  if (posBase < minBase) posBase = minBase;
 // if (posShoulder < minShoulder) posShoulder = minShoulder;
//  if (posElbow < minElbow) posElbow = minElbow;
//  if (posGripper < minGripper) posGripper = minGripper;

 // notPressed = digitalRead(inPressed); 
 // notPressed2 = digitalRead(inPressed2); 


////
 // notPressed = digitalRead(inPressed); 
  notPressed2 = digitalRead(inPressed2); 
 
  delay(20);

 // if(notPressed == 0) // if the mapped joystick value is above 95, increase Pan value
 // {
  
 // panw=0;
 //   Serial.println(" pressed: ");
  
  
 // }
 // else // if the mapped joystick valie is below 85, decrease Pan value
 // {
  
 // panw=180;
 //  Serial.println(" NOTpressed: ");
  
 // }

 // servoWrist.write(panw); // move Servo to Pan value

 // delay(20);
 
  if(notPressed2 == 0) // if the mapped joystick value is above 95, increase Pan value
  {
      Serial.print("pressed: ");
    if((presscnt % 2) == 0)
    {
      panw2=0;
      Serial.println(" even: ");
      Serial.println(presscnt);
      servoWrist2.write(panw2); // move Servo to Pan value
      presscnt=presscnt+1;
    } else {
      Serial.println(presscnt);
      panw2=90;
      Serial.println(" odd: ");
      presscnt=presscnt+1;

      servoWrist2.write(panw2); // move Servo to Pan value
    }
  } else {
        Serial.println(notPressed);
  }
//  else // if the mapped joystick valie is below 85, decrease Pan value
//  {
  
//  panw2=90;
//   Serial.println(" NOTpressed2: ");
  
//  }
 
//  if((presscnt % 2) == 0)
//  {
//    panw2=0;
//    Serial.println(" even: ");
//    Serial.println(presscnt);
//  } 
  
  
//  servoWrist2.write(panw2); // move Servo to Pan value

  delay(20);
//


if(dx > 95) // if the mapped joystick value is above 95, increase Pan value
{

pan=pan+5;


}
if(dx < 85) // if the mapped joystick valie is below 85, decrease Pan value
{

pan=pan-5;


}
// Serial.print("Panx");
//Serial.print(pan);
if (pan > 180) pan = maxBase;
if (pan < 0) pan = minBase;

  servoBase.write(pan); // move Servo to Pan value

 // delay(20);

if(dy > 95) // if the mapped joystick value is above 95, increase Pan value
{
pany=pany+5;
}
if(dy < 85) // if the mapped joystick valie is below 85, decrease Pan value
{
pany=pany-5;
}
if (pany > 79) pany = maxShoulder;
 Serial.print("Pany");
Serial.println(pany);
if (pany < 0) pany = 0;

  servoShoulder.write(pany); // move Servo to Pan value

  //servoBase.write(dx);
  //servoShoulder.write(posShoulder);
  //servoElbow.write(posElbow);
//  delay(20);


if(dz > 95) // if the mapped joystick value is above 95, increase Pan value
{
panz=panz+3;
}
if(dz < 85) // if the mapped joystick valie is below 85, decrease Pan value
{
panz=panz-3;
}
//  Serial.print(panz);
  if (panz < 105) panz = maxElbow;
  if (panz < 0) panz = minElbow;

  servoElbow.write(panz); // move Servo to Pan value
 Serial.print("Panz");
Serial.println(panz);
  //servoBase.write(dx);
  //servoShoulder.write(posShoulder);
  //servoElbow.write(posElbow);
//  delay(20);

if(dg > 95) // if the mapped joystick value is above 95, increase Pan value
{
pang=pang+1;
}
if(dg < 85) // if the mapped joystick valie is below 85, decrease Pan value
{
pang=pang-1;
}
  Serial.print("Pang");
  Serial.print(pang);
  if (pang > 130) pang = maxGripper;
  if (pang < 10) pang = minGripper;

  servoGripper.write(pang); // move Servo to Pan value

 
  //delay(20);

}



void StartPosition()
{
  int angleBase = servoBase.read();
  int angleShoulder = servoShoulder.read();
  int angleElbow = servoElbow.read();
  int angleGripper = servoGripper.read();

  posBase = startBase;
  posShoulder = startShoulder;
  posElbow = startElbow;
  posGripper = startGripper;
  posWrist = startWrist;
  posWrist2 = startWrist2;

  servoBase.write(posBase);
  servoShoulder.write(posShoulder);
  servoElbow.write(posElbow);
  servoGripper.write(posGripper);
//  servoWrist.write(posWrist);
  servoWrist2.write(posWrist2);
}
