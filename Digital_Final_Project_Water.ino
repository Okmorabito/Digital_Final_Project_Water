#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//Auto-generated code to enable the Teensy to observe the peak and frequency functions

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


//set variables that are used to control the pump and valve
int solenoidPin = 36;
int waterPumpPin = 37;
int mappedPressure = 0;
int constrainedFreq = 0;

void setup() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT); //this program sends messages to the external devices so I created them as OUTPUTs
  AudioMemory(50);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC); //Setup code needed for the teensy's audio shield to run properly
  notefreq1.begin(.15);
}

void loop() {
  waterValve(); //both functions run endlessly inside the loop
  waterPump();
}

void waterValve(){
  if (notefreq1.available()) {
    int freq = notefreq1.read();
    if (freq > 500) {
      digitalWrite(solenoidPin, HIGH);
    }
    if (freq < 500) {
      digitalWrite(solenoidPin, LOW);
    }
  }// if the frequency is above 500 Hz, the valve opens and allows water to flow through
}// if the frequency is below 500 Hz, the valve remains closed, preventing the flow of water

void waterPump(){
  if (notefreq1.available()) {
    int freq = notefreq1.read();
    if (freq > 500) {
      digitalWrite(waterPumpPin, LOW);
    }
    if (freq < 500) {
      digitalWrite(waterPumpPin, HIGH);
    } // if the frequency is above 500 Hz, the pump will remain off
  } // if the frequency is below 500 Hz, the pump will turn on and start to spurt water out into the top basin
}

// These other functions are what my project would have used if I was able to get my full vision of the project up and running

// void waterValve() {
//   if (peak1.available()){
//     int peak = peak1.read() * 10.0;
//     //Serial.println(peak);
//     if (peak > 5) {
//       digitalWrite(solenoidPin, HIGH);
//     } if (peak < 5){
//       digitalWrite(solenoidPin, LOW);
//     }
//   } //if peak was above 5, then the valve would open allowing the flow of water
// } // if peak was below 5, the valve would remain closed

// void waterPump() {
//   if (notefreq1.available()) {
//     int freq = notefreq1.read();
//     int constrainedFreq = constrain(freq, 250, 530); // the constrain() function is used here to make sure the pump only turns on when playing the C4 major scale
//     Serial.println(constrainedFreq);
//     mappedPressure = map(constrainedFreq, 250, 530, 0, 255); // I map the frequency to allow me to PWM(Pulse width modulate) the pump which would control the height of the water stream
//     if (250 < constrainedFreq < 530) {
//       analogWrite(waterPumpPin, mappedPressure);
//     }
//     if (250 > constrainedFreq < 530) {
//       analogWrite(waterPumpPin, 0); // if the frequency was outside of the C4 major scale, the pump would shut off since I only want to analyze those frequencies
//     }
//   }
// }
