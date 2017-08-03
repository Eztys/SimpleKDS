#include <SimpleKDS.h>  // Include KDS library 
#include <SPI.h>        //
#include <Arduino.h>    // Include these three libraries for commucation, processing and display of information on the OLED
#include <U8g2lib.h>    //

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Fro the U8G2lib library, pick the line that matches your OLED display
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// The request byte sequence for the coolant temperature, battery voltage, current gear and operating time
byte temperature_req[] =   {0x80, 0x11, 0xF1, 0x02, 0x21, 0x07, 0xAC};
byte voltage_req[] =       {0x80, 0x11, 0xF1, 0x02, 0x21, 0x0A, 0xAF};
byte gear_req[] =          {0x80, 0x11, 0xF1, 0x02, 0x21, 0x0B, 0xB0};
byte operatingtime_req[] = {0x80, 0x11, 0xF1, 0x02, 0x21, 0x44, 0xE9};

bool ECUconnected = false;    // Initially the ECU is not connected (reset flag)
const int resbufsize = 100;   // 100 bytes is sufficient to hold all responses
byte resbuf[resbufsize];      // Allocate the byte array in memory
byte resState;                // Initialize the state for communication
int i=1;                      // Counter for number of connection attempts

float voltage;        // Float for the battery voltage (in Volts)
float temperature;    // Float for the temperature (in degrees Celsius)
int gear;             // Integer for the gear
float operatingtime;  // Float for the operating time

SimpleKDS KDS(resbufsize);  // Initialize an object of the KDS class

// Setup 
void setup() {
  u8g2.begin(); // Start communication with the OLED
  KDS.setTiming(5, 0, 0, 55); // Set the timing parameters for KDS communication
  delay(3000);  // Short delay that allows the motorcycle to settle
}

// Infinite loop
void loop() {
  
  // Continuously try to initialize communication with the ECu and show the number of attempts on the screen
  if (!ECUconnected) {  
    
    // Draw the text on the screen
    u8g2.firstPage();
    do {
      
      u8g2.setFont(u8g2_font_helvB14_tr);
      u8g2.drawStr(0,15,"CONNECTING");
      u8g2.setFont(u8g2_font_helvB08_tr);
      u8g2.drawStr(0,63,"ATTEMPT");
      u8g2.setCursor(63,63);
      u8g2.print(i);

    } while ( u8g2.nextPage() );
    i++;  // Increase counter
    
    ECUconnected = KDS.initECU(); // Attempt to initialize the ECU (usually one attempt is sufficient)
  
    
  // If the communication is OK, request and parse the desired values from the ECU  
  } else {
    
    // Request battery voltage
    KDS.sendRequest(voltage_req, sizeof(voltage_req));
    resState = BUSY;
    while (resState == BUSY) resState = KDS.getResponse(resbuf);
    if(resState == SUCCESS) {
      // Request completely received, convert the data byte to a voltage
      voltage = (float) resbuf[6]/12.75;
    } else {
      // Something went wrong, reinitialize the ECU
      ECUconnected = false;
    }
    
    // Request coolant temperature
    if(ECUconnected){
      KDS.sendRequest(temperature_req, sizeof(temperature_req));
      resState = BUSY;
      while (resState == BUSY) resState = KDS.getResponse(resbuf);
      if(resState == SUCCESS) {
        // Request completely received, convert the data byte to a temperature
        temperature = (float) (resbuf[6]-48)/1.6;
      } else {
        // Something went wrong, reinitialize the ECU
        ECUconnected = false;
      }
    }

    // Request gear
    if(ECUconnected){
      KDS.sendRequest(gear_req, sizeof(gear_req));
      resState = BUSY;
      while (resState == BUSY) resState = KDS.getResponse(resbuf);
      if(resState == SUCCESS) {
        // Request completely received, convert the data byte to gear
        gear = resbuf[6];
      } else {
        // Something went wrong, reinitialize the ECU
        ECUconnected = false;
      }
    }

    // All data receive at this point, update OLED display
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
        
    // Finished updating OLED 
    // Go to next itteration
  }
}


