//Versuch der Regelung vom Ventilator
//PI-Regler

#include <OneWire.h> 
#include <DallasTemperature.h>
#include "PID_v1.h"

#define ONE_WIRE_BUS 12  //the DT sensor Daten wird vom PIN 12 bekommen

#define pin 4  // Vom PIN4 wird die Drehzahl vom Ventilator gelesen
#define pwm 9 // Vom PIN9 wird die PWM Value vom Venzilator ausgegeben
#define pwm_heiz 5 // Vom PIN5 wird die PWM Value vom Heizwiderstand ausgegeben
double pwmValue = 100;  // PWM value für Ventilator, damit die Drehzahl vom Ventilator gereglt werden kann.
int pwmValue_heiz=70; // PWM value für Heizwiderstand

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

String command; //String für Parser
double t_input=40; //die Temoeratur vom Sensor
double t_offset=40;//Soll-Temperatur
double k_p=50;//die Verstärkung des proportionalen Anteil
double k_i=40;//der Parameter des integrierenden Anteil 
double k_d=0.5; //der Parameter des differenzielen Anteil
PID myPID(&t_input, &pwmValue, &t_offset, k_p, k_i, k_d, DIRECT);//PID-Regler

int i=0;//Zähler des Zyklus

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//Parametrien des Fenster
  pinMode(pin, INPUT_PULLUP);
  pinMode(pwm, OUTPUT);
  pinMode(pwm_heiz, OUTPUT);
  sensors.begin(); //Temperatur Sensor startet.
  
  myPID.SetMode(AUTOMATIC);//PI-Regler startet.
  analogWrite(pwm, pwmValue);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  i++; //zählen die Zyklen
  parserCheck();//Parsersoll prüfen ob eine Commandline übergeben wurde
  sensors.requestTemperatures();//Temperatur vom Sensor abgefragt
  t_input=sensors.getTempCByIndex(0);//lesen die Temperatur des Sensor
  myPID.Compute();//regeln
  pwmValue=(int)pwmValue;// Formatkonvertierung vom double zur int 
  if(pwmValue>255)  //PWM value ist zwischen 0 und 255
  { 
    pwmValue=255;
    }
  if(pwmValue<0)
  {
    pwmValue=0;
    }
  analogWrite(pwm, 255-pwmValue);

  //Heizwiderstand wird eingeschaltet am 5th Zyklus. Hier funktioniert der Widerstand als eine Einheitssprung 
  if(i==35)
  {
    analogWrite(pwm_heiz,pwmValue_heiz);
    }
  
//Ausgabe vom Daten
  Serial.print(255-pwmValue);
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
