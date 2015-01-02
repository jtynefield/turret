#include <ax12.h>
#include <BioloidController.h>

#define BAUDRATE 9600
#define PAN 1
#define TILT 2

#define TILT_MIN 256
#define TILT_MAX 768
#define TILT_RANGE (TILT_MAX-TILT_MIN)

BioloidController turret(1000000); 

void setup() {
  delay(1000); // apparently controller needs some settle time
  Serial.begin(BAUDRATE);
  
  turret.poseSize = 2;
  turret.readPose();
  turret.setNextPose(PAN, 512);
  turret.setNextPose(TILT, 512);
  turret.interpolateSetup(1000);
  while( turret.interpolating > 0 ) {
    turret.interpolateStep();
    delay(3);
  }
}

// 1-9 map to tilt
// a-z map to pan
// esc to quit

void loop() {
  if ( Serial.available() > 0 ) {
     unsigned char inByte = Serial.read();
     
      if ( inByte >= 'a' && inByte <= 'z' ) {
          int pan = inByte - 'a';
          pan *= 1024;
          pan /= 'z' - 'a';
          pan = (pan>1023)?1023:pan;
          SetPosition(PAN,pan);
      }

      if ( inByte >= '1' && inByte <= '9' ) {
          int tilt = inByte - '1';
          tilt *= TILT_RANGE;
          tilt /= '9' - '1';
          tilt += TILT_MIN;
          tilt = (tilt>TILT_MAX)?TILT_MAX:tilt;
          tilt = (tilt<TILT_MIN)?TILT_MIN:tilt;
          SetPosition(TILT,tilt);
      }
  }
}
  
  
