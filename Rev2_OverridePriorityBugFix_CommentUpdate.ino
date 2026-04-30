/*
------------------------------------------------------------
Project Title: Arduino Intelligent Energy Efficient Lighting Control
Purpose:
Controls prototype lighting using ambient light sensing,
occupancy demand, and manual override.

Changes:
Bug fix so that override works at all light conditions now, where as before
it would only work in normal (high) lighting conditions
------------------------------------------------------------
*/

//PIN DEFINITIONS
const int ldrPin = A0;          // Ambient light sensor
const int potPin = A1;          // Threshold adjustment potentiometer

const int occupancyButton = 2;  // Simulated occupancy input
const int overrideButton = 3;   // Manual override input

const int dimLED = 5;           // Dim lighting LED (PWM)
const int fullLED = 6;          // Full lighting LED (PWM)
const int standbyLED = 7;       // Standby indicator LED

//VARIABLES
int lightValue = 0;
int thresholdValue = 0;

bool occupancyState = false;
bool overrideState = false;

//SETUP
void setup() {
  
  pinMode(occupancyButton, INPUT_PULLUP);
  pinMode(overrideButton, INPUT_PULLUP);

  pinMode(dimLED, OUTPUT);
  pinMode(fullLED, OUTPUT);
  pinMode(standbyLED, OUTPUT);

  Serial.begin(9600);
}

//MAIN LOOP
void loop() {

  // Read analogue sensor values
  lightValue = analogRead(ldrPin);
  thresholdValue = analogRead(potPin);

  // Read push buttons
  // Buttons are active LOW because of INPUT_PULLUP
  occupancyState = (digitalRead(occupancyButton) == LOW);
  overrideState = (digitalRead(overrideButton) == LOW);

  // Small debounce delay for button stability
  delay(30);

  // Print values to Serial Monitor for calibration/testing
  Serial.print("Light Sensor: ");
  Serial.print(lightValue);
  Serial.print(" | Threshold: ");
  Serial.print(thresholdValue);
  Serial.print(" | Occupancy: ");
  Serial.print(occupancyState);
  Serial.print(" | Override: ");
  Serial.println(overrideState);

  // =========================================================
  // STATE 0: MANUAL OVERRIDE
  // Forces lights ON in all ambient conditions
  // =========================================================
  if (overrideState) {

    analogWrite(dimLED, 180);
    analogWrite(fullLED, 255);
    digitalWrite(standbyLED, LOW);
  }

  // =========================================================
  // STATE 1: BRIGHT ROOM (ENOUGH NATURAL LIGHT)
  // Turn lights (apart from stanby) off
  // =========================================================
  else if (lightValue > thresholdValue) {

    analogWrite(dimLED, 0);
    analogWrite(fullLED, 0);
    digitalWrite(standbyLED, HIGH);
  }

  // =========================================================
  // STATE 2: MEDIUM LIGHT + ROOM OCCUPIED
  // Turns dimLED (Yellow) on
  // =========================================================
  else if (lightValue <= thresholdValue &&
           lightValue > (thresholdValue * 0.5) &&
           occupancyState) {

    analogWrite(dimLED, 120);
    analogWrite(fullLED, 0);
    digitalWrite(standbyLED, LOW);
  }

  // =========================================================
  // STATE 3: DARK ROOM + ROOM OCCUPIED
  // Turns fullLED (Red) on
  // =========================================================
  else if (lightValue <= (thresholdValue * 0.5) &&
           occupancyState) {

    analogWrite(dimLED, 0);
    analogWrite(fullLED, 255);
    digitalWrite(standbyLED, LOW);
  }

  // =========================================================
  // STATE 4: DARK/MEDIUM ROOM BUT UNOCCUPIED = ENERGY SAVING
  // only standby on
  // =========================================================
  else {

    analogWrite(dimLED, 0);
    analogWrite(fullLED, 0);
    digitalWrite(standbyLED, HIGH);
  }

  delay(300);
}
