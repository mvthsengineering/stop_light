//The purpose of this sketch is to have a way to test the lights
//Artificial time to test the whole schedule


#include "RTClib.h"
#define GREEN 4
#define YELLOW 3
#define RED 2
#define WARNING_TIME 5
#define LIGHT_ON  0
#define LIGHT_OFF 1
#define TEST_TIME 500
RTC_DS1307 rtc;

int currentPeriod = 0;
int period = 0;
int nowTime = 466;

// Starting/end times of the period
// Each Block signifies a period
int schReg[16][2] {
  // Block 0 (Advisory)
  {7, 45},  //465 
  {7, 59},  //469 
  // Block 1 (Period 1)
  {7, 52},  //472
  {8, 48},  //528
  // Block 2 (Period 2)
  {8, 51},  //531
  {9, 47},  //587
  // Block 3 (Period 3)
  {9, 50},  //590
  {10, 46}, //640
  // Block 4 (Period 4)
  {10, 51}, //651
  {11, 47}, //707
  // Block 5 (Lunch)
  {11, 50}, //710
  {12, 18}, //738
  // Block 6 (Period 5)
  {12, 23}, //743
  {13, 19}, //799
  // Block 7 (Period 6)
  {13, 22}, //802
  {14, 18}, //858
};



// Converts the hours and minutes of the array into # of minutes
int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};

void setup() {
  //int conv_time;
  int i;
  Serial.begin(9600);

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }
  /*
    DateTime now = rtc.now();
    int hrs = now.minute();
    int mins = now.second();
    //int secs = now.second();
    convert_time(hrs, mins);
    // int nowTime = conv_time;
  */

  // Gets initial block
  for ( i = 0; i < 16; i += 2) {
    int startPeriod = convert_time(schReg[i][0], schReg[i][1]);
    int nextPeriod = convert_time(schReg[i + 2][0], schReg[i + 2][1]);

    if (nowTime >= startPeriod && nowTime <= nextPeriod) {
      break;
    }
  }
  period = i ;
  Serial.print("period=");
  Serial.println(period);
  //while(1);
}

void loop() {
  nowTime += 1;
  //int conv_time;

  /*
    DateTime now = rtc.now();
    int hrs = now.hour();
    int mins = now.minute();
    int secs = now.second();
    int nowTime = convert_time(hrs, mins);
  */




  // Gets time values for current period

  int startPeriod = convert_time(schReg[period][0], schReg[period][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[period + 1][0], schReg[period + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[period + 2][0], schReg[period + 2][1]);  //red is on between endPeriod and nextPeriod
  // Serial.print("nowTime: ");
  // Serial.println(testTime);


  Serial.print("nowTime: ");
  Serial.print(nowTime);
  //Serial.println(secs);
  Serial.print(" period = ");
  Serial.print(period);
  /*
  Serial.print("startPeriod: ");
  Serial.println(startPeriod);
  Serial.print("endPeriod: ");
  Serial.println(endPeriod);
  Serial.print("nextPeriod: ");
  Serial.println(nextPeriod);
  Serial.println("________________________");



  /*
    Serial.print("block: ");
    Serial.println(period);

    Serial.print("hrs:");
    Serial.println(hrs);
    Serial.print("mins:");
    Serial.println(mins);
    Serial.print("secs:");
    Serial.println(secs);
    Serial.println("-------------------");
  
  //Serial.println();
*/
  // In class time
  if (nowTime <= endPeriod - WARNING_TIME) {
    digitalWrite(GREEN, LIGHT_ON);
    digitalWrite(YELLOW, LIGHT_OFF);
    digitalWrite(RED, LIGHT_OFF);
    Serial.println(" GREEN ON");

    // Five minutes before period ends
  } else if (nowTime < endPeriod) {
    digitalWrite(YELLOW, LIGHT_ON);
    digitalWrite(GREEN, LIGHT_OFF);
    digitalWrite(RED, LIGHT_OFF);
    Serial.println(" YELLOW ON");

    // Passing time
  } else if (nowTime >= endPeriod && nowTime <= nextPeriod) {
    digitalWrite(RED, LIGHT_ON);
    digitalWrite(YELLOW, LIGHT_OFF);

    // Cycle the light testing and go to next period
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED, LIGHT_OFF);
    digitalWrite(GREEN, LIGHT_ON);
    Serial.println(" RED ON");
    period += 2;
  }
  
/*
  // Function to turn off the lights when theres no school
  if (nowTime < 465 || nowTime > 858) {
    digitalWrite(GREEN, LIGHT_OFF);
    digitalWrite(YELLOW, LIGHT_OFF);
    digitalWrite(RED, LIGHT_OFF);
  }

  //Turn only red light on during lunch time
  if ((nowTime >= convert_time(schReg[10][0], schReg[10][1])) && nowTime <= convert_time(schReg[11][0], schReg[11][1])) {
    digitalWrite(GREEN, LIGHT_OFF);
    digitalWrite(YELLOW, LIGHT_OFF);
    digitalWrite(RED, LIGHT_ON);
  }
  //reset timer
  if (nowTime >= 860) {
    nowTime = 460;
  }
  */
  delay(TEST_TIME);
}
