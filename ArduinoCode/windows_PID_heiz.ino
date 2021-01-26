//Versuch der Regelung vom Heizwiderstand
//PI-Regler

#include <OneWire.h> 

#include <DallasTemperature.h>
#include "PID_v1.h"

#define ONE_WIRE_BUS 12  //the DT sensor Daten wird vom PIN 12 bekommen

#define pwm_heiz 5 // schalter wird mit PIN5 verbunden
double pwmValue = 100;  // PWM value für Ventilator, damit die Drehzahl vom Ventilator gereglt werden kann.
double pwmValue_heiz=0; // PWM value für Heizwiderstand

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

String command; //String für Parser
double t_input=40; //die Temoeratur vom Sensor
double t_offset=40;//Soll-Temperatur
double k_p=5;//die Verstärkung des proportionalen Anteil 
double k_i=0.3;//die Verstärkung des integrierenden Anteil 
double k_d=0; //Nur PI_Regler wird getestet,deswegen ist k_d=0
PID myPID(&t_input, &pwmValue_heiz, &t_offset, k_p, k_i, k_d, DIRECT);//PI-Regler

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//Parametrien des Fenster 
  pinMode(pwm_heiz, OUTPUT);//
  sensors.begin(); //Temperatur Sensor startet.
  
  myPID.SetMode(AUTOMATIC);//PI-Regler startet.

  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  parserCheck();//Parsersoll prüfen ob eine Commandline übergeben wurde
  sensors.requestTemperatures();//Temperatur vom Sensor abgefragt
  t_input=sensors.getTempCByIndex(0);//lesen die Temperatur des Sensor
  myPID.Compute();//regeln
  pwmValue_heiz=(int)pwmValue_heiz;// Formatkonvertierung vom double zur int 
  if(pwmValue_heiz>255) //PWM value ist zwischen 0 und 255
  { 
    pwmValue_heiz=255;
    }
  if(pwmValue_heiz<0)
  {
    pwmValue_heiz=0;
    }
    analogWrite(pwm_heiz,pwmValue_heiz);

//Ausgabe vom Daten
  Serial.print(pwmValue_heiz);
  Serial.print(" ");
  Serial.print(t_input);
  Serial.print(" ");
  Serial.println(t_offset);

}


//_________________________________________PARSER______________________________________________
  
void parserCheck() {
  //Prüfen ob Serial vom USB aus übergeben wird
  //Jeder Charakter wird durchgegangen bis eine new line übergeben wird. Dann wird der Befehl überprüft und ausgeführt
  while(Serial.available())
  {
      char c = Serial.read();
		
      if(c == '\n')
      //every command is separate from a line
      {
          //command wird überprüft, was für ein Befehl vorliegt
		  parseCommand(command);
          command = "";
      }
      else 
      {
          //Zusammenfügen der Charakter
		  command += c;
      }
    }
  

}

void parseCommand(String com)
{
  //Erstellen von Arbeitsvariablen
	String part1;
   String part2;

//Teilen des Strings in 2 Teile
   part1 = com.substring(0, com.indexOf(" "));
   part2 = com.substring(com.indexOf(" ") + 1);

   //Überprüfen von part 1 und part 2
 if(part1.equalsIgnoreCase("setOffset"))
  {
      int token2 = part2.toInt();
      
      t_offset = token2;
     //optionale Ausgabe
	 // Serial.print("Solltemperatur geändert auf ");
      //Serial.print(t_offset);
      //Serial.println(" C°");
  }
  else if(part1.equalsIgnoreCase("setHeiz"))
  {
    //Änderung PWHEIZ
    int token3 = part2.toInt();
    analogWrite(pwm_heiz, token3);
    
    }
    else if(part1.equalsIgnoreCase("ki"))
  {
    //Änderung K_I Ändern
    int token4 = part2.toInt();
    k_i=token4;
    //Serial.print("k_i geändert auf ");
     // Serial.print(token4);
    }
    else if(part1.equalsIgnoreCase("kp"))
  {
    //Änderung K_P
    int token5 = part2.toInt();
    k_p=token5;
    //Serial.print("k_p geändert auf ");
      //Serial.print(token5);
    }
  else
  {
      Serial.println("COMMAND NOT RECONGNIZED");
  }
  
}

