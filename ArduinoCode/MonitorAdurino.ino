#include <OneWire.h> 
#include <DallasTemperature.h>
#include "PID_v1.h"
#include <Adafruit_MLX90614.h>
#define ONE_WIRE_BUS 12  //output of the DT sensor 从引脚12读取温度传感器的数据
#include <Wire.h>
#define pin 4  // green,to calculate the RPM of the fan 电风扇的数据从引脚4回到arduino，可以用来显示电扇的转速
#define pwm 9 // blau, pass the signal to the fan,output of the control loop,连接引脚9
#define pwm_heiz 5 // schalter wird mit PIN5 verbunden
double pwmValue = 100;  // output of the control loop
int pwmValue_heiz=100; // Schwingen bei 110 beobachtet
int counter=0;
int Delay =200;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

String command; //String für Parser
double t_input=20; //sollgroesse 设定值
double t_offset=40;//t_amb 环境温度
//double error=0;//误差
//double inte_error=0;
double k_p=100;//Schwingen bei 100 beobachtet
double k_i=100;//Schwingen bei 100 beobachtet
double k_d=1; //Schwingen bei 1 beobachtet
PID myPID(&t_input, &pwmValue, &t_offset, k_p, k_i, k_d, DIRECT);

unsigned long rpm = 5000;

int i=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//视窗
  pinMode(pin, INPUT_PULLUP);//设置pin脚得到数据
  pinMode(pwm, OUTPUT);//设置pwm脚输出数据
  pinMode(pwm_heiz, OUTPUT);//设置pwm脚输出数据
  sensors.begin(); //开始测试温度
   delay(Delay);
  myPID.SetMode(AUTOMATIC);
  analogWrite(pwm, pwmValue);//设置pwm为100
  mlx.begin();  
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
  i++;
  parserCheck();
 
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
  analogWrite(pwm, 255-pwmValue);
  if(i==50)
  {
    
    analogWrite(pwm_heiz,pwmValue_heiz);
    }
  //else
   //{
    
    //analogWrite(pwm_heiz,0);
   // }
//  rpm = 30000/pulseIn(pin, LOW);
// Serial.println(rpm);
//   Serial.println(i++);
//  Serial.print("\t");
  
//  Serial.print("Temperature");
  if(counter=0){
    Serial.print("pwm");
    Serial.print(" "); 
    Serial.print("Temperatur Sensor");
    Serial.print(" "); 
    Serial.println("T_Soll");
    }
   counter = counter + 1;

   Serial.print(mlx.readObjectTempC());
   Serial.print(" ");
   Serial.print(t_input);
 // Serial.println(error);
// Serial.println(inte_error);
  Serial.print(" ");
 // Serial.print("\t");
 Serial.println(mlx.readAmbientTempC());
}


//_____PARSER_____

 
  
void parserCheck() {
  if(Serial.available())
  {
      char c = Serial.read();
      String txt=Serial.readString();
       if(txt=="Hallo"){
        Serial.print("Hallo");}
      if(c == '\n')
      //every command is separate from a line
      {
          //parseCommand(command);
          command = "";
      }
      else 
      {
          command += c;
          parseCommand(command);
          //Serial.print(command);
      }
     
        }
    
  

}

void parseCommand(String com)
{
   String part1;
   String part2;


   //PINON 9
   //PINOFF 9

   part1 = com.substring(0, com.indexOf(" "));
   part2 = com.substring(com.indexOf(" ") + 1);
//Serial.println("stephan dumm");
 if(part1.equalsIgnoreCase("setOffset"))
  {
      int token2 = part2.toInt();
      //Serial.println("stephan dumm");
      //digitalWrite(pin, LOW);
      //analogWrite(pwm, token2); um Fan geschwindigkeit zu ändern
      t_offset = token2;
      //Serial.print("Solltemperatur geändert auf ");
      //Serial.print(t_offset);
      //Serial.println(" C°");
  }
  else if(part1.equalsIgnoreCase("setHeiz"))
  {
    
    int token3 = part2.toInt();
    analogWrite(pwm_heiz, token3);
    //Serial.print("Solltemperatur geändert auf ");
      //Serial.print(token3);
    }
  else
  {
      //Serial.println("COMMAND NOT RECONGNIZED");
  }
  
}
