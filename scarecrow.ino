
#include <Wire.h>
#include <RTC.h>

// you need to install https://github.com/cvmanjoo/RTC for the rtc clock
// PCF8563 needs to be connected to SDA and SCL pins
// The buzzer must be connected via a tranzistor to the build in led pin

// how much will it ring?
#define ON_TIME_MIN 10
#define ON_TIME_MAX 30

// how much between 2 consecutive rings
#define OFF_TIME_MIN 20
#define OFF_TIME_MAX 40

// when to start beeping
#define START_HOUR 7
#define START_MINUTE 0

// when to stop beeping
#define END_HOUR 21
#define END_MINUTE 30

int hour, minute, onTime, offTime, startTime, endTime;
int beeper = LED_BUILTIN;

static PCF8563 RTC;

void setup_clock() {
  //  setup rtc pcf8563
  RTC.begin();

  // uncomment to set the clock!
//    RTC.stopClock();

  Serial.print("Is Clock Running: ");
  if (RTC.isRunning())
  {
    Serial.println("Yes");
    Serial.print(RTC.getDay());
    Serial.print("-");
    Serial.print(RTC.getMonth());
    Serial.print("-");
    Serial.print(RTC.getYear());
    Serial.print(" ");
    Serial.print(RTC.getHours());
    Serial.print(":");
    Serial.print(RTC.getMinutes());
    Serial.print(":");
    Serial.print(RTC.getSeconds());
    Serial.print("");

    Serial.println("");
    delay(1000);
  }
  else
  {
    delay(1500);

    Serial.println("No");
    Serial.println("Setting Time");
    RTC.setDateTime(__DATE__, __TIME__);
    Serial.println("New Time Set");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);
    RTC.startClock();
  }
}

void setup()
{
  pinMode(beeper, OUTPUT);
  Serial.begin(9600);
  setup_clock();
  startTime = START_HOUR * 60 + START_MINUTE;
  endTime = END_HOUR * 60 + END_MINUTE;
}

char print_buffer[100];
int timeInMinutes;

// trick: x << 10 is the same with x*1024
void loop()
{
  hour = RTC.getHours();
  minute = RTC.getMinutes();
  timeInMinutes = hour * 60 + minute;

  if (startTime < timeInMinutes && timeInMinutes < endTime) {
    sprintf(print_buffer, "SOUNDING THE ALARM. Time is %d. START time is %d (%d:%d). END time is %d (%d:%d).", timeInMinutes, startTime, START_HOUR, START_MINUTE, endTime, END_HOUR, END_MINUTE );
    Serial.println(print_buffer);

    //  turn on the beeper a random interval between ON_TIME_MIN and ON_TIME_MAX seconds
    onTime = random(ON_TIME_MIN, ON_TIME_MAX);

    sprintf(print_buffer, "Turning alarm ON for %d. ON_TIME_MIN=%d ON_TIME_MAX=%d", onTime, ON_TIME_MIN, ON_TIME_MAX);
    Serial.println(print_buffer);

    digitalWrite(beeper, HIGH);
    delay(onTime << 10);
    digitalWrite(beeper, LOW);
    //  turn off the beeper a random interval between OFF_TIME_MIN and OFF_TIME_MAX seconds
    offTime = random(OFF_TIME_MIN, OFF_TIME_MAX);

    sprintf(print_buffer, "Turning alarm OFF for %d. OFF_TIME_MIN=%d OFF_TIME_MAX=%d", offTime, OFF_TIME_MIN, OFF_TIME_MAX);
    Serial.println(print_buffer);;
    delay(offTime << 10);
  }
  else {
    sprintf(print_buffer, "Time is %d. START time is %d (%d:%d). END time is %d (%d:%d). Sleeping", timeInMinutes, startTime, START_HOUR, START_MINUTE, endTime, END_HOUR, END_MINUTE );
    Serial.println(print_buffer);
    delay(60000);
  }
}
