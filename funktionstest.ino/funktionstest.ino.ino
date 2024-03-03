#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

Servo servoBase;
Servo servoShoulder;
Servo servoElbow;
Servo servoWrist;
Servo servoGripper;

void setup() {
  servoBase.attach(9);
  servoShoulder.attach(6);
  servoElbow.attach(5);
  servoWrist.attach(3);
  servoGripper.attach(11);

  display("JAKOB");
  delay(3000);
}

void display(const char* text) {
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

void loop() {
  // Function test for each servo
  functionTest();
  delay(1000); // Delay between tests
}

void functionTest() {
  // Test each servo by moving it to its minimum, middle, and maximum positions
  testServo(servoBase, "Base");
  testServo(servoShoulder, "Shoulder");
  testServo(servoElbow, "Elbow");
  testServo(servoWrist, "Wrist");
  testServo(servoGripper, "Gripper");
}

void testServo(Servo &servo, const char *servoName) {
  // Move servo to minimum position
  servo.write(0);
  updateDisplay(servoName, 50);
  delay(500); // Wait for half a second

  // Move servo to middle position
  servo.write(90);
  updateDisplay(servoName, 40);
  delay(500);

  // Move servo to maximum position
  servo.write(180);
  updateDisplay(servoName, 60);
  delay(500);
}

void updateDisplay(const char *name, int angle) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(name);
  display.print(": ");
  display.print(angle);
  display.display();
}
