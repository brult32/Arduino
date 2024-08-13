unsigned long timeOut = 600000;  //10min 600000
unsigned const long partnerTime = timeOut;
int extraCounter = -1;
unsigned long previousMillis = 0;
unsigned long interval = 1000;

const int buzzerPin = 9;
const int ledInterval = 8;
int ledTimerPins[] = { 2, 3, 4, 5 };
int ledPartner = 7;

int loopAllLeds = 3;
unsigned long ledCounter[] = { 0, 0, 0, 0 };
bool ledTimerPassed[] = { false, false, false, false };

int ledIntervalState = HIGH;

const int buttonResetPin = 10;
int butResetState = 0;

const int buttonPartnerPin = 11;
int buttonPartnerState = 0;

const int buttonStopNoisePin = 12;
int buttonStopNoiseState = 0;
bool playNoise = false;
bool doneTime = false;

float melody[] = { 261.63, 196, 196, 220, 196, 0, 247.94, 261.63 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void (*resetFunc)(void) = 0;  //declare reset function @ address 0

void playBuzz() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void timeEnded() {
  if (!doneTime) {
    playNoise = true;
    doneTime = true;
  }
}
void afterEnd() {
  if (playNoise) {
    playBuzz();
  }
  if (doneTime) {
    buttonStopNoiseState = digitalRead(buttonStopNoisePin);
    if (buttonStopNoiseState == HIGH) {
      digitalWrite(buzzerPin, LOW);
      playNoise = false;
    }
  }
}

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledInterval, OUTPUT);
  pinMode(buttonResetPin, INPUT);
  pinMode(buttonStopNoisePin, INPUT);
  for (int i = 0; i <= loopAllLeds; i++) {
    pinMode(ledTimerPins[i], OUTPUT);
    digitalWrite(ledTimerPins[i], LOW);
  }

  pinMode(ledPartner, OUTPUT);
  buttonPartnerState = digitalRead(buttonPartnerPin);
  if (buttonPartnerState == HIGH) {
    timeOut += partnerTime;
    digitalWrite(ledPartner, HIGH);
  } else {
    digitalWrite(ledPartner, LOW);
  }

  long onePercent = timeOut / 100;
  int tempNumbers[] = { 25, 50, 75, 90 };
  for (int i = 0; i <= loopAllLeds; i++) {
    ledCounter[i] = onePercent * tempNumbers[i];
  }
}

void loop() {
  unsigned long currentMillis = millis();
  butResetState = digitalRead(buttonResetPin);
  if (butResetState == HIGH) {
    Serial.println("resetting");
    resetFunc();  //call reset
  }
  afterEnd();
  if (currentMillis >= timeOut) {
    timeEnded();
  } else {
    for (int i = 0; i <= loopAllLeds; i++) {
      if (!ledTimerPassed[i] && currentMillis > ledCounter[i]) {
        ledTimerPassed[i] = true;
        digitalWrite(ledTimerPins[i], HIGH);
      }
    }
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledIntervalState == LOW) {
        ledIntervalState = HIGH;
      } else {
        ledIntervalState = LOW;
      }
      digitalWrite(ledInterval, ledIntervalState);
    }
  }
}
