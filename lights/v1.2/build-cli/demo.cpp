#include <Arduino.h>
#include <SoftwareSerial/SoftwareSerial.cpp>   //Software Serial Port
#define RxD 7 //2, 3
#define TxD 6
 
#define DEBUG_ENABLED 1

SoftwareSerial blueToothSerial(RxD,TxD);

void setupBTConnection()
{
    Serial.println("Setting up Bluetooth link");    // For debugging, Comment this line if not required
    blueToothSerial.begin(38400);                   // Set BluetoothBee BaudRate to default baud rate 38400
    blueToothSerial.print("\r\n+STWMOD=0\r\n");     // 0: slave, 1: master
    blueToothSerial.print("\r\n+STNA=seeedBT\r\n"); // device name
    /*blueToothSerial.print("\r\n+STPIN=0000\r\n");   // set pincode*/
    blueToothSerial.print("\r\n+STOAUT=1\r\n");     // 0: paired autoconnect forbid, 1: allowed
    blueToothSerial.print("\r\n+STAUTO=0\r\n");     // 0: autoconnect forbid, 1: allowed
    delay(2000);                                    // This delay is required.
    blueToothSerial.print("\r\n+INQ=1\r\n");
    Serial.println("Slave bluetooth is inquirable");
    delay(2000); // This delay is required.
    blueToothSerial.flush();
}
 
void setup() 
{ 
    Serial.begin(9600);          //Serial port for debugging, Comment this line if not required  
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBTConnection();
} 
 
void loop()
{
    char recvChar;
    while(1){
        if(blueToothSerial.available()){//check if there's any data sent from the remote bluetooth shield
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
        }
        if(Serial.available()){//check if there's any data sent from the local serial terminal, you can add the other applications here
            recvChar  = Serial.read();
            blueToothSerial.print(recvChar);
        }
    }
}

/*
void loop() 
{ 
    if(Serial.available())//check if there's any data sent from the local serial terminal, you can add the other applications here
    {
        blueToothSerial.print((unsigned char)Serial.read());
    }  
    else  if(blueToothSerial.available())//check if there's any data sent from the remote bluetooth shield
    {
        Serial.print((unsigned char)blueToothSerial.read());
    }  

    if(blueToothSerial.read() == 'a')
    {
        blueToothSerial.println("You are connected to Bluetooth Bee");
        //You can write you BT communication logic here
    }
    
}*/ 
