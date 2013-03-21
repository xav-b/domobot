#include <SoftwareSerial/SoftwareSerial.cpp>   //Software Serial Port
#define RxD 6  //6 and 7
#define TxD 7
 
#define DEBUG_ENABLED  1
 
SoftwareSerial blueToothSerial(RxD,TxD);
 
//Checks if the response "OK" is received
void CheckOK()
{
    char a,b;
    while(1)
    {
        if(blueToothSerial.available())
        {
            a = blueToothSerial.read();
            if('O' == a)
            {
                // Wait for next character K. available() is required in some cases, as K is not immediately available.
                while(blueToothSerial.available()) 
                {
                    b = blueToothSerial.read();
                    //Serial.println(b);
                    break;
                }
                if('K' == b)
                {
                    break;
                }
            }
        }
    }
    //Serial.print("Ok");
    while( (a = blueToothSerial.read()) != -1)
    {
        //Wait until all other response chars are received
    }
}
 
void sendBlueToothCommand(char command[])
{
    blueToothSerial.print(command);
    CheckOK();   
}
 
void setupBlueToothConnection()
{
    blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
    // blueToothSerial.begin(115200); //Set BluetoothBee BaudRate
    delay(1000);
    sendBlueToothCommand("\r\n+STWMOD=0\r\n");
    sendBlueToothCommand("\r\n+STNA=AP_BB\r\n");
    sendBlueToothCommand("\r\n+STAUTO=0\r\n");
    sendBlueToothCommand("\r\n+STOAUT=1\r\n");
    sendBlueToothCommand("\r\n +STPIN=0000\r\n");
    delay(2000); // This delay is required.
    sendBlueToothCommand("\r\n+INQ=1\r\n");
    delay(2000); // This delay is required.
}

void setup() 
{ 
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBlueToothConnection();
 
} 
 
void loop() 
{ 
    //Typical Bluetoth command - response simulation:

    //Type 'a' from PC Bluetooth Serial Terminal
    //See Bluetooth Bee - Wiki for instructions

    if(blueToothSerial.read() == 'a')
    {
        blueToothSerial.println("You are connected");
        //You can write you BT communication logic here
    }
} 
