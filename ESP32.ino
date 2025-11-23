#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

//Declare variables
//Sets the state that determines which loop to run in the read function
byte state = 0;
//Gets the gear of the vehichle from gear and is used to verify if the gear has changed for updating display
int gearcheck;
//This variable never gets updated, it's just a placeholder for checking the rpms to set the state of the screen (redline or no)
int rpmcheck = 0;
//gear variable gets read in as an int then gearstate is set to whatever that is
char gearstate[11] = {'R', 'N', '1', '2', '3', '4', '5', '6', '7', '8', '\0'};

//Initialize TFT display, set the rotation of the screen and turn on serial communication
void setup(void) {
  tft.init();
  tft.setRotation(1);
  Serial.begin(115200);
}

void read() 
{ 
  if(Serial.available()) {
    
    int gear = Serial.readStringUntil(';').toInt();
    int rpms = Serial.readStringUntil(';').toInt();
    int maxrpm = Serial.readStringUntil(';').toInt();
    
    float perctflt = ((float)rpms/(float)maxrpm)*100;
    int perct = round(perctflt);

    if (gearcheck != gear) {
      state = 0;
    }

    if (gearcheck = gear) {
      if (rpmcheck != rpms) {

        if (perct <= 89 && state > 1){
        state = 0;
        }
        if (perct >= 90 && state < 2){
        state = 2;
        }

      }
    }

    gearcheck = gear;
    

    if (perct <= 89) {
      switch (state) {
        case 0:
          gear = Serial.readStringUntil(';').toInt();
          rpms = Serial.readStringUntil(';').toInt();
          maxrpm = Serial.readStringUntil(';').toInt();
          tft.fillScreen(TFT_BLACK);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          tft.setTextSize(7);
          tft.drawChar(gearstate[gear], 120, 50, 4);
          tft.setTextSize(2);
          tft.drawNumber(rpms, 260, 5, 1);
          state = 1;
          break;
        case 1:
          gear = Serial.readStringUntil(';').toInt();
          rpms = Serial.readStringUntil(';').toInt();
          maxrpm = Serial.readStringUntil(';').toInt();
          tft.drawNumber(rpms, 260, 5, 1);
          if (perct > 89){
            state = 2;
          }
          break;
      }
    }
    
    if (perct >= 90) {
      switch (state) {
        case 2:
          gear = Serial.readStringUntil(';').toInt();
          rpms = Serial.readStringUntil(';').toInt();
          maxrpm = Serial.readStringUntil(';').toInt();
          tft.fillScreen(TFT_RED);
          tft.setTextColor(TFT_BLACK, TFT_RED);
          tft.setTextSize(7);
          tft.drawChar(gearstate[gear], 120, 50, 4);
          tft.setTextSize(2);
          tft.drawNumber(rpms, 260, 5, 1);
          state = 3;
          break;
        case 3:
          gear = Serial.readStringUntil(';').toInt();
          rpms = Serial.readStringUntil(';').toInt();
          maxrpm = Serial.readStringUntil(';').toInt();
          tft.drawNumber(rpms, 260, 5, 1);
          if (perct < 90){
            state = 0;
          }
          break;
      }
    }    
  }
}

void loop(){
  read();
}

void bootscreen() {




  }

