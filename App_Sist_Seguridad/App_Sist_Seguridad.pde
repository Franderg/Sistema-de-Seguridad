//Aplicacion del sistema de seguridad
//Instituto Tecnologico de Costa Rica
//Arquitectura de Computadores 1

import processing.serial.*; //Se import la libreria serial para la comunicacion por bluetooth


Serial port;                //Se instancia un puerto.
int[] PC_Time = new int[3]; //Variable para registrar la hora.
int[] DD_MM_YY= new int[3]; //Variable para registrar la fecha.
PImage img1,img2,img6,img7,img8,img9; //Se instancian las imagenes que se ocuparan para la interfaz
PImage img3,img4,img5;
PImage img10,img11;                                  //Se instancian las imagenes que se ocuparan para la interfaz
String curr_time,curr_date,filename,row_data;        //Se instancian los strings necesarios para la fecha, hora y el archivo de texto.
int inString;
int valueAlert =0;                                        //Variable de entrada
PrintWriter output;                                  //Variable para escritura de log.
PFont font;                                          //Variable predeterminada de la interfaz
boolean archivo = false;                             // Booleano exclusivo para crear el archivo que servira de registro en la aplicación
boolean sensor1 = true;
boolean sensor2 = true;
void setup(){
  size(450,500);                                     //Se establece el tamaño de la pantalla.
  background(255,255,255);                           //Se pinta la pantalla en blanco para cada actualizacion.
  img1 = loadImage("Detector.png");                  
  img2 = loadImage("Ultrasonico.png");              //Se cargan las imagenes necesarias para la interfaz.
  img3 = loadImage("A_Roja.gif");
  img4 = loadImage("A_Amarilla.gif");
  img5 = loadImage("A_Verde.gif");
  
  img6 = loadImage("Reporte1.png");
  img7 = loadImage("Reporte2.png");
  img8 = loadImage("Conexion1.png");
  img9 = loadImage("Conexion2.png");
  img10= loadImage("Detector2.png");
  img11= loadImage("Ultrasonico2.png");
  font = loadFont("ARBERKLEY-36.vlw");        //Se carga la fuente predeterminada del programa.
  port = new Serial(this, "COM9", 9600);      //Se carga un nuevo puerto serial
}

void draw(){
  background(102, 224, 255); //Color del fondo de la pantalla.
  if (sensor1)
    image(img1,40,150);      //Se colocan las imagenes en el marco de la ventana.
  else
    image(img10,40,150);
    
  if (sensor2)
    image(img2,40,275);      //Se colocan las imagenes en el marco de la ventana.
  else
    image(img11,40,275);
    
  image(img6,40,400);
  image(img8,235,400);
  
  fill(81,126,124);         //Se setea el color del pincel
  text("Security",30,50);   //Se pinta texto en pantalla.
  text("System",80,100);    //Se pinta texto en pantalla.
  
  curr_time = PC_Time();  //Se obtiene el tiempo del a funcion de tiempo
  curr_date = PC_Date();  //Se obtiene la fecha de la funcion de fecha.
  
  fill(234,93,21);        //Se setea el color del pincel.
  textFont(font);           //Se establece la fuente del texto.
  text(curr_date,225,50);   //Se imprime la fecha en la pantalla.
  text(curr_time,225,100); //Se imprime el tiempo en la pantalla.
  if(mousePressed){        //Condicional cuando se presiona el mouse.
      if(mouseX>40 && mouseX <40+100 && mouseY>150 && mouseY <150+100){  //Condicional para el boton del sensor 1
       println("Presionaste el boton del sensor uno");
       if (!sensor1){
         port.write('3');      //Desactiva el servo
       }
       else{
         port.write('2');     //Activa el servo
       }
       sensor1 = !sensor1;
       fill(0);
      }
      if(mouseX>40 && mouseX <40+100 && mouseY>285 && mouseY <285+100){  //Condicional para el sensor 2
       println("Presionaste el boton del sensor dos");
       if (!sensor2){
         port.write('1');      //Desactiva el ultrasonico
       }
       else{
         port.write('0');     //Activa el ultrasonico
       }
       sensor2 = !sensor2;
       fill(0); 
      }
      if(mouseX>40 && mouseX <40+170 && mouseY>400 && mouseY <400+70){  //Condicional para ver el reporte de alarmas.
       println("Presionaste el boton de reporte");
       image(img7,40,400);
       fill(0);
      }
      if(mouseX>235 && mouseX <235+170 && mouseY>400 && mouseY <400+70){ //Condicional para verificar conexion con el bluetooth
       println("Presionaste el boton de verificar conexion");
       image(img9,235,400);
       port.write('4');                                                                           //Se manda un mensaje de comprobacion de la conexion
       inString = port.read();                                                                   //Se recibe un mensaje de comprobacion de la conexion
       if (inString!=-1){
          javax.swing.JOptionPane.showMessageDialog ( null, "Conexion exitosa","Estado de la Conexion", javax.swing.JOptionPane.INFORMATION_MESSAGE  );
       }else if(inString==4 ||inString==52){
         javax.swing.JOptionPane.showMessageDialog ( null, "Error de conexion","Estado de la Conexion", javax.swing.JOptionPane.INFORMATION_MESSAGE  );
       }
       fill(0);
      }
      if(mouseX>175 && mouseX <175+250 && mouseY>125 && mouseY <125+250){  //Condicional para las imagenes de alarma.
       println("Presionaste el boton de alarma");
       String [] alarmas = loadStrings("Alertas.txt");
       String alarma = alarmas[alarmas.length-1];
       if (alarmas.length <=3){
          javax.swing.JOptionPane.showMessageDialog ( null, "No hay reportes","Ultima alerta", javax.swing.JOptionPane.INFORMATION_MESSAGE  );
       }else{
         javax.swing.JOptionPane.showMessageDialog ( null, alarma,"Ultima alerta", javax.swing.JOptionPane.INFORMATION_MESSAGE  );
       }
       fill(0);
      }
      output.flush();   //Se escribe todo lo almacenado en el archivo de texto.
      output.close();
      mousePressed = false;
     } 
  fill(255,255,255);
  if(!archivo){  //Condicional para crear el archivo recien se ejecuta el programa.
    filename = "Alertas"+".txt";
    output = createWriter(this.filename); //Se crea un fichero en la carpeta del programa.
    output.println("Fechas de monitoreo");
    output.println("");
    output.println("MM    DD    YYYY    HH    MM  SS");
    archivo=true;
  }
  
  
  inString = port.read();
  if(inString!=-1) println(inString);
  if ((inString ==1 || inString == 49)&&inString!=-1){          //Si el mensaje de alerta es amarillo
    valueAlert =1;
    image(img4,175,125);
    row_data = curr_date + "  " + curr_time + "Alerta amarilla";     //Se arma el string que se guardara en el archivo de texto.
    this.output.println(this.row_data);                              //Se graba el contenido en el archivo ".txt"
  }else 
    if ((inString ==2 || inString == 50)&&inString!=-1){       //Si el mensaje de alerta es rojo
      valueAlert =2;
      image(img3,175,125);    
      row_data = curr_date + "  " + curr_time + "Alerta roja";                         //Se arma el string que se guardara en el archivo de texto.
      this.output.println(this.row_data);                              //Se graba el contenido en el archivo ".txt"
    }else if ((inString ==3 || inString == 51)&&inString!=-1){
      valueAlert =3;
      image(img4,175,125);                                      //Si el mensaje de Alerta es verde
      row_data = curr_date + "  " + curr_time + "Alerta roja";                         //Se arma el string que se guardara en el archivo de texto.
      this.output.println(this.row_data);  
    }else
      if(inString==-1){
        if (valueAlert ==3){ //Alerta en verde
          image(img5,175,125);
        }else 
          if(valueAlert ==1){ //Alerta en amarillo
            image(img4,175,125);
          }
          else 
            if(valueAlert ==2){  //Alerta en rojo
              image(img3,175,125);
            }  
       }
       
  output.flush();   //Se escribe todo lo almacenado en el archivo de texto.
  output.close();
}

//Funcion para obtener el tiempo
String PC_Time(){
  PC_Time[2] = second();
  PC_Time[1] = minute();
  PC_Time[0] = hour();
  return join(nf(PC_Time,2)," : ");
}

//Funcion para obtener la fecha del ordenador
String PC_Date(){
  DD_MM_YY[2] = year();
  DD_MM_YY[1] = month();
  DD_MM_YY[0] = day();
  return join(nf(DD_MM_YY,2)," - ");
}