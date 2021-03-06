#include "SimpleKDS.h"
 
SimpleKDS::SimpleKDS(int resbufsize){
	K_IN = 0;				// RX pin
	K_OUT = 1;				// TX pin
	BAUDRATE = 10400;		// KDS baudrate
	responseTimeout = 5000;			// Maximum timeout for response
	ECUAddress = 0x11;
	sourceAddress = 0xF1;
	responseBufferSize = resbufsize;	// Maximum buffer size to prevent buffer overflow

	interByteReqDelay = 5;  // 5
	interByteResDelay = 0;  // 10
	interReqResDelay = 0;  	// 25
	interResReqDelay = 55;  // 55
	
	Serial.begin(BAUDRATE);
		
}

SimpleKDS::~SimpleKDS(){
	Serial.end();
}

void SimpleKDS::setTiming(byte P1, byte P2, byte P3, byte P4){
	interByteReqDelay = P1;   
	interByteResDelay = P2;   
	interReqResDelay = P3;  	
	interResReqDelay = P4;   
}

void SimpleKDS::sendRequest(byte *reqbuf, byte len) {

	for (byte i = 0; i < len; i++) {
		Serial.write(reqbuf[i]);
		delay(interByteReqDelay);
	}
	
}

void SimpleKDS::setAddresses(byte ECU, byte source) {
	
	ECUAddress = ECU;
	sourceAddress = source;
	
}

void SimpleKDS::setTimeout(int t) {
	responseTimeout = t;
}


byte SimpleKDS::getResponse(byte *rbuffer) {
	// These bytes remain unchanged in between function calls
	// Used to monitor the response while not blocking the program
	static int index = 0;
	static int len = 255;
	static unsigned long t = 0;

	// After a request is send the timer is set
	if (t == 0) t = millis();

	// Check if function call is within timeout limit
	if (millis() - t < responseTimeout) {
		
		// Check if more bytes need to be received
		if (index < len) {
			
			// Check if bytes are available
			while (Serial.available() > 0) {
				// Reset timer
				t=0;
				
				// Read one byte
				rbuffer[index] = Serial.read();
				
				// First received byte must be correct
				if ((rbuffer[0] == 0x80) || (rbuffer[0] == 0x81)) {

					// Check if received message is echo or response
					if (index == 1) {
						// Read own request, reset index & timer (start over)          
						if (rbuffer[1] != sourceAddress) {
							// Check if next byte could be beginning of new request
							if((rbuffer[1] == 0x80) || (rbuffer[1] == 0x81)) {
								// Discard only first byte i.e. shift 1 byte to the left
								rbuffer[0] = rbuffer[1];		
								// Next byte should be index=1 
								index = 1;								
							} else {
								index = 0;					            
							}
							return BUSY;
							
						}
					// Set the length of the response
					} else if (index == 3) {		
						if (rbuffer[0] == 0x80) {
							len = rbuffer[3] + 5;
							
							// Check if response buffer is large enough
							if (len > responseBufferSize) {
								len = 255;
								index = 0;
								return BUFFEROVERFLOW;
							}
						} else {
							len = 5;
						}
						
					}
					
					// Increase index to read next byte
					index++;					
				}
				
			}
			
			// Read correct byte move on to next byte
			return BUSY;                             
			
		// Read all bytes, reset timer, index and length  
		} else {
			t = 0;
			index = 0;
			len = 255;
			delay(interResReqDelay);
			return SUCCESS;                      
		}
		
	// Timeout, reset timer, index and length         
	} else {
		t = 0;
		index = 0;
		len = 255;
		return TIMEOUT;                         
	}
}

bool SimpleKDS::initECU(){
	
	byte start_communication[] = {0x81, ECUAddress, sourceAddress, 0x81, 0x00};		
	start_communication[4] = start_communication[0]+start_communication[1]+start_communication[2]+start_communication[3];
	
	byte start_diagnostic_mode[] = {0x80, ECUAddress, sourceAddress, 0x02, 0x10, 0x80, 0x0};  
	start_diagnostic_mode[6] = start_diagnostic_mode[0]+start_diagnostic_mode[1]+start_diagnostic_mode[2]+start_diagnostic_mode[3]+start_diagnostic_mode[4]+start_diagnostic_mode[5];
	
	byte resbuf[10];	
	byte resState;
	
	// Clear serial RX buffer
	while(Serial.available()) Serial.read();
	
	// End Serial Port
	Serial.end(); 

	// Start fast init sequence
	pinMode(K_OUT, OUTPUT);                       
	digitalWrite(K_OUT, HIGH);
	// Idle communication to create correct initial conditions
	delay(350);                               	  
	digitalWrite(K_OUT, LOW);                     
	delay(25);
	digitalWrite(K_OUT, HIGH);
	delay(25);

	// Start serial communication with ECU
	Serial.begin(BAUDRATE);

	// Send start communication request
	sendRequest(start_communication, sizeof(start_communication));

	// Read response
	resState = BUSY;
	while (resState == BUSY) resState = getResponse(resbuf);
	
	// Return false if start communication request was denied or failed
	if (resState != SUCCESS) return false;	
	if(resbuf[4] != byte(start_communication[3] + 0x40)) return false;
	
	// Send start diagnostic mode request
	sendRequest(start_diagnostic_mode, sizeof(start_diagnostic_mode));            	
	
	// Read response
	resState = BUSY;
	while (resState == BUSY) resState = getResponse(resbuf);
	
	// Return false if diagnostic mode request was denied or failed
	if (resState != SUCCESS) return false;
	if(resbuf[4] != byte(start_diagnostic_mode[4] + 0x40)) return false;
	
	// Initialization successful
	return true;	
}

