#include <Arduino.h>
#include "LedControl.h"
// #include <Ticker.h>
#include <avdweb_VirtualDelay.h>

#include "ntp.h"

// Ticker clockTicker;
VirtualDelay clockDelay2000;
VirtualDelay clockDelay500;
LedControl lc=LedControl(12,14,5,1);

byte Numbers[]={
// abcdefg
  B11111100, // 0 abcdef
  B01100000, // 1 bc
  B11011010, // 2 abdeg
  B11110010, // 3 abcdg
  B01100110, // 4 bcfg
  B10110110, // 5 acdfg
  B10111110, // 6 acdefg
  B11100000, // 7 abc
  B11111110, // 8 abcdefg
  B11110110, // 9 abcdfg
  B00000000, // 10 off
  B00000010, // 11 -
};

bool secondsDots = false;
bool isTemp = true;
byte clockLeds = B00000000;
int s = app_state.Temp;

void clockTick(){
  clockDelay500.start(500);  
  clockDelay2000.start(2000);
  
  

  if(clockDelay2000.elapsed()){
    if(isTemp){
      clockLeds = clockLeds | B00100000;
      s = app_state.Temp;
    }else{
      clockLeds = clockLeds & B11011111;
      s = app_state.Humid;
    }
    isTemp = !isTemp;
  }

  if(clockDelay500.elapsed()){  

    byte dots_on = B11000000;
    byte dots_off = B00111111;
    clockLeds = secondsDots ? clockLeds | dots_on : clockLeds & dots_off;
    secondsDots = !secondsDots;

    int h = timeClient.getHours();
    int m = timeClient.getMinutes();
    // int s = timeClient.getSeconds();
    
    lc.setColumn(0, 1, Numbers[h / 10]);
    lc.setColumn(0, 2, Numbers[h % 10]);
    lc.setColumn(0, 3, Numbers[m / 10]);
    lc.setColumn(0, 4, Numbers[m % 10]);
    lc.setColumn(0, 5, Numbers[s / 10]);
    lc.setColumn(0, 6, Numbers[s % 10]);
    lc.setColumn(0, 7, clockLeds);
  }

}

void lc_init(){
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  // lc.clearDisplay(0);
  // for(int i = 1; i <=4; i++){
  //   lc.setColumn(0, i, Numbers[0]);
  // }
  // for(int i = 5; i <=6; i++){
  //   lc.setColumn(0, i, Numbers[11]);
  // }
  // clockTicker.attach(1, clockTick);
}