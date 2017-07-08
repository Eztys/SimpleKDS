#include <SimpleKDS.h>

byte RPM_req[] = {0x80, 0x11, 0xF1, 0x02, 0x21, 0x09, 0xAE};   //ECU replies with OK: 80 F1 11 04 61 09 AA BB CS, RPM = AA*100 + BB

// Flag for ECU connection
bool ECUconnected = false;
// Variable that monitors the response of the "getResponse" function
byte resState;
// Buffer that holds the response message
const int resbufsize = 100;
byte resbuf[resbufsize];
// Variable to store the RPM in
int RPM;

SimpleKDS KDS(resbufsize);

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
  resState = BUSY;
  while (resState == BUSY) resState = KDS.getResponse(resbuf);


  if (resState == SUCCESS) {
    // Successfully received the response message which is stored in resbuf

    // Check if response is positive
    if (resbuf[4] == 0x61) {
      // RPM are calculated by 100*resbuf[6]+resbuf[7]
      RPM = 100 * resbuf[6] + resbuf[7];

      // Do something with RPM e.g. show on LCD, write to file on SD,
    } else {
      // No positive response, reset ECU connection
      ECUconnected = false;
    }

  } else {
    // Timeout, reinitiate ECU
    ECUconnected = false;
  }

}
