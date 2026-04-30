/*
------------------------------------------------------------
Project Title: Arduino Intelligent Energy Efficient Lighting Control
Purpose:
Controls prototype lighting using ambient light sensing,
occupancy demand, and manual override.
------------------------------------------------------------
*/

//PIN DEFINITIONS 
const int ldrPin = A0;          // Ambient light sensor
const int potPin = A1;          // User threshold adjustment

const int occupancyButton = 2;  // Simulated occupancy input
const int overrideButton = 3;   // Manual override input

const int dimLED = 5;           // PWM dim lighting
const int fullLED = 6;          // PWM full lighting
const int standbyLED = 7;       // standby indicator

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

  // Read analogue values
  lightValue = analogRead(ldrPin);
  thresholdValue = analogRead(potPin);

  // Read button states (LOW means pressed due to INPUT_PULLUP)
  occupancyState = (digitalRead(occupancyButton) == LOW);
  overrideState = (digitalRead(overrideButton) == LOW);

  // Print values for calibration/testing
  Serial.print("Light Sensor: ");
  Serial.print(lightValue);
  Serial.print(" | Threshold: ");
  Serial.print(thresholdValue);
  Serial.print(" | Occupancy: ");
  Serial.print(occupancyState);
  Serial.print(" | Override: ");
  Serial.println(overrideState);

  // ----------- STATE 1: BRIGHT ROOM + NO OVERRIDE -----------
  if (lightValue > thresholdValue && !overrideState) {

    analogWrite(dimLED, 0);
    analogWrite(fullLED, 0);
    digitalWrite(standbyLED, HIGH);

  }

  // ----------- STATE 2: BRIGHT ROOM + OVERRIDE PRESSED -----------
  else if (lightValue > thresholdValue && overrideState) {

    analogWrite(dimLED, 150);
    analogWrite(fullLED, 200);
    digitalWrite(standbyLED, LOW);

  }

  // ----------- STATE 3: MEDIUM LIGHT + OCCUPIED -----------
  else if (lightValue <= thresholdValue && lightValue > (thresholdValue * 0.5) && occupancyState) {

    analogWrite(dimLED, 120);
    analogWrite(fullLED, 0);
    digitalWrite(standbyLED, LOW);

  }

  // ----------- STATE 4: DARK ROOM + OCCUPIED -----------
  else if (lightValue <= (thresholdValue * 0.5) && occupancyState) {

    analogWrite(dimLED, 0);
    analogWrite(fullLED, 255);
    digitalWrite(standbyLED, LOW);

  }

  // ----------- STATE 5: DARK ROOM + UNOCCUPIED -----------
  else {

    analogWrite(dimLED, 0);
    analogWrite(fullLED, 0);
    digitalWrite(standbyLED, HIGH);

  }

  delay(300);
}
