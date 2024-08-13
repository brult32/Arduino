unsigned long timeOut = 900000;  //10min 600000
unsigned long previousMillis = 0;
unsigned long interval = 1000;

const int buzzerPin = 9;
const int ledInterval = 8;
int ledTimerPins[] = { 2, 3, 4, 5 };

int loopAllLeds = 3;
unsigned long ledCounter[] = { 0, 0, 0, 0 };
bool ledTimerPassed[] = { false, false, false, false };

int ledIntervalState = HIGH;

bool playNoise = false;
bool endTime = false;

float melody[] = { 261.63, 196, 196, 220, 196, 0, 247.94, 261.63 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void playBuzz() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledInterval, OUTPUT);

  for (int i = 0; i <= loopAllLeds; i++) {
    pinMode(ledTimerPins[i], OUTPUT);
    digitalWrite(ledTimerPins[i], LOW);
  }

  long onePercent = timeOut / 100;
  int tempNumbers[] = { 25, 50, 75, 90 };
  for (int i = 0; i <= loopAllLeds; i++) {
    ledCounter[i] = onePercent * tempNumbers[i];
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (playNoise) {
    playBuzz();
  }
  if (!endTime && currentMillis >= timeOut) {
    playNoise = true;
    endTime = true;
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
