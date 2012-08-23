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

void detected();
void switchLight(); 
void sendReport();
float getDegreeTemperature(int pin);

#define LED_PIN    13      // _______________ LED pin
#define LED_PORT   PORTB
#define LED        PB5
#define PHOTO_PIN  0
#define TEMP_PIN   1
#define DETECT_PIN 2 

int present = 0;        // Someone present in the room (-1) or not (0)
int on = 0;             // Light on (0 off, -1 otherwise)
int automatic = 0;      // Automatic (-1) or manual mode (0)
double cpt = 0;            // Cpt for counting reports sent
int supervision = 0;         // If set to 0, send every loop a supervision through serial comm'
//TODO In pure C, replace with timers and overflow interruptions
double timer = 0;       // Used for inactivity detection;
int timerThreshold = 30;  // Threshold time without movement in automatic mode to switch off lights
int lightThreshold = 600; // Threshold to evaluate switch on/off need in automatic mode
double delayStep = 1000;
/*char input = 0;       // Input string received*/

void setup() {
    Serial.begin(9600);
    Serial.flush();
    pinMode(LED_PIN, OUTPUT);      
    pinMode(DETECT_PIN, INPUT);

    attachInterrupt(0, detected, RISING);
}

void loop(){ 
    /*
     * Wait for update supervision and Timer
     * Most of the work is achieved within interruptions
     */
    delay(delayStep);
    timer--;
    if ( timer < 0 ) {
        timer = timerThreshold;
        present = 0;
        if ( on && automatic )
            switchLight();
    }
    if ( supervision )
        sendReport();
 }

void serialEvent() {
    while(Serial.available()) {
        delay(50);
        char  input = Serial.read();
        if ( input == 's' ) {
            automatic = 0;
            Serial.println("switch");
            switchLight();
        }
        else if ( input == 'r' )  {
            Serial.println("report");
            sendReport();
        }
        else if ( input == 'v' )  {
            Serial.println("supervision");
            supervision = ~supervision;
        }
        else if ( input == 'a' )  {
            Serial.println("automatic");
            automatic = ~automatic;
        }
        else if ( input == 'm' )  {
            Serial.println("lightElevate");
            lightThreshold += 10;
        }
        else if ( input == 'l' )  {
            Serial.println("lightLower");
            lightThreshold -= 10;
        }
        else if ( input == 'u' )  {
            Serial.println("timerElevate");
            timerThreshold += 30;
        }
        else if ( input == 't' )  {
            Serial.println("timerLower");
            timerThreshold -= 30;
        }
        else if ( input == 'e' )  {
            Serial.println("delayElevate");
            delayStep += 1000;
        }
        else if ( input == 'd' )  {
            Serial.println("delayLower");
            delayStep -= 1000;
        }
        else
            Serial.println("KO");
    }
}

void detected() {
    timer = timerThreshold;
    if ( !present ) {
        present = ~present;
        Serial.println("movement");
    }
    if ( automatic ) {
        int lightLevel = analogRead( PHOTO_PIN );
        if ( ((lightLevel > lightThreshold) && on) ||
                (lightLevel < lightThreshold && !on) )
            switchLight();
    }
}

void switchLight() {
    PINB |= _BV(LED);
    on = ~on;
}

void sendReport() {
    cpt++;
    int lightLevel = analogRead( PHOTO_PIN );
    float temperature = getDegreeTemperature( TEMP_PIN );
    delay(100);
    Serial.print(cpt); Serial.print("  ");
    Serial.print(timer); Serial.print("  ");
    Serial.print(temperature); Serial.print("  ");
    Serial.print(lightLevel); Serial.print("  ");
    Serial.print(automatic); Serial.print("  ");
    Serial.print(present); Serial.print("  ");
    Serial.print(supervision); Serial.print("  ");
    Serial.print(on); Serial.print("  ");
    Serial.print(lightThreshold); Serial.print("  ");
    Serial.print(timerThreshold); Serial.print("  ");
    Serial.print(delayStep);
    Serial.println();
}

float getDegreeTemperature(int pin) {
    /*return ((analogRead(pin) * .004882814) - 0.5) * 100;*/
    /*return (analogRead(pin) * 0.4882812);*/
    int tempRaw = analogRead( pin );
    float volt = tempRaw * 5.0;
    volt /= 1024.0;
    float tempC = (volt - 0.5) * 100;
    return tempC;
}
