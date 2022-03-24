
#include <Servo.h>
Servo servoMotor;
int n=3;
int pingPin[] = {A0,A1,A2};
int pinLed=13;
int pinResistor=A3;
int pinLightOutsideStore=3;
double valueNight;
int Alarm=13;

struct UltraSonic{
  
 long duration;
 long inches;
 long cm;
  
};

struct UltraSonic machine[3];
  
void setup() {
  
 //setting the input/output pins
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  pinMode(pinResistor, INPUT);
  pinMode(Alarm, OUTPUT);
  servoMotor.attach(2);
  afiseazaOptiuni();
}

void loop() {
 if (Serial.available()) {


    String com = Serial.readString();
    char comanda = com.charAt(0)&0xDF;   //se forteaza litere mari NOT 20H
    
    switch (comanda) {
     case 'A':
        afiseazaOptiuni();
        break;
 case 'C' :       //turning OFF the lights outside the store
        {
         digitalWrite(pinLightOutsideStore, LOW);
         
         Serial.println("light turned off");
        }
        break;
       case 'B' :       //turning ON the lights outside the store
        {
         digitalWrite(pinLightOutsideStore, HIGH);
         
         Serial.println("light turned on");
        }
        break;
case 'T' :    //testing ALARM
        {
         
          Serial.print( "An administrator is needed to stop the alarm\n");
          Serial.print("The motors that activate the protection for doors and windows have been started\n");
  Serial.println(); 
  for(int i=0;i<n;i++){
               
  pinMode(pingPin[i], OUTPUT);
  digitalWrite(pingPin[i], LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin[i], HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin[i], LOW);


  pinMode(pingPin[i], INPUT);
  machine[i].duration = pulseIn(pingPin[i], HIGH);

  machine[i].inches = microsecondsToInches(machine[i].duration);
  machine[i].cm = microsecondsToCentimeters(machine[i].duration);

     Serial.print("The value recorded by the motion sensor ");
       Serial.print(i);
        Serial.print("  with ultrasound");
      
  Serial.print(":  ");  
  Serial.print(machine[i].inches);
  Serial.print("in, ");
  Serial.print(machine[i].cm);
  Serial.print("cm");
  Serial.println();
  
    
  valueNight=analogRead(pinResistor) * (5.0 / 1023.0);
     servoMotor.write(0);
  if(machine[i].cm<300 && machine[i].cm>200 && valueNight>3.4)
  { digitalWrite(pinLed,HIGH);
    servoMotor.write(40);
    delayMicroseconds(500);
    digitalWrite(pinLed,LOW);
    delay(100);
    tone(Alarm,200,100);
  }
    else
      digitalWrite(pinLed,LOW);
    if(machine[i].cm<=200 && machine[i].cm>=3 && valueNight>3.4)
    {digitalWrite(pinLed,HIGH); 
    tone(Alarm,500,300);servoMotor.write(40);}
    else
      digitalWrite(pinLed,LOW);
    

  
  }
          //the alarm will be ON until the administrator arrives
        tone(Alarm,5000,5000);  
        }
break;
 case 'O' :       //activate light by the administrator
        {
         digitalWrite(pinLightOutsideStore, HIGH);
          digitalWrite(pinLed, LOW);
           digitalWrite(Alarm, LOW);
           servoMotor.write(0);
         Serial.println("The administrator will turn off the alarm and turn on the outside light .");
          noTone(Alarm); 
        }
      break;
    }}}
long microsecondsToInches(long microseconds) {

  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  
  return microseconds / 29 / 2;
}
void afiseazaOptiuni() {
  Serial.write("Commands:\n");
  Serial.write("A - display available commands: \n");
  Serial.write("B - turn ON the lights outside the shop \n");
  Serial.write("C - turn OFF the lights outside the shop  \n");
  Serial.write("T - test the alarm system \n"); //one motor will move all the security doors, another will activate the window grille and the photoresistor will activate the alarm and light
  Serial.write("O - Safety system shutdown by the administrator\n");
  Serial.write("\n");
 
}

