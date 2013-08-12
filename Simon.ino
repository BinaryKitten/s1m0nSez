//#include <Tone.h>

#include <Button.h>

#include "pitches.h"

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

int pinMelodies[5][2] = {
  {NOTE_B4, 8},
  {NOTE_C5, 8},
  {NOTE_A4, 8},
  {NOTE_E4, 8},
  {NOTE_G2, 4}
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
//        playMelody(funeral, funeralDurations);
        showError();
        pickSimon();
        playStartSound();
        pos = -1;
      }
      Serial.print(String(buttonPressed) + "  ");
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
  int note = pinMelodies[4][0];
  int duration = pinMelodies[4][1];
  
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
    playNote(note, duration);
  }
  
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
  int newPin = 1;
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

void playNote(int note, int duration) {
  int noteDuration = 1000/duration;
  tone(8, note, noteDuration);
// to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int pauseBetweenNotes = noteDuration * 1.01;
  delay(pauseBetweenNotes);
//  
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
