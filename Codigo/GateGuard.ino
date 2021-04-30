//Libreria y variables para el servo/lector de movimiento
#include <Servo.h>
int pir = 3, estado = 0;
String mov = "false";
Servo servo;	
unsigned long prevMillis_ser = 0;
const long interval_ser = 1500;
bool datasent = false;

//Variables para el sensor de ultrasonido
const int sigPin = 11;
int distanceThreshold = 150;
String sanadist = "false";
long distance_old = 0;
unsigned long prevMillis_ult = 0;
const long interval_ult = 1000;

//Variables para el sensor de temperatura
int umbral = 38 ; // temperatura de fiebre
int temp_old = 0;
String fiebre = "false";

//Variables senddata
unsigned long prevMillis_send = 0;
const long interval_send = 10000;

String ssid = "Simulator Wifi"; // SSID to connect to
String password = ""; // Our virtual wifi has no password
String host = "https://apex.oracle.com"; // Open Weather Map API
const int httpPort = 80;
String uri = "/pls/apex/tc1004b/gatepost/insert?valor_temp=";
String tmp = "&fiebre=";
String gel = "&uso_gel=";
String ult = "&sana_dist=";


int setupESP8266(void) {
// Start our ESP8266 Serial Communication
Serial.begin(115200); // Serial connection over USB to computer
Serial.println("AT"); // Serial connection on Tx / Rx port to ESP8266
delay(10); // Wait a little for the ESP to respond
if (!Serial.find("OK")) return 1;
// Connect to 123D Circuits Simulator Wifi
Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
delay(10); // Wait a little for the ESP to respond
if (!Serial.find("OK")) return 2;
// Open TCP connection to the host:
Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
delay(50); // Wait a little for the ESP to respond
if (!Serial.find("OK")) return 3;
return 0;
}

//-------------------------------------------------------
//Código del lector de tmp
//-------------------------------------------------------
int tempdata() {
int temp = map(analogRead(A0),20,358,-40,125);
if (temp>=umbral)
    digitalWrite(5, HIGH);
else
  digitalWrite(5,LOW);
return temp;
}

//-------------------------------------------------------
//Codigo para el sensor PIR y servo
//-------------------------------------------------------
String geldata(bool sent){
if (sent == true){ //Reinicia el estado si ya se enviaron los datos
  mov = "false";
}
estado = digitalRead(pir);
/*Cuando el sensor envia un alto el servomotor gira 90° y espera 2 segundos
De esta forma se acciona el dispensador de gel antibacterial*/
unsigned long currentMillis_ser = millis(); //Almacena el tiempo pasado
if(estado == HIGH){
 if (currentMillis_ser - prevMillis_ser > interval_ser){
   prevMillis_ser = currentMillis_ser;
   Serial.println("Dispensador activado");
   servo.write(90);
   mov = "true";
 }
}
  else{
   servo.write(0);
  }
return mov;
}

//------------------------------------------------------
//Codigo del sensor de ultrasonido
//------------------------------------------------------
String distdata(){
long cm, duration;
pinMode(sigPin, OUTPUT);  // Despeja el disparador
digitalWrite(sigPin, LOW);
delayMicroseconds(2);
// Establece el pin disparador a estado HIGH por un momento
digitalWrite(sigPin, HIGH);
delayMicroseconds(5);
digitalWrite(sigPin, LOW);
  
pinMode(sigPin, INPUT);
// Lee el pin echo, y regresa el tiempo de viaje de la onda de sonido 
duration = pulseIn(sigPin, HIGH);
// Convierte el tiempo en distancia
cm = microsecondsToCentimeters(duration);
if (cm>distanceThreshold)
  sanadist = "true";
if (cm<=distanceThreshold & sanadist=="true"){ //Avisa si se traspasó el umbral
  Serial.print("MANTEN LA DISTANCIA, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  sanadist = "false";
  }
unsigned long currentMillis_ult = millis();
  
//Activa una alarma si pasa el umbral de distancia
if(cm<= distanceThreshold & currentMillis_ult - prevMillis_ult >= interval_ult){
  tone(4,500,200);
  prevMillis_ult = currentMillis_ult;
}
  
delay(100);
return sanadist;
}

void senddata(){
int datot = tempdata();
if (datot>=umbral){ //Con la temperatura comprueba si es fiebre
  	fiebre = "true";
  }
else {
  fiebre = "false";
}
String datod = distdata();
String datog = geldata(datasent);
datasent = false; 
unsigned long currentMillis_send = millis(); 
//Se comprueba si pasaron 10 segundos para volver a mandar datos
if(currentMillis_send - prevMillis_send >= interval_send){ 
  String httpPacket = "GET " + uri + String(datot) + tmp + String(fiebre)+ gel 
  + String(datog) + ult + String(datod) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;
  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  datasent = true;
  prevMillis_send = currentMillis_send;
  if (!Serial.find("SEND OK\r\n")) return;
}
}
  
void setup() {
setupESP8266();
servo.attach(2);
servo.write(0);
pinMode(5,OUTPUT); //LED de tmp
pinMode(pir,INPUT);//Pin del PIR como entrada
}
void loop() {
senddata();
delay(100);
}
long microsecondsToCentimeters(long microseconds){ 
  return microseconds / 29 / 2;
}