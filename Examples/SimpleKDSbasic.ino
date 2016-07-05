#include <SimpleKDS.h>

byte RPM_req[] = {0x80, 0x11, 0xF1, 0x02, 0x21, 0x09, 0xAE};   //ECU replies with OK: 80 F1 11 04 61 09 AA BB CS, RPM = AA*100 + BB

// Flag for ECU connection
bool ECUconnected = false;
// Variable that monitors the response of the "getResponse" function
byte resState;
// Buffer that holds the response message
byte resbuf[20];
// Variable to store the RPM in
int RPM;

SimpleKDS KDS;

void setup() {
  // Default timing according to ISO-14230
  KDS.setTiming(5, 10, 25, 55);
  // Wait for 2 seconds to allow the motorcycle to initiate
  delay(2000);
}

void loop() {

  // Try to connect with the ECU
  while (!ECUconnected) {
    ECUconnected = KDS.initECU();
  }

  // Request engine RPM
  KDS.sendRequest(RPM_req, sizeof(RPM_req));

  // Receive the response. Receiving this way will block the Arduino.
  resState = RES_BUSY;
  while (resState == RES_BUSY) resState = KDS.getResponse(resbuf);


  if (resState == RES_SUCCESS) {
    // Successfully received the response message which is stored in resbuf
    // RPM are calculated by 100*resbuf[6]+resbuf[7]

    RPM = 100 * resbuf[6] + resbuf[7];

    // Do something with RPM e.g. show on LCD, write to file on SD,

  } else {
    // Timeout, reinitiate ECU
    ECUconnected = false;
  }

}




