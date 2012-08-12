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

void switch_light(); 
void serial_event();
void send_report();
void detection();

#define  BUTTON_PIN 2       // arduino board's pushbutton pin
#define  LED_PIN    13      // _______________ LED pin
#define  LED_PORT   PORTB
#define  LED        PB5
#define  PHOTO_PIN  0       // _______________ light sensor pin
#define  DETECT_PIN 3       // _______________ movement detector pin

int state = 0;              // Light state (0 for off)
int move = 0;               // Movement detection (0 for nothing)
int mode = 0;               // 0 for automatic mode, 1 for manual
int cpt = 0;                // Cpt for counting reports sent
int light_level = 0;        // Light level detected by photoresistor
char input = 0;             // Input string received
int time = 0;               // A simple counter, reset at each detection

void setup() {
    // initialize i/o
    Serial.begin(9600);
    Serial.flush();
    pinMode(LED_PIN, OUTPUT);      
    pinMode(BUTTON_PIN, INPUT); 
    pinMode(DETECT_PIN, INPUT); 
  
    //TODO movement detectin as interrupt
    attachInterrupt (0, switch_light, RISING);
    attachInterrupt (1, detection, RISING);
}

void loop(){ 
    light_level = analogRead( PHOTO_PIN );
    serial_event();

    if ( mode == 0 ) {
        if ( light_level < 600 && state != 0 ) {
            mode = ~mode;
            switch_light();
        }
        else if ( light_level > 600 && move != state ) {
            mode = ~mode;
            switch_light();
        }
    }
    delay(500);
    time++;
    if ( time > 10 )
        LED_PORT &= _BV(LED);

 }

void serial_event() {
    while(Serial.available()) {
        delay(50);
        input = Serial.read();
        if ( input == 's' ) {
            Serial.println("switch");
            switch_light();
        }
        else if ( input == 'r' )  {
            Serial.println("report");
            send_report();
        }
    }
}

void switch_light() {
    PINB |= _BV(LED);
    state = ~state;
    mode = ~mode;
}

void detection() {
    time = 0;
    move = ~move;
}

void send_report() {
    cpt++;
    delay(100);
    Serial.println("count light move mode state");
    Serial.print(cpt);
    Serial.print("  ");
    Serial.print(light_level);
    Serial.print("  ");
    Serial.print(move);
    Serial.print("  ");
    Serial.print(mode);
    Serial.print("  ");
    Serial.print(state);
    Serial.println();
}
