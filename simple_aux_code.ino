



 // initialize digital pins SIG as output and buttonpin as input.
const int buttonPin1 =6 ;     // the number of the pushbutton pin
const int buttonPin2 =7 ;     // the number of the pushbutton pin
const int buttonPin3 =8 ;     // the number of the pushbutton pin
const int buttonPin4 =9 ;     // the number of the pushbutton pin
const int buttonPin5 =10 ;     // the number of the pushbutton pin

const int L_SIG =1  ;      // the number of the LED pin
const int R_SIG =2  ;      // the number of the LED pin
const int Brake =3  ;      // the number of the LED pin
const int DRIVING =4  ;      // the number of the LED pin
const int HAZ =5  ;      // the number of the LED pin
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int buttonState3 = 0;         // variable for reading the pushbutton status
int buttonState4 = 0;         // variable for reading the pushbutton status
int buttonState5 = 0;         // variable for reading the pushbutton status

int stateLED_DRIVING = LOW;
int stateLED_HAZ = LOW;
int previous = LOW;
long time = 0;
long debounce = 200;



void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);

  pinMode(L_SIG, OUTPUT);
  pinMode(R_SIG, OUTPUT);
  pinMode(Brake, OUTPUT);
  pinMode(DRIVING, OUTPUT);
  pinMode(Brake, OUTPUT);

}

void loop() {
  // read the state of the pushbutton value:
  buttonState2 = digitalRead(buttonPin2);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState2 == HIGH) {
digitalWrite(L_SIG, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(1000);                       // wait for a second
digitalWrite(L_SIG, LOW);    // turn the LED off by making the voltage LOW
delay(1000);                       // wait for a second
  } else {
    // turn LED off:
    digitalWrite(L_SIG, LOW);

    
  }
   if (buttonState3 == HIGH) {
    // turn LED on:
  digitalWrite(R_SIG, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(R_SIG, LOW);
  delay(1000);
  } else {
    // turn LED off:
    digitalWrite(R_SIG, LOW);
  }

  
  if (buttonState4 == HIGH) {
    // turn LED on:
  digitalWrite(Brake, HIGH);   // turn the LED on (HIGH is the voltage level)
  } else {
    // turn LED off:
    digitalWrite(Brake, LOW);
  }

  
 buttonState1 = digitalRead(DRIVING);  
  if(buttonState1 == HIGH && previous == LOW && millis() - time > debounce) {
    if(stateLED_DRIVING == HIGH){
      stateLED_DRIVING = LOW; 
    } else {
       stateLED_DRIVING = HIGH; 
    }
    time = millis();
  previous == buttonState1;
  }

  
    if(buttonState5 == HIGH && previous == LOW && millis() - time > debounce) {
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
  digitalWrite(HAZ, stateLED_HAZ);
    delay(1000);                       // wait for a second

  previous == buttonState5;
}
