# Personal Temperature Controll Project

This repo describes the code for implementing a prototype for a 
personal climate system, which was implemented with a heating and
a cooling function. The body temperature of a person should be measured, 
and according to the temperature either a fan or a heat rod should be 
activated to controll the body temperature. The aim is to create a comfortable
temperature bubble for a working person infront of a desk, to save energy 
consumed by the HVAC system.

The concept was already proposed and prototyped in Arens and Zhang[2006]:

[comment]:#![Concept](doc/Concept.PNG "Concept"

<img src="/doc/Concept.PNG" width="300" />


The projects consisted of a hardware implementation with an 
Arduino micro controller with a compatible programm and a additional
monitor for plotting the incoming data from the Arduino on a computer system.
A major part of the project was to implement a PDI-Controller for controlling 
either a fan or a heating rod to warm the body.

## The PID-controller

The implementation of the controller does work on following assumptions:
- a constant body temperature is set at the beginning of the regulation to keep up for the whole process and equals T_Target(german: T_Soll)
- T_Target(german: T_Soll) can only be manual be changed. The adurino is capable to accept commands to change T_Target(german: T_Soll) and the PID reacts to this change
- both heating or cooling system react to changes of body temperature in changing their intensity, measured by the difference of bodytemperature T_Body(german:T_Haut) and T_Target(german: T_Soll)
- ideal body temperature lies around 37°Celsius

The following diagramm shows how the controller should react:

![Flowchart](doc/Flussdiagramm.png "Flussdiagramm")

Theoretic cases to compute by the adurino:

[comment]:#![Fallunt](doc/Fallunt.png "Fallunt"

<img src="/doc/Fallunt.png" width="200" />

### Used Hardware

On the harware side we used:
- Aduino Board UNO R3
- Contactsensor OneWire DS18B20
- Cpu fan
- 12R Resistor (german: Widerstand)
- Breadboard (german: Busleiste)

According to the controll loop in the chapter above, the tasks of the
components are represented as following:

![loop](doc/Loop.gif "Loop")

The follow scheme was used for connecting the components:

![scheme](doc/schematischeDarstellung.png "scheme")

The protoype looked as follows:

![Hardware](doc/Hardware1.PNG "Hardware")

### Software

On the software side, we computed the adurino with C++. We used existing
libaries to connect the Arduino with sensors and for the PID controller. We 
developed a command parser for input commands over computer.

The following scripts were used for these cases:
- _windows_PID_heiz.ino_ was used for heating case
- _windows_PID_venti.ino_ was used for cooling case

These commands where implemented:
- setOffset _double_: change T_Target temperature
- ki _double_: change K_i or integral part of the PID controller
- kp _double_: change K_p or change the constant part of the PID controller

## Calibration and Tests

To calibrate the controller we had to set K_i and K_p manually. To find appropriate we conducted
a test to determine the [Step response][link1] of our system for heating and cooling case.

For the following values we recieved a ideal response for the heating case:
![Heating](doc/StepResp_Heat.PNG "Heating")

And for the cooling case:
![Cooling](doc/StepResp_Col.PNG "Cooling")

In a livetest we generated good results for the cooling case with using the CPU fan:
![Livetest](doc/Livetest.PNG "Livetest")

For the heating case we couldnt replicate the results from the Step response test. There are also several downfalls
which we observed:

- There can be a discrepancy between mesured temperature and bodytemperature, because the sensor has to be glued to the body
- A solution with the cable sensor is very impractical. A better solution can be provided by using an infrared sensor
- The heating case was impractical because of the used hardware/heatrod. A better solution should be found here
- The environmental temperature influence isn't quite clear, but didn't had a strong inpact on the measurments 
- The fan distance is another unknown parameter, which has to be researched further 


## Sources

Arens, E., Zhang, H., 2006. The skin's role in human thermoregulation and comfort, in: N. Pan, P. Gibson (Eds.), Therm. Moisture Transp. Fibrous Mater., Wood- head Publishing Ltd, 560-602. 

## Credits
Project "Anwendungswerkstatt" in 2019 at RWTH Aachen E3D

By:

Yuhan Hu, Stephan Wegewitz, Evaluna Angelini, Shutong Li



[link1]: https://en.wikipedia.org/wiki/Step_response