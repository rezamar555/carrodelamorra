/*
    Proyecto para la materia de Desarrollo de proyectos de automatización


    En esta versión se cambiaron varias cosas: Para hacer girar y dejar de girar se dividieron las funciones en dos estados,
    en vez de tenerlas en 4 estados como antes para que solo existan 2 estados y no se combinen
    El beep es intermitente cada medio segundo, no es continuo.
    Se usa el módulo BT para poder leer la información desde el Putty mediante la conexión previa con la compu.
    Sólo falta crear un temporizador, definir cuándo empieza y termina un recorrido y poner a funcionar el módulo de carga.
*/
#include "AFMotor.h"
#define SensorUno A8
#define SensorDos A9
#define trig 22// cable azul
#define echo 24// cable verde
#define buzzer 40
#define PB 44

/*Motores*/
AF_DCMotor MotorUno(1);// derecha adelante
AF_DCMotor MotorDos(2);// izquierda adelante
AF_DCMotor MotorTres(3);// derecha atras
AF_DCMotor MotorCuatro(4);// izquierda atras

int duration, distance;
byte LsensorU;
byte LsensorD;
int t1=0,t2=0;
int  starte=0;
int t, val,readd,k;



void setup() {
  /* parte 1. sensores infrarrojos*/
  pinMode(SensorUno, INPUT);
  pinMode(SensorDos, INPUT);
  /*para ver el estado del carrito en las pruebas*/
  Serial3.begin(9600);
  Serial.begin(9600);
  /*Parte 2. Sensor ultrasónico*/
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  /*Parte 3. Buzzer*/
  pinMode(buzzer,OUTPUT);
  /*Parte 4. Módulo de carga*/
  pinMode(PB,INPUT);
}

void loop() {
  
val = digitalRead(PB);
 LsensorU = digitalRead(SensorUno);//A5 Sensor derecha cable naranja
 LsensorD = digitalRead(SensorDos);//A4 Sensor izquierda cable café
 if((t>0)||((t>0)&&(LsensorU == LOW && LsensorD == LOW))){
  Serial.print("Se dejó de detectar peso");
  Serial.print("Tiempo total de recorrido:");
  Serial.println(t);
  Serial.println("\n");
  t=0,t1=0,t2=0;
  state3();  
  }


  
  while((val==HIGH)&&(LsensorU != LOW && LsensorD != LOW)){
    readd=1;
    starte=1;
    
    Serial.print("Peso detectado");
    Serial.print("Iniciando recorrido");
    
   
     val = digitalRead(PB); 
      digitalWrite(trig, HIGH);
      delay(100);
      digitalWrite(trig, LOW); 
      duration = pulseIn(echo, HIGH);
      distance = duration / 59  ;
  
      if(distance>=10){
        digitalWrite(buzzer,LOW);
        state1();
        t1++;
        Serial.print("Movimiento"); 
        Serial.println(t1);
        delay(1000);
        }
    
     if(distance<=10){
      state2();
      t2++;
      Serial.println(t2);
      delay(1000);

    }
    t=t1+t2;
    Serial.print("x \n");
    }  
   
   
    
    



}
 
  
  /*if(starte=1){
     val = digitalRead(PB); 
  int readd;
  if(val==HIGH){
    readd=1;
    }  
  if(val==LOW){
    readd=2;
    }
    digitalWrite(trig, HIGH);
  delay(100);
  digitalWrite(trig, LOW); 
  duration = pulseIn(echo, HIGH);
  distance = duration / 59  ;
  
  if(distance>=10){
      digitalWrite(buzzer,LOW);
      state1();
      t1++;
      Serial.print("Movimiento"); 
      Serial.println(t1);
     delay(1000);
    }
    
  if(distance<=10){
    state2();
    t2++;
     Serial.println(t2);
    delay(1000);

    }
  t=t1+t2;*/
  /*if(readd=2){
    starte=0;
    Serial.print("Peso retirado");
    Serial.print("terminado recorrido");
    Serial.print("tiempo total del recorrido\n");
    Serial.println(t);
    Serial.print(" S");
    
     }
           
   
  }
  
    if(starte=0){
      starte=3;
      readd=3;
    if(starte=3){
    t=0;
    t1=0;
    t2=0;
    }
    }
   */


  

 /*estados de movimiento*/
  void state1(){
  LsensorU = digitalRead(SensorUno);//A5 Sensor derecha cable naranja
  LsensorD = digitalRead(SensorDos);//A4 Sensor izquierda cable café
   if (LsensorU == HIGH && LsensorD == HIGH) {
    MotorUno.setSpeed(700);
    MotorDos.setSpeed(700);
    MotorTres.setSpeed(700);
    MotorCuatro.setSpeed(700);
    MotorUno.run(FORWARD); //para que vaya hacia adelante
    MotorDos.run(BACKWARD); // para que vaya hacia adelante
    MotorTres.run(BACKWARD); // hacia delante
    MotorCuatro.run(BACKWARD);// hacia delante
    Serial3.println("avanza"); //esto es para ver el estado
    }
  if (LsensorU == HIGH && LsensorD == LOW) {
    MotorUno.setSpeed(450);
    MotorDos.setSpeed(450);
    MotorTres.setSpeed(450);
    MotorCuatro.setSpeed(450);
    MotorUno.run(BACKWARD); //hacia atrás llanta DAdelante
    MotorDos.run(BACKWARD);//hacia delante llanta IAdelante
    MotorTres.run(BACKWARD); // hacia atrás DTrasera
    MotorCuatro.run(FORWARD);// hacia delante ITrasera
    Serial3.println("Derecha");
    }
  if (LsensorU == LOW && LsensorD == HIGH) {
   // para que se muva a la izquierda necesita detectar que el de la derecha esté en blanco
    Serial3.println("izquierda");
    MotorUno.setSpeed(700);
    MotorDos.setSpeed(650);
    MotorTres.setSpeed(700);
    MotorCuatro.setSpeed(650);
    MotorUno.run(FORWARD); //hacia delante llanta DAdelante
    MotorDos.run(FORWARD);//hacia atrás llanta IAdelante
    MotorTres.run(FORWARD); // hacia delante DTrasera
    MotorCuatro.run(BACKWARD);// hacia atrás ITrasera  
    }

  }

  void state2(){
    for(int c=0; c<1;c++){
      digitalWrite(buzzer,HIGH);
      delay(500);
    }
        //  Cuando se detecte un objeto a 10 cm de distancia con el ultrasónico el carrito deberá de parar, además se debe de emitir una alarma
    MotorUno.run(RELEASE);
    MotorDos.run(RELEASE);
    MotorTres.run(RELEASE);
    MotorCuatro.run(RELEASE);
    for(int c=0; c<1;c++){
      digitalWrite(buzzer,LOW);
      delay(500);
    }
    Serial.println("Objeto detectado");
    
    
     }

    void state3(){
      Serial3.println("ESPERA");
      MotorUno.run(RELEASE); 
      MotorDos.run(RELEASE);//
      MotorTres.run(RELEASE); 
      MotorCuatro.run(RELEASE);   
      }
    
