const int xboxContr = 4;

const long timeOut = 30000;  //10min 600000

unsigned long previousMillis = 0;
unsigned long interval = 1000;
bool halfInterval = false;
bool quarterInterval = false;
bool lastMinute = false;
int ledState = HIGH;
const int ledTimer = 8;

const int buttonPin = 2;
int buttonState = 0;

void setup() {
  pinMode(xboxContr, OUTPUT);
  pinMode(ledTimer, OUTPUT);
  digitalWrite(xboxContr, HIGH);
  pinMode(buttonPin, INPUT);
}

void (*resetFunc)(void) = 0;  //declare reset function @ address 0

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis >= timeOut) {
    digitalWrite(xboxContr, LOW);
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.println("resetting");
      resetFunc();  //call reset
    }
  } else {
    if (currentMillis - previousMillis >= interval) {
      if (!halfInterval && currentMillis > timeOut * 0.5) {
        interval = (interval / 2);
        halfInterval = true;
      }
      if (!quarterInterval && currentMillis > timeOut * 0.75) {
        interval = (interval / 2);
        quarterInterval = true;
      }
      if (!lastMinute && currentMillis > timeOut * 0.9) {
        interval = (interval / 2);
        lastMinute = true;
      }
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledTimer, ledState);
    }
  }
}
