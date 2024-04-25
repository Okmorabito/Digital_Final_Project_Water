#include <Adafruit_NeoPixel.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s1;                   //xy=295,339
AudioAnalyzePeak peak1;               //xy=452,300
AudioAnalyzeNoteFrequency notefreq1;  //xy=457,402
AudioOutputI2S i2s2;                  //xy=535,343
AudioConnection patchCord1(i2s1, 0, peak1, 0);
AudioConnection patchCord2(i2s1, 0, i2s2, 1);
AudioConnection patchCord3(i2s1, 0, i2s2, 0);
AudioConnection patchCord4(i2s1, 0, notefreq1, 0);
AudioControlSGTL5000 sgtl5000_1;  //xy=391,209
// GUItool: end automatically generated code


int solenoidPin = 36;
int waterPumpPin = 37;
int mappedPressure = 0;

void setup() {
  pinMode(solenoidPin, OUTPUT);
  AudioMemory(500);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  notefreq1.begin(.15);
}

void loop() {
  waterValve();
  waterPump();
}

void waterValve() {
  if (peak1.available()) {
    int peak = peak1.read() * 8;
    Serial.println(peak);
    if (peak > 5) {
      analogWrite(solenoidPin, HIGH);
    }
    if (peak < 5) {
      analogWrite(solenoidPin, LOW);
    }
  }
}

void waterPump() {
  if (notefreq1.available()) {
    float freq = notefreq1.read();
    Serial.println(freq);
    mappedPressure = map(freq, 440, 800, 0, 255);
  }
  for (int i = 0; i < 255; i++) {
    analogWrite(waterPumpPin, i); //iterates through power voltages for water pump 
    delay(10);
  }
}
//wire 2 mosfets to seperate the pump and valve
//make the pump's mosfet power changed based on the frequency, you have to map certain numbers in the analogWrite to make the appearence of change greater
//PWM (Pulse-Width Modulation) for water pump.   https://www.javatpoint.com/arduino-pwm

