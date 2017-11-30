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
int val2 = 0;
boolean ultrasonic = false ;
boolean preultrasonic=false;
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
 myservo.write(0);
}

void pir(){
  ultrasonic = false;
  delay(1000);
  val2 = digitalRead(pirPin);
  if (val2 == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      if (estado == 1){
        preestado = 1;
        estado = 2;
        BT.write(2);
      }
      Serial.print("preestado");
      Serial.println(preestado);
      Serial.print("estado");
      Serial.println(estado);
      Serial.println("Movimiento detectado");
      }
      // We only want to print on the output change, not state
      pirState = HIGH;
    
  } else {
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Movimiento finalizado");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

void loop() {
   val = digitalRead(pirPin);  // read input value
   if (ultrasonic) digitalWrite(vccultr, HIGH);
   if (servo)digitalWrite(vccservo, HIGH);
   digitalWrite(vccblue, HIGH);
   digitalWrite(vccpir, HIGH);

  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    char BluetoothData = (BT.read());
    //Serial.println(BluetoothData); 
    if (BluetoothData == '0') {ultrasonic=true;}  
    if (BluetoothData == '1') {ultrasonic=false;} 
    if (BluetoothData == '2') {servo=true;}  
    if (BluetoothData == '3') {servo =false;} 
    if (BluetoothData == '4') {
      estado == 0 ? BT.write(3) : BT.write(estado);
      servo ? BT.write(5) : BT.write(6);
      ultrasonic ? BT.write(7) : BT.write(8);
      BT.write(4);
    } 
  }
  
  //Ultrasonico
 if(ultrasonic == true ){
   /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */ 
   digitalWrite(trigPin, LOW);  // Added this line
   delayMicroseconds(2); // Added this line
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10); // Added this line
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   //Calculate the distance (in cm) based on the speed of sound.
   delay(1000);
   distance = (duration/2) / 29.1;
   Serial.println(distance);
   if (distance >= 30 || distance <= 10){
     preestado = 0;
     estado = 1;
     pir();
   }
 }
 
 if (estado == 1){
   if(servo){
     for (int i = 0; i <= 120; i = i+60){
       myservo.write(i);
       delay(1000);
       val = 0;
       pir();     
     }
      for (int i = 120; i <= 0; i = i-60){
       myservo.write(i);
       delay(1000);
       pir();    
     }
     myservo.write(0);
     delay(100);
   }
   val = digitalRead(pirPin);  // read input value
 }
 
 //PIR
  
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      if (estado == 1){
        preestado = 1;
        estado = 2;
        BT.write(2);
      }
      Serial.print("preestado loop");
      Serial.println(preestado);
      Serial.print("estado loop");
      Serial.println(estado);
      Serial.println("Movimiento detectado loop");
      if(estado==0){
        preestado = 0;
        estado = 1;
        preultrasonic=ultrasonic;
        ultrasonic = false;
      }
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Movimiento finalizado loop");
      // We only want to print on the output change, not state
      pirState = LOW;
    } 
  }

 //bluetooth

    if ((estado==0) && (estado!=preestado)){
      BT.write(3); 
      Serial.println("Alerta Verde");
      preestado=0;
      ultrasonic = preultrasonic;
    }
    if ((estado==1) && (estado!=preestado)){
      BT.write(1); 
      Serial.println("Alerta Amarilla");
      preestado = 1;
    }
    if ((estado==2) && (estado!=preestado)){
      BT.write(2); 
      Serial.println("Alerta Roja");
      preestado=2;
      delay(8000);
      estado=0;
    }
}
