#include <SoftwareServo.h>
#include <WaveHC.h>
#include <WaveUtil.h>

#include <Ping.h>  //include the ping)) libs
//#include <Servo.h> // includes the servo libs

#define LEFTSERVOPIN  9
#define RIGHTSERVOPIN  6
#define HEADSERVOPIN  8
#define PINGPIN 7        // Ultrasound signal pin
#define LEFTSERVOCALIB 52  //servo zero out
#define RIGHTSERVOCALIB 79  //servo zero out

SoftwareServo leftServo;       // name the servo
SoftwareServo rightServo;      // name the servo
SoftwareServo headServo;
Ping ping = Ping(PINGPIN);

SdReader card;
FatVolume vol;
FatReader root;
FatReader file;
WaveHC wave;
#define error(msg) error_P(PSTR(msg))

int speed = 10;

void setup(){
   //pinMode(RIGHTSERVOPIN,OUTPUT);
   //pinMode(LEFTSERVOPIN,OUTPUT);
   //pinMode(HEADSERVOPIN,OUTPUT);
   //leftServo.attach(LEFTSERVOPIN);	 // Attaches the Left Servo to PWM pin 6 on the Arduino
   //rightServo.attach(RIGHTSERVOPIN);   // Attaches the Right Servo to PWM pin 5 on the Arduino
   //headServo.attach(HEADSERVOPIN);
   rightServo.attach(RIGHTSERVOPIN);
   rightServo.setMaximumPulse(2200);
   leftServo.attach(LEFTSERVOPIN);
   leftServo.setMaximumPulse(2200);
   headServo.attach(HEADSERVOPIN);
   headServo.setMaximumPulse(2200);
   goStop();
   Serial.begin(9600);                  // Sets the baud rate to 9600
   turnHeadRight();
   turnHeadLeft();
   turnHeadForward();
   
   PgmPrintln("Pi speaker");
  
  if (!card.init()) {
    error("Card init. failed!");
  }
  if (!vol.init(card)) {
    error("No partition!");
  }
  if (!root.openRoot(vol)) {
    error("Couldn't open dir");
  }

  PgmPrintln("Files found:");
  root.ls();
}

void loop() 
{
  double distanceForward = 0;
  double distanceRight = 0;
  double distanceLeft = 0;
  
  distanceForward = lookForward();
  if(distanceForward > 12)
  {
    goForward(0);
    delay(700);
    if(distanceForward > 24)
    {
      playfile("AllClear");
    }
  }
  else
  {
    goForward(-7);
    distanceRight = lookRight();
    distanceLeft = lookLeft();
    if(distanceRight > distanceForward && distanceRight > distanceLeft)
    {
      playfile("TurnRight");
      goRight();
      delay(600);
      goStop();
    }
    else if(distanceLeft > distanceForward && distanceLeft > distanceRight)
    {
      playfile("TurnLeft");
      goLeft();
      delay(600);
      goStop(); 
    }
    else
    {
      goForward(-5);
      delay(100);
      goStop();
    }
  } 
  SoftwareServo::refresh();
}

void setupRobot()
{
    pinMode(RIGHTSERVOPIN,OUTPUT);
   pinMode(LEFTSERVOPIN,OUTPUT);
   pinMode(HEADSERVOPIN,OUTPUT);
   leftServo.attach(LEFTSERVOPIN);	 // Attaches the Left Servo to PWM pin 6 on the Arduino
   rightServo.attach(RIGHTSERVOPIN);   // Attaches the Right Servo to PWM pin 5 on the Arduino
   headServo.attach(HEADSERVOPIN);
   goStop();
}

void playfile(char *name) {
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  if (!file.open(root, name)) {
    PgmPrint("Couldn't open file ");
    Serial.print(name); 
    return;
  }
  if (!wave.create(file)) {
    PgmPrintln("Not a valid WAV");
    return;
  }
  // ok time to play!
  wave.play();
}
/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

/*
 * turn Head Forward
 */
void turnHeadForward(){
 headServo.write(90);
 delay(300);
}
/*
 * turn Head Right
 */
void turnHeadRight(){
 headServo.write(179);
 delay(700);
}
/*
 * turn Head Left
 */
void turnHeadLeft(){
 headServo.write(0);
 delay(700);
}

/*
 * Look Forward
 */
double lookForward(){
 turnHeadForward();
 ping.fire();
 delay(50);
 return ping.inches();
}
/*
 * Look Right
 */
double lookRight(){
  turnHeadRight();
  ping.fire();
  delay(50);
  turnHeadForward();
  return ping.inches();
}
/*
 * Look Left
 */
double lookLeft(){
  turnHeadLeft();
  ping.fire();
  delay(50);
  turnHeadForward();
  return ping.inches();
}


/*
 * sends the robot forwards
 */
void goForward(int faster){
 leftServo.write(LEFTSERVOCALIB - speed - faster);
 rightServo.write(RIGHTSERVOCALIB + speed + faster);
}

/*
 * sends the robot backwards
 */
void goBackward(int faster){
 leftServo.write(LEFTSERVOCALIB + speed + faster);
 rightServo.write(RIGHTSERVOCALIB - speed - faster);
}
  
/*
 * sends the robot right
 */
void goRight(){
 leftServo.write(LEFTSERVOCALIB - speed);
 rightServo.write(RIGHTSERVOCALIB - speed);
}

/*
 * sends the robot left
 */
void goLeft(){
 leftServo.write(LEFTSERVOCALIB + speed);
 rightServo.write(RIGHTSERVOCALIB + speed);
}

/*
 * stops the robot
 */
void goStop(){
 leftServo.write(LEFTSERVOCALIB);
 rightServo.write(RIGHTSERVOCALIB);
}
