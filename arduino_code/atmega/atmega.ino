//pines alimentación
#define vccpir 1
#define vccultr 2
#define vccservo 3 
#define vccblue 4


//Servo
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 

//Pines a utilizar
//Ultrasonico
#define echoPin 9 // Echo Pin
#define trigPin 10 // Trigger Pin

//PIR
#define pirPin 11

//Bluetooth
#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(12,13);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
int pirState = LOW; //
int val = 0;                    // variable for reading the pin status
boolean ultrasonic = true ;
boolean servo = true;
int estado = 0;


void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(pirPin, INPUT);     // declare sensor as input
 myservo.attach(8);
}

void loop() {
   if (ultrasonic) digitalWrite(vccultr, HIGH);
   if (servo)digitalWrite(vccservo, HIGH);
   digitalWrite(vccblue, HIGH);
   digitalWrite(vccpir, HIGH);



  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    if (BT.read()==0) ultrasonic=true ;
    if (BT.read()==1) ultrasonic=false;
    if (BT.read()==2) servo=true;
    if (BT.read()==3) servo =false;
    if (BT.read()==4) BT.write(4);
  }
      


  
  //String bluetooth = BT.read();

  //Ultrasonico
  
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 


 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if(ultrasonic == true ){
   //alerta del ultrasonico   
 }
 
 if(servo == true){
   //alerta del servo   
 }
 
 //PIR
 
 val = digitalRead(pirPin);  // read input value PIR
  if (val == HIGH) {            // check if the input is HIGH
   if (pirState == LOW) {
     // we have just turned on
     Serial.println("Motion detected!");
     // We only want to print on the output change, not state
     pirState = HIGH;
   }
 } else {
   if (pirState == HIGH){
     // we have just turned of
     Serial.println("Motion ended!");
     // We only want to print on the output change, not state
     pirState = LOW;
   }
 }

 //bluetooth


    if (estado==1) BT.write(1);
    if (estado==2) BT.write(2);
}
