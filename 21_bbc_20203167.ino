// Arduino pin assignment
#include <Servo.h>
#define PIN_SERVO 10
#define PIN_IR A0
#define PIN_LED 9

#define _DUTY_MIN 1550
#define _DUTY_NEU 1450
#define _DUTY_MAX 1350 

#define _SERVO_SPEED 550 
#define INTERVAL 20

#define _POS_START (_DUTY_MIN + 100)
#define _POS_END (_DUTY_MAX - 100)

unsigned long last_sampling_time;
int duty_chg_per_interval;
int toggle_interval, toggle_interval_cnt;
float pause_time;
Servo myservo;
int duty_target, duty_curr;
int a, b; // unit: mm

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);
   
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  duty_target = duty_curr = _POS_START;

// initialize serial port
  Serial.begin(57600);

  duty_chg_per_interval = (_DUTY_MAX - _DUTY_MIN) * ((float)_SERVO_SPEED / 180) * ((float)INTERVAL / 1000);

  pause_time = 1;
  toggle_interval = (180.0 / _SERVO_SPEED + pause_time) * 1000 / INTERVAL;
  toggle_interval_cnt = toggle_interval;

  last_sampling_time = 0;
  
  a = 71;
  b = 400;
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {

  if(millis() < last_sampling_time + INTERVAL) return;

  if(duty_target > duty_curr) {
    duty_curr += duty_chg_per_interval;
    if(duty_curr > duty_target) duty_curr = duty_target;
  }
  else {
    duty_curr -= duty_chg_per_interval;
    if(duty_curr < duty_target) duty_curr = duty_target;
  }
  
  float raw_dist = ir_distance();
  float dist_cali = 100 + 300.0 / (b - a) * (raw_dist - a);
  Serial.print("min:0,max:500,dist:");
  Serial.print(raw_dist);
  Serial.print(",dist_cali:");
  Serial.println(dist_cali);

   
  if(dist_cali > 90 && dist_cali <255){
    digitalWrite(PIN_LED, 0);
    myservo.writeMicroseconds(1550);
    delay(1000);
    myservo.writeMicroseconds(1450);
    delay(1000);
  }
  if(dist_cali > 255) {
    digitalWrite(PIN_LED, 255);
     myservo.writeMicroseconds(1350);
     delay(1000);
     myservo.writeMicroseconds(1450);
     delay(1000);
  }

  
  if(toggle_interval_cnt >= toggle_interval) {
    toggle_interval_cnt = 0;
    if(duty_target == _POS_START) duty_target = _POS_END;
    else duty_target = _POS_START;
  }
  else {
    toggle_interval_cnt++;
  }

// update last sampling time
  last_sampling_time += INTERVAL;

}
