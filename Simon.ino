#include <Button.h>

#include "pitches.h"

#define SIZEOF_ARRAY(x)  (sizeof (x) / sizeof (x[0]) )

int simon[30] = {};
int pos = 0;

// pin outs - array {ledPin, buttonPin}
const int ledPins[4] = { 13, 12, 11, 10};
Button button1 = Button(7, BUTTON_PULLDOWN);
Button button2 = Button(6, BUTTON_PULLDOWN);
Button button3 = Button(5, BUTTON_PULLDOWN);
Button button4 = Button(4, BUTTON_PULLDOWN);

const int pinCount = sizeof(ledPins) / sizeof(int);
int ledPin = 0;
int buttonState = 0; 

int pinMelodies[][2] = {
  {NOTE_C4, 8},
  {NOTE_G3, 8},
  {NOTE_A3, 8},
  {NOTE_B3, 8},
}; 

int funeral[] = {
  NOTE_G4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_C5};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int funeralDurations[] = {
  2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,1,2,2,1};

int imperial[][2] = {
//  G G G Eb Bb G Eb Bb G(up above tuning C) D D D Eb (back down) Bb G Eb Bb G G low GG high G F# F E Eb E Ab C# C B Bb A Bb Eb F# Bb G BbD G GG G F# F E Eb E Ab C# C B Bb A Bb Eb Bb GEb Bb G
};

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  int ledPin = 0;
  for(int thisPin =0; thisPin < pinCount; thisPin++) {
    ledPin = ledPins[thisPin];
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
  }
  pickSimon();
}

void loop() {
  if (pos < 30) {
    playSequenceTo(pos);
    Serial.print("Pressed: ");
    for (int i = 0; i <= pos; i++) {
      int pinId = simon[i];
      int buttonPressed = waitForButton();
      showMe(buttonPressed);
      if (buttonPressed != pinId) {
        Serial.println("Failed on level: " + String(pos) + " by pressing " + String(buttonPressed) + " when you should have pressed " + String(pinId));
        playMelody(funeral, funeralDurations);
        showError();
        pickSimon();
        playStartSound();
        pos = -1;
      }
      Serial.print(String(buttonPressed) + "  ");
      delay(500);
    }
    Serial.println("");
    pos += 1;
  } else if (pos == 30) {
    playSuccessSound();
  }
}

void showMe(int pinId) {
  ledPin = ledPins[pinId];
  int note = pinMelodies[pinId][0];
  int duration = pinMelodies[pinId][1];
  Serial.println("note " + String(note));

  digitalWrite(ledPin, HIGH);
  playNote(note, duration);
  digitalWrite(ledPin, LOW);
  delay(250);
}

void showError() {
  int len = (sizeof(funeral) / sizeof(int));
  Serial.println("size: " + String(sizeof(funeral)) + " - " + String(len));
  for (int thisNote = 0; thisNote < len; thisNote++) {
    int note = funeral[thisNote];
    int duration = funeralDurations[thisNote];
    playNote(note, duration);
  }
  
  
  int state = LOW;
  for(int flashy = 0;flashy < 5; flashy++) {
    for(int i = 0;i < 4; i++) {
      digitalWrite(ledPins[i], state);
    }
    if (state == LOW) {
      state = HIGH;
    } else {
      state = LOW;
    }
    delay(500);
  }
//  delay(1000);
  
}

void playStartSound() {
  delay(500);
}

void playSuccessSound() {}

void pickSimon() {
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  for(int i = 0;i < 30; i++) {
    if (i < pinCount) {
      digitalWrite(ledPins[i], LOW);
    }
    simon[i] = random(4);
  }
}

void playSequenceTo(int level) {
  for (int i = 0; i <= level; i++) {
      int pinId = simon[i];
      Serial.println("Position " + String(pos) + " = " + String(pinId));
      showMe(pinId);
  }
}

void playMelody(int melody[], int durations[]) {
  
}

void playNote(int note, int duration) {
  int noteDuration = 1000/duration;
  tone(8, note, noteDuration);
// to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int pauseBetweenNotes = noteDuration * 1.01;
  delay(pauseBetweenNotes);
  
  noTone(8);
}

int waitForButton() {
  while(true) {
    if (button1.uniquePress()) {
        return 0;
        break; // jump out of loop    
    }
    if (button2.uniquePress()) {
        return 1;
        break; // jump out of loop    
    }
    if (button3.uniquePress()) {
        return 2;
        break; // jump out of loop    
    }
    if (button4.uniquePress()) {
        return 3;
        break; // jump out of loop    
    }
  }
  return -1;
}
