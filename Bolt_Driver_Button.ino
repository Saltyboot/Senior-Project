/*
Bolt Driver
Stepper Moter

Nick Jarvis
Matthew Schaefer 

Push Button
  */

#define IN1  8    //stepper motor A
#define IN2  9    //stepper motor B
#define IN3  10   //stepper motor C
#define IN4  11   //stepper motor D
#define IN5  12   //AC light control
#define LOCK 7    //Input from Server Lock
#define LIGHT 6   //Input from Server Light

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
  pinMode(LOCK, INPUT);
  pinMode(LIGHT, INPUT_PULLUP);
  delay(1000);
}
void loop()
{
  while(1){
    if(State == true){                // establish direction of stepper motor
      if(digitalRead(LOCK) == HIGH){  // rotate stepper motor
          while(steps_left>0){
            currentMillis = micros();        
    
            if(currentMillis-last_time>=1000){
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
    delay(100); 
    steps_left=1250;
    if(State != true){
      if(digitalRead(LOCK) == LOW){
          while(steps_left>0){
            currentMillis = micros();
    
            if(currentMillis-last_time>=1000){
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
    if(digitalRead(LIGHT) == LOW){  // When no signal is received from Server Light, keep
      digitalWrite(IN5, LOW);       // Light of
      Serial.println("OFF");
      Serial.println(digitalRead(LIGHT));
    }else{
      digitalWrite(IN5, HIGH);     // When signal is revceived from Server Light, turn on
      Serial.println("ON");        // Light
      Serial.println(digitalRead(LIGHT));
    }
  }
}
void stepper(int xw){             // Stepper motor stepping
  for (int x=0;x<xw;x++){
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
