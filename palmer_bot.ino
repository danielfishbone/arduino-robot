#include <PS2X_lib.h>                 //PS2X header file
#include <Servo.h>                    //servo header file

PS2X ps2x;                            // create PS2 Controller Class

Servo base_l;
Servo joint_la;
Servo joint_lb;
Servo yaw_l;
Servo gripper_l;
Servo base_r; 
Servo joint_ra;
Servo joint_rb;
Servo yaw_r;
Servo gripper_r;

//            PINS
int r=28;                             //red led
int g=30;                             //green led
int b=32;                             //blue led
int dir_la=34;                        //motor direction for left A
int dir_lb=22;                        //motor direction for left B
int dir_ra=24;                        //motor direction for right A
int dir_rb=26;                        //motor direction for right B
int pwm_l=2;                          // pin for left motor speed
int pwm_r=3;                          // pin for right motor speed  
int trig_l=23;                        //trigger pins for ultrasonic sensors
int trig_m=25;
int trig_r=27;
int echo_l=29;                        // echo pins for ultrasonic sensors
int echo_m=31;
int echo_r=33;
int line_l=35;                         // line sensor for left
int line_r=37;                         // line sensor for right

//           VARIABLES

int error = 0;                        //to check for gamepad errors
int speed_pot=0;
int speed_motor=0;                     // variable to hold motor speed from potentiometer
int error_led=0;                      // variable to hold error
int sensor_lx=0;                      //unmapped analog value
int sensor_ly=0;                      //unmapped analog value
int sensor_rx=0;                      //unmapped analog value
int sensor_ry=0;                      //unmapped analog value
int analog_lx=0;                      //mapped analog value
int analog_ly=0;                      //mapped analog value
int analog_rx=0;                      //mapped analog value
int analog_ry=0;                      //mapped analog value
int mode=28;                           // variable to hold robot mode
int angle_rx=0;                        //variable for angles
int angle_ry=0;                        //variable for angles
int angle_lx=0;                        //variable for angles
int angle_ly=0;                        //variable for angles
int yaw_rx=0;
int yaw_lx=0;
int left_l=0;
int right_l=0;
int distance_m=0;                        //variable for ultrasonic sensor
int distance_l=0;                        //variable for ultrasonic sensor
int distance_r=0;                        //variable for ultrasonic sensor
int thresh=15;                            //threshold for obstacle


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(r,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(dir_la,OUTPUT);
  pinMode(dir_lb,OUTPUT);
  pinMode(dir_ra,OUTPUT);
  pinMode(dir_rb,OUTPUT);
  pinMode(pwm_l,OUTPUT);
  pinMode(pwm_r,OUTPUT);
  pinMode(trig_l,OUTPUT);
  pinMode(trig_m,OUTPUT);
  pinMode(trig_r,OUTPUT);
  pinMode(line_l,INPUT);
  pinMode(line_r,INPUT);
  pinMode(echo_l,INPUT);
  pinMode(echo_m,INPUT);
  pinMode(echo_r,INPUT);
   base_l.attach(4);  //set up servo object for left base
   joint_la.attach(5);//set up servo object for left joint a
   joint_lb.attach(6);//set up servo object for left joint b
   yaw_l.attach(7);//set up servo object for left  yaw
   gripper_l.attach(8);//set up servo object for left gripper
   base_r.attach(9);//set up servo object for right base
   joint_ra.attach(10);//set up servo object for right joint a
   joint_rb.attach(11);//set up servo object for rightjoint b
   yaw_r.attach(12);//set up servo object for right yaw
   gripper_r.attach(13);//set up servo object for right gripper

 //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
error = ps2x.config_gamepad(52,51,53,50, true, true);
 if(error == 0){
       
 }
 else if(error == 1){error_led=28;}
 else if(error == 2){error_led=30;}
 else if(error == 3){error_led=32;}
 if(error > 0){
   for (int i=0;i<=2;i++)
     {
      digitalWrite(error_led,HIGH);
      delay(10);
      digitalWrite(error_led,LOW);
      delay(800);
      }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  speed_motor =analogRead(A0);
speed_pot=map(speed_motor,0,1024,0,255);
ps2x.read_gamepad(false,0);
if(ps2x.ButtonPressed(PSB_START)||ps2x.ButtonPressed(PSB_SELECT)){
  led_off();
  mode+=2;
  if (mode>32)
    {
      mode=28;
       }             
}
 digitalWrite(mode,HIGH);
 if (mode==28){
remote_control();
}
else if (mode==30)
{
  follow_line();
  }
else if (mode==32)
{
  obstacle_avoidance();
  }

//distance_m=distance(trig_m,echo_m);
//distance_l=distance(trig_l,echo_l);
//distance_r=distance(trig_r,echo_r);
//
//Serial.print("a");
//Serial.print(distance_m);
//Serial.print("   b");
//Serial.print(distance_l);
//Serial.print("   c");
//Serial.println(distance_r);
delay(30);
}



void follow_line()  // line following function
{
  left_l=digitalRead(line_l);
  right_l=digitalRead(line_r);
if (left_l<=0&&right_l<=0)
{
  forward(130);
  }
 else if(left_l<=0&&right_l>=1)
 {
 roll_r(255);
  }
else if(left_l>=1&&right_l<=0)
{
  roll_l(255);
}
  else{
    stp();
  }
  }


void obstacle_avoidance()  //obstacle avoidance function
{

distance_m=distance(trig_m,echo_m);

if (distance_m>thresh)
{  
  forward(180);
  }
else// ((distance_m<thresh)||(distance_r<thresh) ||(distance_l<thresh))
{
distance_l=distance(trig_l,echo_l);
distance_r=distance(trig_r,echo_r);
  
if(distance_r>distance_l){
 
 roll_r(200);
  delay(1000);
 stp();
 }
 else if(distance_r<=distance_l)
 {
 
  roll_l(200);
 delay(1000);
 stp();}
  }
 // stp();
  }


void remote_control()  //remot control function
{
   if(ps2x.ButtonPressed(PSB_R1)){
    gripper_r.write(100);
    } 
   if(ps2x.ButtonReleased(PSB_R1)){
    gripper_r.write(180);}
   if(ps2x.ButtonPressed(PSB_L1)){
    gripper_l.write(100);
    } 
   if(ps2x.ButtonReleased(PSB_L1)){
    gripper_l.write(180);}
   sensor_lx=ps2x.Analog(PSS_LX); 
   sensor_ly=ps2x.Analog(PSS_LY);
   sensor_rx=ps2x.Analog(PSS_RX);                      
   sensor_ry=ps2x.Analog(PSS_RY);
    analog_lx=map(sensor_lx,0,255,-5,5);                      //mapped analog value
    analog_ly=map(sensor_ly,0,255,-5,5);                      //mapped analog value
    analog_rx=map(sensor_rx,0,255,-5,5);                      //mapped analog value
    analog_ry=map(sensor_ry,0,255,-5,5);                      //mapped analog value
if(! ps2x.Button(PSB_R2)){
  // Right arm
    if(analog_ry>1||analog_ry<-1){
      angle_ry=angle_ry + analog_ry;
      if(angle_ry>=180)
      {
        angle_ry=180;
        }
      else if(angle_ry<=0)
        {
          angle_ry=0;
          }
        base_r.write(180-angle_ry);  
    }
    if(analog_rx>1||analog_rx<-1){
      angle_rx=angle_rx + analog_rx;
      if(angle_rx>=180)
      {
        angle_rx=180;
        }
      else if(angle_rx<=0)
        {
          angle_rx=0;
          }
        joint_rb.write(180-angle_rx);
        delay(10);  
        joint_ra.write((180-angle_rx)); }  
        }
     
      if(ps2x.Button(PSB_R2))
         {
          if(analog_rx>1||analog_rx<-1){
      yaw_rx=yaw_rx + analog_rx;
      if(yaw_rx>=180)
      {
        yaw_rx=180;
        }
      else if(yaw_rx<=0)
        {
          yaw_rx=0;
          }
        yaw_r.write(yaw_rx);
          }
         }
    // left arm
    if(! ps2x.Button(PSB_L2)){
     if(analog_ly>1||analog_ly<-1){
      angle_ly=angle_ly + analog_ly;
      if(angle_ly>=180)
      {
        angle_ly=180;
        }
      else if(angle_ly<=0)
        {
          angle_ly=0;
          }
        base_l.write(angle_ly);  
    }
    if(analog_lx>1||analog_lx<-1){
      angle_lx=angle_lx + analog_lx;
      if(angle_lx>=180)
      {
        angle_lx=180;
        }
      else if(angle_lx<=0)
        {
          angle_lx=0;
          }
        joint_lb.write(180-angle_lx);
        delay(10);  
        joint_la.write((180-angle_lx)); }}
     
      if(ps2x.Button(PSB_L2))
         {
          if(analog_lx>1||analog_lx<-1){
      yaw_lx=yaw_lx + analog_lx;
      if(yaw_lx>=180)
      {
        yaw_lx=180;
        }
      else if(yaw_lx<=0)
        {
          yaw_lx=0;
          }
        yaw_l.write(yaw_lx);
}
}

 if(ps2x.Button(PSB_PAD_UP)) {
  forward(speed_pot);
 }
 else if(ps2x.Button(PSB_PAD_DOWN)){
  backward(speed_pot);
 }
 else if(ps2x.Button(PSB_PAD_RIGHT)){
  right(speed_pot);
 }
 else if(ps2x.Button(PSB_PAD_LEFT)){
  left(speed_pot);
 }
 else {stp();}
}


void led_off(){
  digitalWrite(r,LOW);
  digitalWrite(g,LOW);
  digitalWrite(b,LOW);
  
  }

void forward(int a)
{
digitalWrite(dir_rb,HIGH);
digitalWrite(dir_ra,LOW);
digitalWrite(dir_lb,HIGH);
digitalWrite(dir_la,LOW);
analogWrite(pwm_r,a);
analogWrite(pwm_l,a);
  
 }
void backward(int a)
  {
digitalWrite(dir_rb,LOW);
digitalWrite(dir_ra,HIGH);
digitalWrite(dir_lb,LOW);
digitalWrite(dir_la,HIGH);
analogWrite(pwm_r,a);
analogWrite(pwm_l,a);
  }
void left(int a)
  {
int b=255-a;    
digitalWrite(dir_rb,HIGH);
digitalWrite(dir_ra,LOW);
digitalWrite(dir_lb,LOW);
digitalWrite(dir_la,HIGH);
analogWrite(pwm_r,a);
analogWrite(pwm_l,b);   
  }
void right(int a)
{
int b=255-a;  
digitalWrite(dir_rb,LOW);
digitalWrite(dir_ra,HIGH);
digitalWrite(dir_lb,HIGH);
digitalWrite(dir_la,LOW);
analogWrite(pwm_r,b);
analogWrite(pwm_l,a);

 }
 void roll_r(int a){
 // int b=255-a;
digitalWrite(dir_rb,LOW);
digitalWrite(dir_ra,HIGH);
digitalWrite(dir_lb,HIGH);
digitalWrite(dir_la,LOW);
analogWrite(pwm_r,a);
analogWrite(pwm_l,a);
}
 void roll_l(int a){
  // int b=255-a; 
digitalWrite(dir_rb,HIGH);
digitalWrite(dir_ra,LOW);
digitalWrite(dir_lb,LOW);
digitalWrite(dir_la,HIGH);
analogWrite(pwm_r,a);
analogWrite(pwm_l,a); 
}
 void stp()
 {
digitalWrite(dir_rb,LOW);
digitalWrite(dir_ra,LOW);
digitalWrite(dir_lb,LOW);
digitalWrite(dir_la,LOW);  
 }

  int distance(int a,int b) {
  int duration, distance_1;
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  duration = pulseIn(b, HIGH);
  distance_1 = duration/58.2;
  return distance_1;}
  
