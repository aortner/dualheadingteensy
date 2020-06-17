
int laenge;
long timeofweek;
long lastTime = 0;
float baseline;
double baseline2;
float roll;
double relPosD;
double relPosDH;
int hcount = 0;
String s = "$GNHDT,123.456,T*";
byte XOR;
char c;
char b;
String t;
double heading=0;
double headingcorr = 90;



byte CK_A = 0, CK_B = 0;
byte incoming_char;
boolean headerReceived = false;
unsigned long ackWait = millis();
byte ackPacket[72] = {0xB5, 0x62, 0x01, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int i = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

void loop() {
   
  String input="";
  if (Serial1.available()>0) {     // If anything comes in Serial1 (pins 0 & 1)
     input = Serial1.readStringUntil('\n');
     
   Serial.println(input);   // read it and send it out Serial (USB)  
    }
    
 if (Serial.available()>0) {     // If anything comes in Serial1 (pins 0 & 1)
   Serial1.write(Serial.read());   // read it and send it out Serial (USB)
   
    }
  
  
  if (Serial2.available()>0) {
    incoming_char = Serial2.read();
    if (i < 4 && incoming_char == ackPacket[i]) {
      i++;
    }
    else if (i > 3) {
      ackPacket[i] = incoming_char;
      i++;
    }
  }
  if (i > 71) {
    checksum();
   
    i = 0;
  }
}

void checksum() {
  CK_A = 0;
  CK_B = 0;
  for (i = 2; i < 70 ; i++) {
    CK_A = CK_A + ackPacket[i];
    CK_B = CK_B + CK_A;
  }

  if (CK_A == ackPacket[70] && CK_B == ackPacket[71]) {
    // Serial.println("ACK Received! ");
     parsen();
  }
  else {
    // Serial.println("ACK Checksum Failure: ");
  }
  byte ackPacket[72] = {0xB5, 0x62, 0x01, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
}
