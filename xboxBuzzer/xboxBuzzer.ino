long timeOut = 30000;  //10min 600000
const long quarterTime = timeOut / 4;
const long extraTime = timeOut;  //10min 600000
int extraCounter = 0;  //10min 600000
unsigned long previousMillis = 0;
unsigned long interval = 1000;

const int buzzerPin = 9;
const int ledInterval = 8;
int ledTimerPins[] = {2, 3, 4, 5};
int ledExtraPins[] = {6, 7};

int loopAllLeds = 3;
int ledCounter[] = {quarterTime, quarterTime * 2, quarterTime * 3 , (timeOut / 10) * 9};
bool ledTimerPassed[] = {false, false, false, false};

int ledState = HIGH;

const int buttonResetPin = 10;
int butResetState = 0;

const int buttonChangeTimePin = 11;
int buttonChangeTimeState = 0;

const int buttonStopNoisePin = 12;
int buttonStopNoiseState = 0;
bool stopNoise = false;

int melody[] = {261.63, 196, 196, 220, 196, 0, 247.94, 261.63};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledInterval, OUTPUT);
  pinMode(buttonResetPin, INPUT);
  pinMode(buttonStopNoisePin, INPUT);
  for(int i = 0; i < loopAllLeds; i++){
    pinMode(ledTimerPins[i], OUTPUT);
  } 
  pinMode(ledExtraPins[0], OUTPUT);
  pinMode(ledExtraPins[1], OUTPUT);
}

void playBuzz(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void (*resetFunc)(void) = 0;  //declare reset function @ address 0

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis >= timeOut) {
    if (!stopNoise){
      playBuzz();
    }    
    buttonStopNoiseState = digitalRead(buttonStopNoisePin);
    if (buttonStopNoiseState == HIGH) {
      digitalWrite(buzzerPin, LOW);
      stopNoise = true;
    }
    butResetState = digitalRead(buttonResetPin);
    if (butResetState == HIGH) {
      Serial.println("resetting");
      resetFunc();  //call reset
    }
    
  } else {
    for (int i = 0; i < loopAllLeds; i++){
        if (!ledTimerPassed[i] && currentMillis > ledCounter[i]){
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
      timeOut += extraTime;
      extraCounter++;
      if(extraCounter >= 3){
        timeOut = extraTime;
        extraCounter = 0;
      }
      if (extraCounter != 0){
        digitalWrite(ledExtraPins[[extraCounter], HIGH); 
      }else{
        for (int i = 1; i < 3; i++){
          digitalWrite(ledExtraPins[[i], LOW); 
        }
      }
    }
  }
}
