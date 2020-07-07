float abstand=1.00;  //abstand zwischen den antennen eingeben
float maxfehler=0.1; //wenn der fehler im anstand groesser als 10 cm ist wird keine neigung mehr berechnet
double headingcorr = 90;  //verschiebung der Headingantenne


// wenn die Headingantenne kein RTK Float hat wird auch keine Neigung berechnen
// ubx-nav-relposned - hier kann man mit u-center sehen was Sache ist
// zb abstand der Antenne, Heading und RTK Loesung

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
double heading = 0;


String rawbuffer = "";

byte CK_A = 0, CK_B = 0;
byte incoming_char;
boolean headerReceived = false;
unsigned long ackWait = millis();
byte ackPacket[72] = {0xB5, 0x62, 0x01, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int i = 0;

char nmea[120];
int cc = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

void loop() {



  if (Serial1.available() > 0) {   // If anything comes in Serial1 (pins 0 & 1)
    // Serial.write(Serial1.read());   // read it and send it out Serial (USB)
    char c;
    c = Serial1.read();

    nmea[cc] = c;
    cc++;
    if (c == '\n')
    {
     
      for (int i = 0; i <= (cc -1); i++)
      {
        Serial.write(nmea[i]);
      }
      cc = 0;
      //Serial.print("###");
     // Serial.println(nmea[cc-1],HEX);

    }


    if (cc > 120)cc = 0;
  }


  if (Serial.available() > 0) {   // If anything comes in Serial1 (pins 0 & 1)
    Serial1.write(Serial.read());   // read it and send it out Serial (USB)
  }


  if (Serial2.available() > 0) {
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
