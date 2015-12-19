/* 
now using DTR flow control
*/

boolean local_echo = true;
boolean wait_echo = true;

int DSR_pin = 3;
int DTR_pin = 2; 

boolean waiting = false;

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);

  // RTS
  pinMode(DSR_pin, OUTPUT);
  
  // CTS 
  pinMode(DTR_pin, INPUT);

  // set RTS
  digitalWrite(DSR_pin, HIGH);
}

void loop() {
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte); 
  }
  
    // read from port 0, send to port 1:
  if (Serial.available()) {
    Serial1.flush();
    if (digitalRead(DTR_pin)==LOW) {
      int inByte = Serial.read();
      Serial1.write(inByte);
      if (local_echo && Serial) Serial.write(inByte);
      waiting = false;
    } else {
      if (wait_echo && !waiting && Serial) Serial.write("<---WAIT--->");
      waiting = true;
    }
  } 
}
