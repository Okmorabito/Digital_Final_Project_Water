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
int constrainedFreq = 0;

void setup() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);
  AudioMemory(50);
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
  if (peak1.available()){
    int peak = peak1.read() * 10.0;
    //Serial.println(peak);
    if (peak > 5) {
      digitalWrite(solenoidPin, HIGH);
    } if (peak < 5){
      digitalWrite(solenoidPin, LOW);
    }
  }
}

void waterPump() {
  if (notefreq1.available()) {
    int freq = notefreq1.read();
    int constrainedFreq = constrain(freq, 250, 530);    
    Serial.println(constrainedFreq);
    mappedPressure = map(constrainedFreq, 250, 530, 0, 255);
    if (250 < constrainedFreq < 530) {
      analogWrite(waterPumpPin, mappedPressure);
    }
    if (250 > constrainedFreq < 530) {
      analogWrite(waterPumpPin, 0);
    }
  }
}
