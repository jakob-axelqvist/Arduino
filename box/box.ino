#define enA 5 // Enable1 L298 Pin enA
#define in1 7 // Motor1 L298 Pin in1
#define in2 8 // Motor1 L298 Pin in2
#define in3 9 // Motor2 L298 Pin in3
#define in4 10 // Motor2 L298 Pin in4
#define enB 6 // Enable2 L298 Pin enB
#define L_S A0 // IR sensor Left
//#define R_S A1 // IR sensor Right
#define echo A2 // Echo pin
#define trigger A3 // Trigger pin
#define servoPin A5 // Servo pin
 
#include <Servo.h>
 
Servo myServo;
int Set = 15;
int distance_L, distance_F, distance_R;
 
void setup() {
    Serial.begin(9600); // Start serial communication at 9600bps
    // pinMode(R_S, INPUT); // Declare IR sensor as input
    pinMode(L_S, INPUT); // Declare IR sensor as input
    pinMode(echo, INPUT); // Declare ultrasonic sensor Echo pin as input
    pinMode(trigger, OUTPUT); // Declare ultrasonic sensor Trigger pin as output
    pinMode(enA, OUTPUT); // Declare as output for L298 Pin enA
    pinMode(in1, OUTPUT); // Declare as output for L298 Pin in1
    pinMode(in2, OUTPUT); // Declare as output for L298 Pin in2
    pinMode(in3, OUTPUT); // Declare as output for L298 Pin in3
    pinMode(in4, OUTPUT); // Declare as output for L298 Pin in4
    pinMode(enB, OUTPUT); // Declare as output for L298 Pin enB
 
    analogWrite(enA, 200); // Set Motor1 Speed
    analogWrite(enB, 200); // Set Motor2 Speed
 
    myServo.attach(servoPin);
    scanServo();
    distance_F = Ultrasonic_read();
    delay(500);

    stop();
}
 
// Define constants for sensor states
const int WHITE = LOW;
const int BLACK = HIGH;
const int DISTANCE_THRESHOLD = 20; // Adjust this value as needed
 
void loop() {
  // om vi är på vägen
  if (is_on_road()) {
    Serial.println("on road");
    // kör framåt
    forward();

    // om det är ett hinder på vägen
    if (is_obstacle_on_road()) {
      Serial.println("obstacle on road");
      // tag höger
      stop();
      backward(1000);
      turn_right(1000);
    }
  } else {
    stop();
  }
} 

bool is_on_road() {
  int read = digitalRead(L_S);
  return (read == WHITE);
}

bool is_obstacle_on_road() {
  int distance = Ultrasonic_read();
  Serial.print("distance: "); Serial.println(distance);
  return ((distance > 0) && (distance < DISTANCE_THRESHOLD));
}

void backward(int ms) {
  Serial.println("backward");
  digitalWrite(in2, HIGH); // back höger
  digitalWrite(in4, HIGH); // back vänster  
  delay(ms);
}

void forward() {
  Serial.println("forward");
  digitalWrite(in1, HIGH); // framåt höger
  digitalWrite(in3, HIGH); // framåt vänster
}

void turn_right(int ms) {
  Serial.println("turn_right");
  stop();
  digitalWrite(in3, HIGH); //  framåt vänster
  digitalWrite(in2, HIGH); //  bakåt höger
  delay(ms);
  stop();
}

void stop() {
  Serial.println("stop");
  digitalWrite(in1, LOW); // stopp framåt höger
  digitalWrite(in2, LOW); // stopp bakåt höger
  digitalWrite(in3, LOW); // stopp framåt vänster
  digitalWrite(in4, LOW); // stopp bakåt vänster
}

void forward(int ms) {
  forward();
  delay(ms);
  stop();
}

void loop_old() {
    // Line Follower and Obstacle Avoiding
    int distance_F = Ultrasonic_read();
    Serial.print("D F=");
    Serial.println(distance_F);
 
    // Read the single line sensor
    int sensorState = digitalRead(L_S);
 
    // If the sensor is on white color, move forward or avoid obstacle
    if (sensorState == WHITE) {
        Serial.println("Sensor on white, moving forward or checking side");
        if (distance_F > DISTANCE_THRESHOLD) {
            forward();
        } else {
            stopMovement();
            checkSides();
        }
    }
    // If the sensor is on black, turn right
    else if (sensorState == BLACK) {
        Serial.println("Sensor on black, turning right");
        turnRight();
    }
 
    delay(10)
    ;
}
 
void scanServo() {
    for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(angle);
    }
    for (int angle = 140; angle >= 0; angle -= 5) {
        servoPulse(angle);
    }
    for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(angle);
    }
}
 
void servoPulse(int angle) {
    int pwm = (angle * 11) + 500; // Convert angle to microseconds
    myServo.writeMicroseconds(pwm);
    delay(50); // Refresh cycle of servo
}
 
long Ultrasonic_read() {
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    long duration = pulseIn(echo, HIGH);
    return duration / 29 / 2; // Convert to distance in cm
}
 
void checkSides() {
    scanServo();
    distance_L = Ultrasonic_read();
    myServo.write(90); // Center position
    delay(500);
    distance_R = Ultrasonic_read();
 
    Serial.print("D L=");
    Serial.println(distance_L);
    Serial.print("D R=");
    Serial.println(distance_R);
 
    if (distance_L > distance_R && distance_L > Set) {
        turnLeft();
    } else if (distance_R > distance_L && distance_R > Set) {
        turnRight();
    } else {
        moveBackward();
        delay(500);
        stopMovement();
    }
}

void forward_old() {
    digitalWrite(in1, HIGH); // Left motor forward
    digitalWrite(in2, LOW); // Left motor backward
    digitalWrite(in3, HIGH); // Right motor forward
    digitalWrite(in4, LOW); // Right motor backward
}
 
void moveBackward() {
    digitalWrite(in1, LOW); // Left motor backward
    digitalWrite(in2, HIGH); // Left motor forward
    digitalWrite(in3, LOW); // Right motor backward
    digitalWrite(in4, HIGH); // Right motor forward
}
 
void turnRight() {
    digitalWrite(in1, LOW); // Left motor forward
    digitalWrite(in2, HIGH); // Left motor backward
    digitalWrite(in3, LOW); // Right motor backward
    digitalWrite(in4, HIGH); // Right motor forward
    delay(500);
    stopMovement();
}
 
void turnLeft() {
    digitalWrite(in1, HIGH); // Left motor backward
    digitalWrite(in2, LOW); // Left motor forward
    digitalWrite(in3, HIGH); // Right motor forward
    digitalWrite(in4, LOW); // Right motor backward
    delay(500);
    stopMovement();
}
 
void stopMovement() {
    digitalWrite(in1, LOW); // Left motor stop
    digitalWrite(in2, LOW); // Left motor stop
    digitalWrite(in3, LOW); // Right motor stop
    digitalWrite(in4, LOW); // Right motor stop
}