#include <Arduino.h>

// Pin Definitions for Sensors (Capteurs)
#define C1_PIN 14
#define C2_PIN 27
#define C3_PIN 26

// Pin Definitions for Buttons
#define BUTTON1_PIN 33
#define BUTTON2_PIN 32
#define BUTTON3_PIN 35

// Pin Definitions for Relays
#define R1_PIN 16
#define R2_PIN 17
#define R3_PIN 18
#define R4_PIN 19

// Pin Definitions for 7-Segment Display
#define SEG_A 2
#define SEG_B 3
#define SEG_C 4
#define SEG_D 5
#define SEG_E 6
#define SEG_F 7
#define SEG_G 8

#define CC_FLOOR_1 9
#define CC_FLOOR_2 10
#define CC_FLOOR_3 11

// Pin Definitions for LEDs
#define LED_FLOOR_1 12
#define LED_FLOOR_2 13
#define LED_FLOOR_3 15

// Segment bit patterns for digits 1, 2, 3
const uint8_t digitPatterns[3] = {
  0b01100000, // 1
  0b11011010, // 2
  0b11110010  // 3
};

// Function Prototypes
void moveUp();
void moveDown();
void stopMotor();
void displayFloor(int floor);
void setSegmentPins(uint8_t pattern);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Sensor Pins
  pinMode(C1_PIN, INPUT);
  pinMode(C2_PIN, INPUT);
  pinMode(C3_PIN, INPUT);

  // Initialize Button Pins
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);

  // Initialize Relay Pins
  pinMode(R1_PIN, OUTPUT);
  pinMode(R2_PIN, OUTPUT);
  pinMode(R3_PIN, OUTPUT);
  pinMode(R4_PIN, OUTPUT);

  // Initialize 7-Segment Display Segment Pins
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  // Initialize 7-Segment Display Common Cathode Pins
  pinMode(CC_FLOOR_1, OUTPUT);
  pinMode(CC_FLOOR_2, OUTPUT);
  pinMode(CC_FLOOR_3, OUTPUT);

  // Initialize LED Pins
  pinMode(LED_FLOOR_1, OUTPUT);
  pinMode(LED_FLOOR_2, OUTPUT);
  pinMode(LED_FLOOR_3, OUTPUT);

  // Initialize Relays to OFF
  digitalWrite(R1_PIN, LOW);
  digitalWrite(R2_PIN, LOW);
  digitalWrite(R3_PIN, LOW);
  digitalWrite(R4_PIN, LOW);

  // Initialize 7-Segment Displays to OFF
  digitalWrite(CC_FLOOR_1, HIGH);
  digitalWrite(CC_FLOOR_2, HIGH);
  digitalWrite(CC_FLOOR_3, HIGH);

  // Initialize LEDs to OFF
  digitalWrite(LED_FLOOR_1, LOW);
  digitalWrite(LED_FLOOR_2, LOW);
  digitalWrite(LED_FLOOR_3, LOW);

  // Display initial floor
  displayFloor(1);
}

void loop() {
  bool C1 = digitalRead(C1_PIN);
  bool C2 = digitalRead(C2_PIN);
  bool C3 = digitalRead(C3_PIN);

  bool button1 = !digitalRead(BUTTON1_PIN);  // Active low
  bool button2 = !digitalRead(BUTTON2_PIN);  // Active low
  bool button3 = !digitalRead(BUTTON3_PIN);  // Active low

  // Determine current floor
  int currentFloor = 0;
  if (C1) currentFloor = 1;
  else if (C2) currentFloor = 2;
  else if (C3) currentFloor = 3;

  // Update display
  displayFloor(currentFloor);

  // Handle Button Presses
  if (button1) {
    if (C2 || C3) {
      moveDown();
    }
  } else if (button2) {
    if (C3) {
      moveDown();
    } else if (C1) {
      moveUp();
    }
  } else if (button3) {
    if (C1 || C2) {
      moveUp();
    }
  } else {
    stopMotor();
  }
}

void moveUp() {
  digitalWrite(R1_PIN, HIGH);
  digitalWrite(R2_PIN, HIGH);
  digitalWrite(R3_PIN, LOW);
  digitalWrite(R4_PIN, LOW);
  Serial.println("Moving Up");
}

void moveDown() {
  digitalWrite(R1_PIN, LOW);
  digitalWrite(R2_PIN, LOW);
  digitalWrite(R3_PIN, HIGH);
  digitalWrite(R4_PIN, HIGH);
  Serial.println("Moving Down");
}

void stopMotor() {
  digitalWrite(R1_PIN, LOW);
  digitalWrite(R2_PIN, LOW);
  digitalWrite(R3_PIN, LOW);
  digitalWrite(R4_PIN, LOW);
  Serial.println("Motor Stopped");
}
void displayFloor(int floor) {
  // Turn off all common cathodes
  digitalWrite(CC_FLOOR_1, HIGH);
  digitalWrite(CC_FLOOR_2, HIGH);
  digitalWrite(CC_FLOOR_3, HIGH);
  // Turn off all LEDs
  digitalWrite(LED_FLOOR_1, LOW);
  digitalWrite(LED_FLOOR_2, LOW);
  digitalWrite(LED_FLOOR_3, LOW);
  // Set segment pins for the floor number
  setSegmentPins(digitPatterns[floor - 1]);
