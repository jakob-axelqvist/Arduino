#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo grip;

void setup() {
  // Setup display
  setupDisplay();

  // Setup all servos
  setupServo(base, "base", 9);
  setupServo(shoulder, "shoulder", 6);
  setupServo(elbow, "elbow", 5);
  setupServo(wrist, "wrist", 3);
  setupServo(grip, "grip", 11);
}

void loop() {
  setInitialPosition();

  openGrip();
  lowerElbow();
  closeGrip();
  raiseElbow();
  moveBase(0);
  lowerElbow();
  openGrip();
  raiseElbow();
}

void moveBase(int angle) {
  move(base, "move base", angle);
}

void setInitialPosition() {
  move(elbow, "init elbow", 40);
  move(base, "init base", 90);
  move(wrist, "init wrist", 90);
  move(grip, "init grip", 0);
  delay(5000);
}

void openGrip() {
  move(grip, "open grip", 0);
}

void lowerElbow() {
  move(elbow, "lower elbow", 75);
}

void closeGrip() {
  move(grip, "close grip", 70);
}

void raiseElbow() {
  move(elbow, "raise elbow", 50);
}

void rotateWrist() {
  move(wrist, "rotate wrist", 360);
}

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("START"));
  display.display();
  delay(2000);
}

void setupServo(Servo& servo, String name, int pin) {
  updateDisplay("setup " + name);
  servo.attach(pin);
  delay(100);
  servo.detach();
  delay(100);
  servo.attach(pin);
  delay(100);
}

void move(Servo& servo, String servoName, int angle) {
  updateDisplay(servoName + ": " + angle);
  servo.write(angle);
  delay(2000);
}

void updateDisplay(String text) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}