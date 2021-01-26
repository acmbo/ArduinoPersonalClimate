#include <OneWire.h> 
#include <DallasTemperature.h>
#include "PID_v1.h"

#define ONE_WIRE_BUS 12  //output of the DT sensor 从引脚12读取温度传感器的数据

#define pin 4  // green,to calculate the RPM of the fan 电风扇的数据从引脚4回到arduino，可以用来显示电扇的转速
#define pwm 9 // blau, pass the signal to the fan,output of the control loop,连接引脚9
#define pwm_heiz 5 // schalter wird mit PIN5 verbunden
double pwmValue = 100;  // output of the control loop
int pwmValue_heiz=100;

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

double t_input=22; //sollgroesse 设定值
double t_offset=40;//t_amb 环境温度
//double error=0;//误差
//double inte_error=0;
double k_p=2;
double k_i=5;
double k_d=1;
double i=0;
PID myPID(&t_input, &pwmValue, &t_offset, k_p, k_i, k_d, DIRECT);

unsigned long rpm = 5000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//视窗
  pinMode(pin, INPUT_PULLUP);//设置pin脚得到数据
  pinMode(pwm, OUTPUT);//设置pwm脚输出数据
  pinMode(pwm_heiz, OUTPUT);//设置pwm脚输出数据
  sensors.begin(); //开始测试温度
  
  myPID.SetMode(AUTOMATIC);
  analogWrite(pwm, pwmValue);//设置pwm为100
  analogWrite(pwm_heiz, 100);
  delay(2000);
}
/*
double PI_Regler(double k_p,double k_i)
{
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  t_input=sensors.getTempCByIndex(0);
  double y;
  error=t_input+t_offset;
  inte_error+=error;
  y=k_p*error+k_i*inte_error;
  if(y>255)
  { 
    y=255;
    }
  if(y<0)
  {
    y=0;
    }
  return y;
  }

*/

void loop() {
  // put your main code here, to run repeatedly:
//  pwmValue=PI_Regler(k_p, k_i);

sensors.requestTemperatures();
  t_input=sensors.getTempCByIndex(0);
  myPID.Compute();
  pwmValue=(int)pwmValue;
  if(pwmValue>255)
  { 
    pwmValue=255;
    }
  if(pwmValue<0)
  {
    pwmValue=0;
    }
  analogWrite(pwm, pwmValue);
 // analogWrite(pwm_heiz, pwmValue_heiz);
//  rpm = 30000/pulseIn(pin, LOW);
// Serial.println(rpm);
// Serial.println(i++);
//  Serial.print("\t");
  
//  Serial.print("Temperature");
//  Serial.print(t_input);
 // Serial.println(t_input);
 // Serial.println(error);
// Serial.println(inte_error);
 // Serial.print(" *C ");
 // Serial.print("\t");
 //Serial.println(pwmValue);

// Serial.println("Sensortemperature;  PWMVALUE");

  //Serial.print(i);
  //Serial.print(";");
  if(i==0){Serial.println("Sensortemperature;  PWMVALUE");
  }
  i++;
  
  if(t_input == -127){}else{
  Serial.print(t_input);}
  
 Serial.print(";");
 // Serial.println(error);
//  Serial.print(inte_error);
 // Serial.print(";");
 // Serial.print(" *C ");
 // Serial.print("\t");
 Serial.println(pwmValue);

  
}
