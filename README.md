# Documentación GateGuard
##### Reto IoT, Grupo 500
###### Equipo 1
- Maximiliano Soberano Ramón A01733902
- Alexandra Beatie Hurtado A00826855
- Hector Silverio Cero Soto A01638843
- Alberto Eusebio Guajardo Lavín A00826548
 
**Materia:** Implementación de internet de las cosas

**Profesor:**
Dr. Lorena G. Gómez Martínez
Dr. Sergio Camacho

**Fecha:** 30/04/2021

**Universidad:** Instituto Tecnológico y de Estudios Superiores de Monterrey

## Arquitectura del proyecto
![Arquitecura GateGuard](https://github.com/Vinterum/reto-IoT-eq1/blob/main/Im%C3%A1genes/Dise%C3%B1o%20IoT.png?raw=true)

###### Código del circuito (https://github.com/Vinterum/reto-IoT-eq1/tree/main/Codigo)

	Hay dos códigos 'sensores.cpp' y 'ac.cpp' cada uno para su respectivo node.
	
	Lenguaje de programación:
   		- C++ para Arduino  
	Librerías:
    		- Arduino Json versión 5.0: Librería para poder hacer funcionar la librería de FirebaseArduino
			- FirebaseArduino: Librería para poder conectarse con la DB de Firebase Realtime Database
    		- ESP8266WiFi: Librería para poder conectar el NodeMCU a la red WiFi
    		- NewPing: Librería para facilitar el uso del sensor HC-SR04
    		- DHT: Librería para usar el sensor DHT 11
		
  	Hardware Usado:
    		- DHT11: Sensor de temperatura y humedad
    		- HC-SR04: Sensor ultrasónico
    		- Foco LED de 5mm
    		- Resistencia 220 Ω
    		- Node MCU ESP8266 V2
	IDE:
	  	- Arduino IDE
