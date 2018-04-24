/*
Bolt Driver
Stepper Moter
Nick Jarvis
Matthew Schaefer
Push Button
  */
#include <avr/io.h>
#include <avr/interrupt.h>

#define IN1  8    //stepper motor A
#define IN2  9    //stepper motor B
#define IN3  10   //stepper motor C
#define IN4  11   //stepper motor D
#define IN5  12   //AC light control
#define LOCK 13    //Input from Server Lock
#define LIGHT 4   //Input from Server Light
//Dim1 and Dim2: 00(FULL on), 01(25%Dim), 10(50%Dim), 11(Dim75%)
#define Dim1  3  //input from server dim switch 1
#define Zero  2  //zero crossing signal

int Steps = 0;
boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=1250;
long time;
boolean State = Direction;   


void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);        //Pin setup
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(Dim1, INPUT);
  pinMode(Zero, INPUT_PULLUP);
  pinMode(LOCK, INPUT);
  pinMode(LIGHT, INPUT);
  EICRA |= (1 << ISC10);    // set INT0 to trigger on falling edge
  EIMSK |= (1 << INT0);     // Turns on INT0
 
  sei();
  delay(100);
 }
void loop()
//Serial.println(zero_count);
{ 
  if(State == true)// establish direction of stepper motor
  {                
    if(digitalRead(LOCK) == LOW)
    {  // rotate stepper motor
      while(steps_left>0)
      {
        currentMillis = micros();
        if(currentMillis-last_time>=1000)
        {
          stepper(1);
          time=time+micros()-last_time;
          last_time=micros();
          steps_left--;
        }
      }
      Serial.println(time);       // serial commands for debugging
      Serial.println("Wait...!");
      delay(2000);
      State = false;              // Change state, prevent from unlocking twice
      Direction=!Direction;       // Change rotation direction of stepper motor
     }
  }
  Serial.println("WTF!!");
  delay(100);
  steps_left=1250;
  if(State != true)
  {
    if(digitalRead(LOCK) == HIGH)
    {
      while(steps_left>0)
      {
        currentMillis = micros();
        if(currentMillis-last_time>=1000)
        {
          stepper(1);
          time=time+micros()-last_time;
          last_time=micros();
          steps_left--;
        }
      }
      Serial.println(time);
      Serial.println("Wait...!");
      delay(2000);
      State = true;
      Direction=!Direction;
    }
  }
  //Lighting
  if(digitalRead(LIGHT) == LOW)// When no signal is received from Server Light, keep
  {
    digitalWrite(IN5, LOW);       // Light off
    //Serial.println("OFF");
  }
  
}
void stepper(int xw)// Stepper motor stepping
{             
  for (int x=0;x<xw;x++)
  {
    switch(Steps){
      case 0:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;

      case 1:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, HIGH);
        break;

      case 2:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;

      case 3:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;

      case 4:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;

      case 5:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;

      case 6:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;

      case 7:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;

      default:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
    }
    SetDirection();
  }
}
void SetDirection(){              // Sets Direction
  if(Direction==1){Steps++;}
  if(Direction==0){Steps--;}
  if(Steps>7){Steps=0;}
  if(Steps<0){Steps=7;}
}
   
 //zero crossing delay accounts for 417us which is roughly 7 counts.
ISR (INT0_vect)
{
  if(digitalRead(LIGHT) == HIGH)
  {
    if(digitalRead(Dim1) == LOW)
    {
      digitalWrite(IN5, HIGH);
    }
    if(digitalRead(Dim1) == HIGH)
    {
      digitalWrite(IN5, LOW);
      delayMicroseconds(15600);
      digitalWrite(IN5,HIGH);
//    delayMicroseconds(10);
      digitalWrite(IN5, LOW);
     
    }
  }

} 


