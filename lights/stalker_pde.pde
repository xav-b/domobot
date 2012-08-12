/*
 Light control
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. Moreover an ultrasonic sensor
 give distance of the nearest object in front of it.
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 * signal sensor attached to pin 7
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 28 fev 2012
 modified 29 fev 2012
 by Xavier Bruhiere with open source community help

 */

#define  BUTTON_PIN  2    // arduino board's pushbutton pin
#define  LED_PIN  13      // _______________ LED pin
#define  PING_PIN  7      // _______________ sensor pin
#define  PHOTO_PIN  0     // _______________ light sensor pin

int state = 0;               // light state (0 for off)
int move = 0;                // movement detection (0 for nothing)
int mode = 0;                // current mode, 0 is automatic, -1 is manual
int count = 0;
long cm = 0; 
char recv_byte = 0;

void setup() {
  // initialize i/o
  Serial.begin(9600);
  Serial.flush();
  pinMode(LED_PIN, OUTPUT);      
  pinMode(BUTTON_PIN, INPUT); 
  
  attachInterrupt (0, switch_light, RISING);
}

void loop(){ 
  int light_level = analogRead(PHOTO_PIN);
  
  if (mode == 0) {
    if ((cm = pulse()) < 10)
      move = ~move;   
    if (light_level < 600 && state != 0) {
      mode = ~mode;
      switch_light();
    }
    else if (light_level > 600 && move != state) {
      mode = ~mode;
      switch_light();
    }
  }  
  delay(400);
  
  if (Serial.available()) {
    delay(50);
    if ((recv_byte = Serial.read()) == 's') {
      Serial.println("Switching led state");
      switch_light();  
    }
    else if (recv_byte == 'r')
      print_report(1000 - light_level, cm);
  }
}

void print_report(int light_level, long distance) {
  count++;
  
  delay(100);
  Serial.print("** Report number ");
  Serial.print(count);
  Serial.println(" generated:");
  Serial.println("Sensors monitoring: ");
  Serial.print("  - Light intensity: ");
  Serial.print(light_level);
  Serial.println();
  Serial.print("  - Nearest object: ");
  Serial.print(distance);
  Serial.println("cm");
  Serial.println("Latest observation: ");
  
  Serial.print("  The Stalker system works in ");
  if (mode == 0)
    Serial.print("automatic mode and ");
  else
    Serial.print("manual mode and ");
  if (state == 0) {
    Serial.print(" lightings are turned off, while ");
    if (move == 0 && mode == 0)
      Serial.println("no one has been detected.");
    else if (move != 0 && mode == 0)
      Serial.println("daylight is comfortable.");
    else if (move != 0 && mode != 0)
      Serial.println("the user ordered it.");
  }
  else {
    Serial.print("lightings are powered, as ");
    if (move == 0 || mode != 0)
      Serial.println("the user ordered it.");
    else
      Serial.println("daylight is weak and someone is hanging around in the room.");
  }
  Serial.println("EOF **");
}

void switch_light() {
  mode = ~mode;
  PINB |= _BV(PB5);
  state = ~state;
}

long pulse(){
  long duration, inches, cm;

  pinMode(PING_PIN, OUTPUT);
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_PIN, LOW);

  pinMode(PING_PIN, INPUT);
  duration = pulseIn(PING_PIN, HIGH);

  cm = duration / 29 / 2;
  
  return cm;
}
