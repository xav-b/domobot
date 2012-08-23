/*
 Light control
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. Moreover an ultrasonic sensor
 give distance of the nearest object in front of it.
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 created 28 fev 2012
 modified 29 fev 2012
 by Xavier Bruhiere with open source community help
 */

void switch_light(); 
/*void serialEvent();*/
void send_report();

#define  LED_PIN    13      // _______________ LED pin
#define  LED_PORT   PORTB
#define  LED        PB5

int state = 0;              // Light state (0 for off)
int cpt = 0;                // Cpt for counting reports sent
char input = 0;             // Input string received

void setup() {
    Serial.begin(9600);
    Serial.flush();
    pinMode(LED_PIN, OUTPUT);      
}

void loop(){ 
    /*serial_event();*/
    delay(1000);
 }

void serialEvent() {
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
}

void send_report() {
    cpt++;
    delay(100);
    Serial.print(cpt);
    Serial.print("  ");
    Serial.print(state);
    Serial.println();
}
