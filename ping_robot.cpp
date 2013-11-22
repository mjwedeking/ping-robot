#include <Servo.h> // includes the servo libs

#define LEFTSERVOPIN  5
#define RIGHTSERVOPIN  6

Servo leftServo;       // name the servo
Servo rightServo;      // name the servo
int ultraSoundSignal = 7; // Ultrasound signal pin
int val = 0;
int val2 = 0;
int ultrasoundValue = 0;
int timecount = 0; // Echo counter

int speed = 80;

void setup(){
   pinMode(RIGHTSERVOPIN,OUTPUT);
   pinMode(LEFTSERVOPIN,OUTPUT);
   leftServo.attach(6);	 // Attaches the Left Servo to PWM pin 6 on the Arduino
   rightServo.attach(5); // Attaches the Right Servo to PWM pin 5 on the Arduino
   Serial.begin(9600);                  // Sets the baud rate to 9600
}

void loop() {

 // Ping))) functions: Doing more than I need it to, I think, but code

 timecount = 0;
 val = 0;
 pinMode(ultraSoundSignal, OUTPUT);   // Switch signalpin to output

  digitalWrite(ultraSoundSignal, LOW);  // Send low pulse
  delayMicroseconds(2);                 // Wait for 2 microseconds
  digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
  delayMicroseconds(5);                 // Wait for 5 microseconds
  digitalWrite(ultraSoundSignal, LOW);  // Holdoff

  pinMode(ultraSoundSignal, INPUT);     // Switch signalpin to input
  val = digitalRead(ultraSoundSignal);  // Append signal value to val
  while(val == LOW) {                   // Loop until pin reads a high value
    val = digitalRead(ultraSoundSignal);
}
  while(val == HIGH) {                  // Loop until pin reads a high value
  val = digitalRead(ultraSoundSignal);
  timecount = timecount +1;           // Count echo pulse time
}

ultrasoundValue = timecount;          // Append echo pulse time to ultrasoundValue

// Servo functions: The basics
  if(ultrasoundValue > 300){
      goForward();
      delay(300);
  }
  if(ultrasoundValue < 300){
      goRight();
     delay(150); 
  }
     if(ultrasoundValue < 100){
       goRight();
       delay(250);
  }
}

/*
 * sends the robot forwards
 */
void goForward(){
 leftServo.write(90 - speed);
 rightServo.write(90 + speed);
}

/*
 * sends the robot backwards
 */
void goBackward(){
 leftServo.write(90 + speed);
 rightServo.write(90 - speed);
}
  
/*
 * sends the robot right
 */
void goRight(){
 leftServo.write(90 + speed);
 rightServo.write(90 + speed);
}

/*
 * sends the robot left
 */
void goLeft(){
 leftServo.write(90 - speed);
 rightServo.write(90 - speed);
}

/*
 * stops the robot
 */
void goStop(){
 leftServo.write(90);
 rightServo.write(90);
}

