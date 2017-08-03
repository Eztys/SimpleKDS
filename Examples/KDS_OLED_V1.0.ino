#include <SimpleKDS.h>
#include <SPI.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


byte temperature_req[] =   {0x80, 0x11, 0xF1, 0x02, 0x21, 0x07, 0xAC};
byte voltage_req[] =       {0x80, 0x11, 0xF1, 0x02, 0x21, 0x0A, 0xAF};
byte gear_req[] =          {0x80, 0x11, 0xF1, 0x02, 0x21, 0x0B, 0xB0};
byte operatingtime_req[] = {0x80, 0x11, 0xF1, 0x02, 0x21, 0x44, 0xE9};

bool ECUconnected = false;
const int resbufsize = 100;
byte resbuf[resbufsize];
byte resState;
int i=1;

float voltage;
float temperature;
int gear;
float operatingtime;

SimpleKDS KDS(resbufsize);

void setup() {
  u8g2.begin();
  KDS.setTiming(5, 0, 0, 55);
  delay(3000);
}

void loop() {

  if (!ECUconnected) {  
     
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_helvB14_tr);
      u8g2.drawStr(0,15,"CONNECTING");
      u8g2.setFont(u8g2_font_helvB08_tr);
      u8g2.drawStr(0,63,"ATTEMPT");
      u8g2.setCursor(63,63);
      u8g2.print(i);

    } while ( u8g2.nextPage() );
    i++;
    
    ECUconnected = KDS.initECU();
    
  } else {

    KDS.sendRequest(voltage_req, sizeof(voltage_req));
    resState = BUSY;
    while (resState == BUSY) resState = KDS.getResponse(resbuf);
    if(resState == SUCCESS) {
      voltage = (float) resbuf[6]/12.75;
    } else {
      ECUconnected = false;
    }

    if(ECUconnected){
      KDS.sendRequest(temperature_req, sizeof(temperature_req));
      resState = BUSY;
      while (resState == BUSY) resState = KDS.getResponse(resbuf);
      if(resState == SUCCESS) {
        temperature = (float) (resbuf[6]-48)/1.6;
      } else {
        ECUconnected = false;
      }
    }

    if(ECUconnected){
      KDS.sendRequest(gear_req, sizeof(gear_req));
      resState = BUSY;
      while (resState == BUSY) resState = KDS.getResponse(resbuf);
      if(resState == SUCCESS) {
        gear = resbuf[6];
      } else {
        ECUconnected = false;
      }
    }

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_helvB14_tr);
      u8g2.setCursor(0,15);
      u8g2.print(String(voltage,1)+" V");
      u8g2.setCursor(0,63);
      u8g2.print(String(temperature,1)+"  "+"C");
      u8g2.drawCircle(32,50,2);
      u8g2.setFont(u8g2_font_fub49_tn);
      u8g2.setCursor(80,57);
      u8g2.print(gear);    
      
    } while ( u8g2.nextPage() );
        
  }
}


