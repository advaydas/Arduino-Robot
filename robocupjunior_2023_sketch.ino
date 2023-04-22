/*
This dancing dino robot was made of a chasis with 2 DC motors
For the body of the dino robot we had 2 servo motors 
  Servo motor 1 was used to sweep the head up and down
  Servo motor 2 was used to sweep the legs forward and backward
The legs of the dino did not make contact with the ground and moved in the air
This Arduino code sketch was used to move the 3 motors at the same time

Language+platform: Arduino UNO R3/Arduino IDE
DC motor driver board; Adafruit DRV8871
Servo motor: SG90R

Author-Guides: Ambardeep Das, Martin Woudstra
Students: Advay, Lukas, Dine, Galang, Samay
Team name: GLADS
Category: Dancing Robot
Result: 1st prize!!
*/ 

#include <Servo.h>

Servo myservo_head;   // create servo object to control the dino head servo
Servo myservo_legs;   // create servo object to control the dino leg servo

#define  MIN_POS 0    // position range lower limit for servo motors
#define  MAX_POS 30   // position range upper limit for servo motors

#define SERVO_HEAD 7  // Dino head servo control pin
#define SERVO_LEGS 8  // Dino legs servo control pin

#define MOTOR_RIGHT_IN1 6   // DC motor 1 control pin
#define MOTOR_RIGHT_IN2 5   // DC motor 1 control pin
#define MOTOR_LEFT_IN1 11  // DC motor 2 control pin
#define MOTOR_LEFT_IN2 3  // DC motor 2 control pin

#define SPEED 150     // Speed at which DC motor will run

#define FORWARD 1
#define BACKWARD -1
#define STOPPED 0
#define FREE_WHEELING 10

int direction = STOPPED;

void setup() {
  Serial.begin(9600);

  myservo_head.attach(SERVO_HEAD);  // create servo object to control dino head servo motor
  myservo_legs.attach(SERVO_LEGS);  // create servo object to control dino legs servo motor

  // Setup control pins for DC motor 1
  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);

  // Setup control pins for DC motor 2
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  
  wheels_free(); // Set both motors to move freely
  myservo_head.write(MIN_POS);  // tell dino head servo to go to lowest position
  myservo_legs.write(MIN_POS);  // tell dino legs servo to go to lowest position
}

void wheels_free() {
  Serial.println("free");
  direction = FREE_WHEELING;
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
}

void wheels_forward(int speed = SPEED) {
  Serial.println("forward");
  direction = FORWARD;
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  analogWrite(MOTOR_RIGHT_IN1, speed);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_IN1, speed);
}

void wheels_backward(int speed = SPEED) {
  Serial.println("backward");
  direction = BACKWARD;
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  analogWrite(MOTOR_RIGHT_IN2, speed);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  analogWrite(MOTOR_LEFT_IN2, speed);
  digitalWrite(MOTOR_LEFT_IN1, LOW);
}

void wheels_stop() {
  Serial.println("stopped");  
  direction = STOPPED;
  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
}

void set_servos(int pos){

  // Set and move the dino legs
  int legs_pos = -1;
  if (direction == FORWARD) {
    legs_pos = map(pos, MIN_POS, MAX_POS, (MAX_POS + MIN_POS)/2, MIN_POS);
    
  } else if (direction == BACKWARD) {
    legs_pos = map(pos, MIN_POS, MAX_POS, (MAX_POS + MIN_POS)/2, MAX_POS);
  }
  if (legs_pos >= 0){
    myservo_legs.write(legs_pos);
  }

  // Set and move the dino head
  int head_pos = map(pos, MIN_POS, MAX_POS, MAX_POS * 1.5, MIN_POS);  
  myservo_head.write(head_pos);

  // wait while the servo to reaches the position and gives some time for next move  
  delay(15);
}

void servo_sweep(int start_pos, int end_pos){
  if (start_pos < end_pos){
    for (int pos = start_pos; pos <= end_pos; ++pos) {
      set_servos(pos);
    }
  } else if (start_pos > end_pos){
    for (int pos = start_pos; pos >= end_pos; --pos) {
      set_servos(pos);
    }
  }
}

void full_sweep(int repeat)
{
  for (int i = 0; i < repeat; ++i) {
      servo_sweep(MIN_POS, MAX_POS);
      servo_sweep(MAX_POS, MIN_POS);
    }
}


void forward(int steps) {
  wheels_forward();
  full_sweep(steps);
}

void backward(int steps) {
  wheels_backward();
  full_sweep(steps);
}

void stop(int steps=0) {
  wheels_free();
  delay(200);
  wheels_stop();
  full_sweep(steps);
}

void loop() {
  
  stop(1);
  forward(2);
  stop();
  backward(1);

}