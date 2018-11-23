#include <LiquidCrystal.h>
#include <Servo.h>

//Declaracion de variables
const int PIEZO  = 13;
const int LED  	 = 12;
const int SERVO  = 11;
const int ENC_M	 = 10;
const int BOTON  =  9;
const int S_TEMP = A1;
const int S_GAS  = A2;
Servo miServo;

// Salidas del display 
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

//Variables del programa
int tempC = 0;
int gas = 0;
int estadoServo = 0;
int botonEstado = 0;
int alertaManual = 0;
bool alarmaIncendio = 0;
void setup() {
  //Inicializar puertos
  pinMode(PIEZO,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(BOTON,INPUT);
  pinMode(ENC_M,INPUT);
  //Iniciar el servo en 0
  miServo.attach(SERVO);
  miServo.write(0);
  //Iniziar LCD
  lcd.begin(16,2);
}

void loop() {
  //Leer el estado del boton
  botonEstado = digitalRead(BOTON);
  alertaManual = digitalRead(ENC_M);
  //Leer el senesor de temperatura y de humo
  verSensores();
  //Si la alarma está encendida
  if(botonEstado){
    //Alarma encendida
    // mostrará el mensaje a menos que
    // haya alerta de incendio.
    if(alarmaIncendio==false){
    	lcd.setCursor(0,0);
  		lcd.print("ALARMA ENCENDIDA");
    }
    //Si encuentra que la temperatura es
    // mayor o igual que 60°C o que detecta 
    // mas de 20% de humo, activa la alerta
    // de incendio.
    if(alertaManual == 0|| tempC >= 60 || gas>=20){
      activarAlarm();    
      lcd.setCursor(0,0);
  	  lcd.print("--ALERTA FUEGO--");
      alarmaIncendio = true;
    }
    else{
    	alarmaIncendio= false;
    }
  }
  else{
    //Apaga la alarma (el led y el zumbador)
    // y muestra un mensaje.
    // se sigue mostrando la temperatura y gas.
    desactivarAlarm();    
    lcd.setCursor(0,0);
    lcd.print(">ALARMA APAGADA<");
  }	   	
  
}

/*Si hay alerta de incendio activa la alarma
   Enciende LED y hace sonar la alarma dependiendo 
   si hay mas gas o temperatura.
   si hay mas de 75°C o 40% de gas activa el servo
   (llave de agua).*/
void activarAlarm(){
	digitalWrite(LED,1);
  	delay(100);
  	digitalWrite(LED,0);
  	int tono = 420;
  	if(tempC >=60 && tempC <75)
      tono = 520;
    if(tempC >= 75 && tempC <95)
      tono = 620;
    if(tempC >= 95  && tempC <=125)
      tono = 720;
    
  	tone(PIEZO,tono,1000);
  	delay(100);
    noTone(PIEZO);
  if((tempC>75 || gas> 30) && estadoServo == 0){
      miServo.write(180);
    	estadoServo == 1;
  }
}

/*Con una alarma desactivada apaga el LED
  y apaga la alarma (buzzer).*/
void desactivarAlarm(){
	digitalWrite(LED,0);
  	noTone(PIEZO);
}

/*Actualiza los sensores
	Lee el sensor de gas y temperatura
    haciendo un mapeo de la lectura segun los 
    datos minimos y maximos que puede captar cada
    sensor, ademas los muestra en la parte baja del LCD.*/
void verSensores(){
  	gas = analogRead(S_GAS);
  	gas = map(gas,85,390,0,100);    	
	tempC = analogRead(S_TEMP);
    tempC = map(tempC,20,355,-40,125); 	  
  	lcd.setCursor(0,1);
    lcd.print(String("Tem:")+String(tempC)+String("C")+String(" Gas:")+String(gas)+String("%  "));
}

