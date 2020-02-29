/*
 Name:    Arduino_Home.ino
 Created: 21.06.2019 21:52:05
 Author:  kaufm
 Board:   Arduino Nano ATmega328 (Old Bootloader)
*/
#include <Streaming.h>
//#include <SoftEasyTransfer.h>
#include <SoftwareSerial.h>
#include <RH_E32.h>

/**E32**/
#define PIN_M0 1
#define PIN_M1 2
#define PIN_AX 5
#define SSRXE32 7
#define SSTXE32 6
SoftwareSerial ESerial(SSRXE32, SSTXE32);  //RX, TX
RH_E32 RHTrans(&ESerial, PIN_M0, PIN_M1, PIN_AX);
/**End E32**/

/**ESP6288**/ //(WLAN-uC)
#define SSRX 8
#define SSTX 9
SoftwareSerial WSerial(8, 9);  //f�r WLAN (ESP8266)
int Wreceived_id = -1;
String Wreceived_data = "-1";
//SoftEasyTransfer WET;
/**End ESP6288**/

unsigned long intervall1 = 100;
unsigned long prevmillis1 = 0;
unsigned long intervall2 = 1000;
unsigned long prevmillis2 = 0;
int counter = 0;





void setup() {
	Serial.begin(9600);
	WSerial.begin(9600);
	//WET.begin(details(W_RMSG), &WSerial);
	//WET.begin((byte*)&W_RMSG, sizeof(W_RMSG), &WSerial);

}
void setupFunkFunktionen()
{}

void waitForFunk()
{}

void analyseFunk()
{}

void FunksendMSG()
{}

void WetterAuswerten()
{}



void waitForWLAN()  //warte auf Serielle Kommunikation vom WLAN-Controler
{
	if (WSerial.available() > 0)
	{
		delay(10);
		char inbyte;
		inbyte = WSerial.read();
		if (inbyte == 'i')
		{
			
			Wreceived_id = WSerial.parseInt();
			Serial.println(Wreceived_id);
		}
		else if (inbyte == 'd')
		{
			Wreceived_data = WSerial.readStringUntil('|');
			Serial.println(Wreceived_data);
		}
		else if (inbyte == 'e')
		{	
			analyseWLAN();
			
		}
	}
}

void analyseWLAN()
{
	if (Wreceived_id == 1)
	{
		serialSendMSG(1, String(millis()));
		//serialSendMSG(1, String(millis()));
		//Wreceived_id = -1;
	}
}

void TelegrammNachrichtvorbereiten()
{}

void alleMessdatenAnfordern()
{}



// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() - prevmillis1 >= intervall1)
	{
		waitForWLAN();  //Warte auf Nachrichten vomn WLAN-Controller
		prevmillis1 = millis();
	}

	if (millis() - prevmillis2 >= intervall2)
	{
		Serial << "waiting " << millis() << endl;
		prevmillis2 = millis();
	}

	

}

void NachrichtAnWLAN(int id, String data)
{
	serialSendMSG(id, data);
}
void serialSendMSG(int id, String data)
{
	WSerial.print("i");
	WSerial.print(id);
	WSerial.print("d");
	WSerial.print(data);
	WSerial.print("|e");

	delay(10);

	Serial << "gesendet: " << id << ": " << data << endl;
}
