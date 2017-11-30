//pines alimentación
#define vccpir 1
#define vccultr 2
#define vccservo 3 
#define vccblue 4

//Servo
#include <Servo.h> 
Servo myservo; 

//Pines a utilizar
//Ultrasonico
#define echoPin 9 // Echo Pin
#define trigPin 10 // Trigger Pin

//PIR
#define pirPin 11

//Bluetooth
#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(12,13);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

long duration, distance; // Duracion para calcular la distancia
int pirState = LOW; //
int val = 0;                    // variable para leer el estatus del pir
boolean ultrasonic = false ;
boolean servo = false;
int estado = 0;
int preestado = 0;

void setup() {
 BT.begin(9600);
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(vccultr, OUTPUT);
 pinMode(vccservo, OUTPUT);
 pinMode(vccblue, OUTPUT);
 pinMode(vccpir, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(pirPin, INPUT);     // declare sensor as input
 myservo.attach(8);
}

void pir(){
  val = digitalRead(pirPin);
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      preestado = estado;
      estado = 2;
      Serial.println("Movimiento detectado");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Movimiento finalizado");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  delay(4000)
  estado = 0;
  preestado = estado;
}

void loop() {
   if (ultrasonic) digitalWrite(vccultr, HIGH);
   if (servo)digitalWrite(vccservo, HIGH);
   digitalWrite(vccblue, HIGH);
   digitalWrite(vccpir, HIGH);

  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    char BluetoothData = (BT.read());
      
    if (BluetoothData == '0') {ultrasonic=true;}  
    if (BluetoothData == '1') {ultrasonic=false;} 
    if (BluetoothData == '2') {servo=true;Serial.print(servo);}  
    if (BluetoothData == '3') {servo =false;Serial.print(servo);} 
    if (BluetoothData == '4') {BT.write(4);} 
  }
  
  //Ultrasonico
 if(ultrasonic == true ){
   /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */ 
   digitalWrite(trigPin, LOW); 
   duration = pulseIn(echoPin, HIGH);
   //Calculate the distance (in cm) based on the speed of sound.
   distance = (duration/2) / 29.1;
   Serial.print(distance);
   if (distance <= 22 || distance >= 28){
     BT.write(1);
     estado = 1;
     pir(digitalRead(inputPin));
   }
 }
 
 if (estado == 1){
   if(servo == true){
     for (int i = 120; i >= 0; i = i-60){
       myservo.write(i);
       delay(1000);
       pir();
       delay(1000);     
     }
      for (int i = 0; i <= 120; i = i+60){
       myservo.write(i);
       delay(1000);
       pir();
       delay(1000);     
     }
     myservo.write(0);
   }
 }
 
 //PIR
  val = digitalRead(pirPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      if (estado == 1){
        preestado = estado;
        estado = 2;
      }
      Serial.println("Movimiento detectado");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Movimiento finalizado");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
    delay(4000);
    estado = 0;
    preestado = estado;    
  }

 //bluetooth

    if ((estado==0) && (estado!=preestado)){
      BT.write(3); 
      Serial.println('Alerta Verde');
    }
    if ((estado==1) && (estado!=preestado)){
      BT.write(1); 
      Serial.println('Alerta Amarilla');
    }
    if ((estado==2) && (estado!=preestado)){
      BT.write(2); 
      Serial.println('Alerta Roja');
    }
}
