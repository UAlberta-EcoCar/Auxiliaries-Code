

#include <Servo.h>

Servo myservo;
int startpos = 0;
int pos = 0;


 // initialize digital pins SIG as output and buttonpin as input.
const int right_switch = A5  ;     // the number of the pushbutton pin
const int left_switch = A4 ;     // the number of the pushbutton pin
const int driving = A3 ;     // the number of the pushbutton pin
const int haz =A2 ;     // the number of the pushbutton pin
//const int wiper_switch =A1 ;     // the number of the pushbutton pin
const int break_switch = A0 ;

const int L_SIG = 7  ;      // the number of the LED pin
const int R_SIG = 8  ;      // the number of the LED pin
const int brake_l =6;      // the number of the LED pin
const int brake_r = 5;
const int DRIVING = 2;      // the number of the LED pin

int right_state = 0;         // variable for reading the pushbutton status
int left_state = 0;         // variable for reading the pushbutton status
int driving_state = 0;         // variable for reading the pushbutton status
int haz_state = 0;         // variable for reading the pushbutton status
//int wiper_state = 0;         // variable for reading the pushbutton status
int brake_state = 0;

int stateLED_DRIVING = LOW;
int stateLED_HAZ = LOW;
int previous = LOW;
long time = 0;
long debounce = 200;



void setup() {
  pinMode(right_switch, INPUT);
  pinMode(left_switch, INPUT);
  pinMode(driving, INPUT);
  pinMode(haz, INPUT);
  pinMode(break_switch, INPUT);
  //pinMode(wiper_switch, INPUT);

  pinMode(L_SIG, OUTPUT);
  pinMode(R_SIG, OUTPUT);
  pinMode(brake_l, OUTPUT);
  pinMode(brake_r, OUTPUT);
  pinMode(DRIVING, OUTPUT);

myservo.attach(15);
}

void loop() {
  // read the state of the pushbutton value:
  right_state = digitalRead(right_switch);
  left_state = digitalRead(left_switch);
  driving_state = digitalRead(driving);
  haz_state = digitalRead(haz);
  brake_state = digitalRead(break_switch);
  //wiper_state = digitalRead(wiper_switch);

/*
  if ( wiper_state == HIGH) {
    for ( pos = 0; pos<= 90; pos +=1){
      myservo.write(pos);
      delay(15);
    }
    for (pos = 90; pos>= 0; pos -= 1){
      myservo.write(pos);
      delay(15);
    }
  }
  */
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (left_state == HIGH) {
digitalWrite(L_SIG, HIGH);   // turn the LED on (HIGH is the voltage level)
digitalWrite(brake_l, HIGH);
delay(1000);                       // wait for a second
digitalWrite(L_SIG, LOW);    // turn the LED off by making the voltage LOW
digitalWrite(brake_l, LOW);
delay(1000);                       // wait for a second
  } else {
    // turn LED off:
    digitalWrite(L_SIG, LOW);
    digitalWrite(brake_l, LOW);

    
  }
   if (right_state == HIGH) {
    // turn LED on:
  digitalWrite(R_SIG, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(brake_r, HIGH);
  delay(1000);                       // wait for a second
  digitalWrite(R_SIG, LOW);
  digitalWrite(brake_r, LOW);
  delay(1000);
  } else {
    // turn LED off:
    digitalWrite(R_SIG, LOW);
  }

  
  if (brake_state == HIGH) {
    // turn LED on:
  digitalWrite(brake_l, HIGH);
  digitalWrite(brake_r, HIGH);  // turn the LED on (HIGH is the voltage level)
  } else {
    // turn LED off:
    digitalWrite(brake_l, LOW);
    digitalWrite(brake_r, LOW);
  }

  
 driving_state = digitalRead(DRIVING);  
  if(driving_state == HIGH && previous == LOW && millis() - time > debounce) {
    if(stateLED_DRIVING == HIGH){
      stateLED_DRIVING = LOW; 
    } else {
       stateLED_DRIVING = HIGH; 
    }
    time = millis();
  previous == driving_state;
  }

  
    if(haz_state == HIGH && previous == LOW && millis() - time > debounce) {
    if(stateLED_HAZ == HIGH){
      stateLED_HAZ = LOW; 
    } else{
       stateLED_HAZ = HIGH;
       delay(1000);
       stateLED_HAZ = LOW;
       delay(1000);
       stateLED_HAZ = HIGH;    
    }
    time = millis();
  }
  digitalWrite(haz, stateLED_HAZ);
    delay(1000);                       // wait for a second

  previous == haz_state;
}
