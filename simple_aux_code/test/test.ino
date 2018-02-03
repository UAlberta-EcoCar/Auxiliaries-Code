
bool right_state = LOW;
bool left_state = LOW;
bool driving_state = LOW;
bool haz_state = LOW;
bool wiper_state = LOW;
bool brake_state = LOW;
bool driving_previous = LOW;

const int driving = 2;
const int pin2 = 3;
const int pin3 = 4;
const int brake_r = 5;
const int brake_l = 6;
const int left = 7;
const int right = 8;
const int brake_switch = A0;
const int wiper_switch = A1;
const int haz_switch = A2;
const int driving_switch = A3;
const int left_switch = A4;
const int right_switch = A5;

int debounce = 50;

long time = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(driving, OUTPUT);
pinMode(pin2, OUTPUT);
pinMode(pin3, OUTPUT);
pinMode(brake_r, OUTPUT);
pinMode(brake_l, OUTPUT);
pinMode(left, OUTPUT);
pinMode(right, OUTPUT);
pinMode(right_switch, INPUT);
pinMode(left_switch, INPUT);
pinMode(driving_switch, INPUT);
pinMode(haz_switch, INPUT);
pinMode(wiper_switch, INPUT);
pinMode(brake_switch, INPUT_PULLUP);


}

void loop() {
right_state = digitalRead(right_switch);
  left_state = digitalRead(left_switch);
  driving_state = digitalRead(driving_switch);
  haz_state = digitalRead(haz_switch);
  brake_state = digitalRead(brake_switch);

if( right_state == HIGH ){
  digitalWrite(right, HIGH);
  digitalWrite(brake_r, HIGH);
  delay(100);
  digitalWrite(right,LOW);
  digitalWrite(brake_r,LOW);
  delay(100);
}

if( left_state == HIGH ){
  digitalWrite(left, HIGH);
  digitalWrite(brake_l, HIGH);
  delay(100);
  digitalWrite(left,LOW);
  digitalWrite(brake_l,LOW);
  delay(100);
}

if( brake_state == LOW ){
  digitalWrite(brake_r, HIGH);
  digitalWrite(brake_l, HIGH);
}else{
 digitalWrite(brake_r, LOW);
 digitalWrite(brake_l, LOW);
}
  if(driving_state == HIGH && driving_previous == LOW && millis() - time > debounce) {
    if(driving == HIGH){
      digitalWrite(driving,LOW); 
    } else {
       digitalWrite(driving,HIGH); 
    }
    time = millis();
  driving_previous == driving_state;
  }

if( haz_state == HIGH ){
  digitalWrite(brake_r, HIGH);
  digitalWrite(brake_l, HIGH);
  digitalWrite(right,HIGH);
  digitalWrite(left,HIGH);
  delay(100);
  digitalWrite(brake_r, LOW);
  digitalWrite(brake_l, LOW);
  digitalWrite(right,LOW);
  digitalWrite(left,LOW);
  delay(100);
}
}
