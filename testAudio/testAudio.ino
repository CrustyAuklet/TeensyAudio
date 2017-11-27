#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=146,292
AudioOutputI2S           i2s1;           //xy=383,291
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=152,360
// GUItool: end automatically generated code

// Use these with the audio adaptor board
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

/*** GLOBAL VARIABLES ***/
char tmpStr[30];          // temporary string for holding serial output
float vol = .8;           // Volume setting ( range is 0 to 1, distortion when > .8 )
short soundNum = 0;       // The number of the sound to play, set in ISR by dip-switches

void setup() {
    Serial.begin(9600);
    
    // Setup the audio
    Serial.println("Starting Audio Controls...");
    AudioMemory(8);
    sgtl5000_1.enable();
    sgtl5000_1.volume(vol);
  
    // Make sure that SD card is inserted
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
        // stop here, but print a message repetitively
        while (1) {
            Serial.println("Unable to access the SD card!");
            delay(500);
        }
    }
    Serial.println("SD Card detected...");
    
} // END OF SETUP

void loop() {
    if(!playSdWav1.isPlaying()) {
        playFile(soundNum);
        soundNum = (soundNum == 10 ? 0 : soundNum + 1);
    }
}

// Playfile function from Teensy Example
// plays numerically names wave files from SD card
void playFile(const short fileNum)
{
  sprintf(tmpStr, "%02d.WAV", fileNum);
  Serial.print("Playing file: ");
  Serial.println(tmpStr);

  // sketch continues to run while the file plays.
  playSdWav1.play(tmpStr);

  // A brief delay for the library read WAV info
  delay(5);
}

void setVolume() {
    vol = (analogRead(15) / 1024.0) * .8;
    sgtl5000_1.volume(vol);
}

