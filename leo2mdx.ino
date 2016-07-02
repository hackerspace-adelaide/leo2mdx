/* 
now using DTR flow control
*/

boolean local_echo = false;
boolean wait_echo = false;

int DSR_pin = 3;
int DTR_pin = 2; 
int led_pin = 13;

boolean waiting = false;

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);

  // RTS
  pinMode(DSR_pin, OUTPUT);
  
  // CTS 
  pinMode(DTR_pin, INPUT);

  pinMode(led_pin, OUTPUT);

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
    //flushing here might send a char after DTR change
    //but we checked DTR at the last read so it should be ok
    //Serial1.flush();
    if (digitalRead(DTR_pin)==LOW) {
      digitalWrite(led_pin,LOW);
      int inByte = Serial.read();
      Serial1.write(inByte);
      if (local_echo && Serial) Serial.write(inByte);
      Serial1.flush();
      waiting = false;
    } else {
      // DTR high
      digitalWrite(led_pin,HIGH);
      if (wait_echo && !waiting && Serial) Serial.write("<---WAIT--->");
      waiting = true;
    }
  } 
}
