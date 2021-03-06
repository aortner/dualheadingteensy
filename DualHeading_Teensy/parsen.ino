


void parsen() {

  int carrSoln;
  bool gnssFixOk, diffSoln, relPosValid, isMoving, refPosMiss, refObsMiss ;
  bool refPosHeadingValid, relPosNormalized;
  double roll;
  heading  =  (long)ackPacket[24 + 6] ;
  heading += (long)ackPacket[25 + 6] << 8;
  heading += (long)ackPacket[26 + 6] << 16 ;
  heading += (long)ackPacket[27 + 6] << 24 ;
  heading = heading / 100000;

  heading = heading + headingcorr;
  if (heading >= 360) heading -= 360;
  if (heading < 0) heading += 360;


 //Serial.print("LastHeading: ");
   // Serial.println(heading);

if(heading>0)
{
  
  for (int i = COMPASS_VALUES; i >= 1; i--)
  {
    compass[i] = compass[i - 1];
   // Serial.print(i);
  //Serial.print(" ");
  //  Serial.println(compass[i]);
  }
  compass[0]=heading*100;
}
else
{
//  Serial.println("Kein Heading");
}


// calculate average course derivation
kurs = compass[0];

d_sum = 0;
for (i=1; i<COMPASS_VALUES; i++)
{
  d = compass[i] - kurs;
  if (d >  18000) d -= 36000;
  if (d < -18000) d += 36000;
  d_sum += d;
 

  
}

d_sum  += COMPASS_VALUES / 2;  // round (add 0.5)
d_sum  /= (COMPASS_VALUES);
kurs += d_sum;

// normalize
if (kurs >= 36000) kurs -= 36000;
if (kurs <     0) kurs += 36000;


heading=kurs;
heading /=100;

  //  Serial.print("Kurs: ");
 //   Serial.println(heading);






  baseline  =  (long)ackPacket[20 + 6] ;
  baseline += (long)ackPacket[21 + 6] << 8;
  baseline += (long)ackPacket[22 + 6] << 16 ;
  baseline += (long)ackPacket[23 + 6] << 24 ;
  baseline = baseline / 100;
  baseline2 = (long)ackPacket[35 + 6];
  baseline2 =   baseline2 / 10000;
  baseline = baseline + baseline2;

  relPosD  =  (long)ackPacket[16 + 6] ;
  relPosD += (long)ackPacket[17 + 6] << 8;
  relPosD += (long)ackPacket[18 + 6] << 16 ;
  relPosD += (long)ackPacket[19 + 6] << 24 ;
  relPosD = relPosD / 100;
  relPosDH = (long)ackPacket[34 + 6];
  relPosDH = relPosDH / 100000;
  relPosD = relPosD + relPosDH;

  uint32_t flags = ackPacket[60 + 6];

  //  Serial.println(flags, BIN);

  gnssFixOk = flags & (1 << 0);
  diffSoln = flags & (1 << 1);
  relPosValid = flags & (1 << 2);
  carrSoln = (flags & (0b11 << 3)) >> 3;
  isMoving = flags & (1 << 5);
  refPosMiss = flags & (1 << 6);
  refObsMiss = flags & (1 << 7);
  refPosHeadingValid = flags & (1 << 8);
  relPosNormalized = flags & (1 << 9);

  /*

      Serial.print("gnssFixOk : ");
      Serial.println(gnssFixOk);
      Serial.print("diffSoln : ");
      Serial.println(diffSoln);
      Serial.print("relPosValid : ");
      Serial.println(relPosValid);
      Serial.print("carrSoln : ");
      Serial.println(carrSoln);

      Serial.print("isMoving : ");
      Serial.println(isMoving);
      Serial.print("refPosMiss : ");
      Serial.println(refPosMiss);
      Serial.print("refObsMiss : ");
      Serial.println(refObsMiss);

      Serial.print("refPosHeadingValid : ");
      Serial.println(refPosHeadingValid);
      Serial.print("relPosNormalized: ");
      Serial.println(relPosNormalized);
  */


  if (gnssFixOk && diffSoln && relPosValid && isMoving)
  {
     //Serial.println("Alles OK! ");
  }
  else
  {
    // Serial.println("Fehler! ");
    return;
  }
  s = "$GNHDT,";
  s = s + heading + ",T*";

  for (XOR = 0, i = 0; i < s.length(); i++) {
    c = (unsigned char)s.charAt(i);
    if (c == '*') break;
    if ((c != '$') && (c != '!')) XOR ^= c;
  }
  // t = String(XOR, HEX);



  Serial.print(s);
  if (XOR < 16) Serial.print("0"); // add leading 0 if needed
  Serial.println(XOR, HEX);


double p = sqrt(baseline*baseline-relPosD*relPosD);

  

  if (carrSoln == 2) {
   // roll = (atan2(relPosD, baseline)) * 180 / 3.141592653589793238;
    roll = (atan(relPosD/p)) * 180 / 3.141592653589793238;
    roll *= -1;
      }
  else roll = 0;

  if (baseline > abstand + maxfehler || baseline < abstand - maxfehler)
  {
    roll = 0;
    Serial.print("Baseline fehler :  " );
    Serial.println(baseline);

  }




  //$PTNL,AVR,181059.6,+149.4688,Yaw,+0.0134,Tilt,,,60.191,3,2.5,6*00
  s = "$PTNL,AVR,181059.6,+149.4688,Yaw,";
  s = s + roll + ",Tilt,,,60.191,3,2.5,6*";

  for (XOR = 0, i = 0; i < s.length(); i++) {
    c = (unsigned char)s.charAt(i);
    if (c == '*') break;
    if ((c != '$') && (c != '!')) XOR ^= c;
  }
  // t = String(XOR, HEX);

  Serial.print(s);
  if (XOR < 16) Serial.print("0"); // add leading 0 if needed
  Serial.println(XOR, HEX);



}
