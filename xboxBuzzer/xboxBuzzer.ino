unsigned long onePercent = 300;
unsigned long timeOut = onePercent * 100;  //10min 600000
const long extraTime = timeOut;  //10min 600000
int extraCounter = -1;           //10min 600000
unsigned long previousMillis = 0;
unsigned long interval = 1000;

const int buzzerPin = 9;
const int ledInterval = 8;
int ledTimerPins[] = { 2, 3, 4, 5 };
int ledExtraPins[] = { 6, 7 };

int loopAllLeds = 3;
unsigned long ledCounter[] = { onePercent * 25, onePercent * 50, onePercent * 75, onePercent * 90 };
bool ledTimerPassed[] = { false, false, false, false };

int ledState = HIGH;

const int buttonResetPin = 10;
int butResetState = 0;

const int buttonChangeTimePin = 11;
int buttonChangeTimeState = 0;
bool buttonChangeTimePressed = false;

const int buttonStopNoisePin = 12;
int buttonStopNoiseState = 0;
bool playNoise = false;
bool doneTime = false;

float melody[] = { 261.63, 196, 196, 220, 196, 0, 247.94, 261.63 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledInterval, OUTPUT);
  pinMode(buttonResetPin, INPUT);
  pinMode(buttonStopNoisePin, INPUT);
  for (int i = 0; i < loopAllLeds; i++) {
    pinMode(ledTimerPins[i], OUTPUT);
    digitalWrite(ledTimerPins[i], LOW);
  }
  for (int i = 0; i < 2; i++) {
    pinMode(ledExtraPins[1], OUTPUT);
    digitalWrite(ledExtraPins[0], LOW);
  }
}

void playBuzz() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void (*resetFunc)(void) = 0;  //declare reset function @ address 0

void timeEnd(){
if (!doneTime) {
      playNoise = true;
      doneTime = true;
    }
}
void afterEnd(){
  if (playNoise) {
    playBuzz();
  }
  if (doneTime){
    buttonStopNoiseState = digitalRead(buttonStopNoisePin);
    if (buttonStopNoiseState == HIGH) {
      digitalWrite(buzzerPin, LOW);
      playNoise = false;
    }
    butResetState = digitalRead(buttonResetPin);
    if (butResetState == HIGH) {
      Serial.println("resetting");
      resetFunc();  //call reset
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();
  afterEnd();
  if (currentMillis >= timeOut) {
    timeEnd();
  } else {
    for (int i = 0; i <= loopAllLeds; i++) {
      if (!ledTimerPassed[i] && currentMillis > ledCounter[i]) {
        ledTimerPassed[i] = true;
        digitalWrite(ledTimerPins[i], HIGH);
      }
    }
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledInterval, ledState);
    }

    buttonChangeTimeState = digitalRead(buttonChangeTimePin);
    if (buttonChangeTimeState == HIGH) {
      if (!buttonChangeTimePressed) {
        buttonChangeTimePressed = true;
        timeOut += extraTime;
        extraCounter++;
        if (extraCounter >= 2) {
          timeOut = extraTime;
          extraCounter = -1;
        }
        if (extraCounter > -1) {
          digitalWrite(ledExtraPins[extraCounter], HIGH);
        } else {
          for (int i = 0; i < 2; i++) {
            digitalWrite(ledExtraPins[i], LOW);
          }
        }
      }
    }else {
      buttonChangeTimePressed = false;
    }
  }
}
