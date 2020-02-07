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
SoftwareSerial WSerial(8, 9);  //für WLAN (ESP8266)
//SoftEasyTransfer WET;
/**End ESP6288**/

unsigned long intervall = 1000;
unsigned long prevmillis = 0;
int counter = 0;

struct MSG_STRUCT
{
	int id;
	unsigned int data;
};

MSG_STRUCT W_RMSG;



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

void WetterAuswerten()
{}

void NachrichtAnWLAN(int id, int data)
{
	WSerial.print("i");
	WSerial.print(id);
	WSerial.print("d");
	WSerial.print(data);
	WSerial.print("e");

	Serial << "gesendet: " << id << ": " << data << endl;
}

void waitForWLAN()
{
	if (WSerial.available() > 0)
	{
		char inbyte;
		inbyte = WSerial.read();
		if (inbyte == 'i')
		{
			W_RMSG.id = WSerial.parseInt();
		}
		else if (inbyte == 'd')
		{
			W_RMSG.data = WSerial.parseInt();
		}
		else if (inbyte == 'e')
		{
			Serial << "Zahl: " << W_RMSG.id << ": " << W_RMSG.data << endl;
			analyseWLAN();

		}
	}
}

void analyseWLAN()
{}

void TelegrammNachrichtvorbereiten()
{}

void alleMessdatenAnfordern()
{}



// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() - prevmillis >= intervall)
	{
		W_TMSG.id = ++counter;
		W_TMSG.data = millis();
		//WET.sendData();


		/*
		WSerial.print("i");
		WSerial.print(W_RMSG.id);
		WSerial.print("d");
		WSerial.print(W_RMSG.data);
		WSerial.print("e");
		*/
		//serialSendMSG(&W_TMSG);
		NachrichtanWLAN(++counter, millis());


		//WSerial.println("hallo");

		prevmillis = millis();
	}

	waitForWLAN();

}

void serialSendMSG(MSG_STRUCT *MSG)
{
	WSerial.print("i");
	WSerial.print(MSG->id);
	WSerial.print("d");
	WSerial.print(MSG->data);
	WSerial.print("e");

	Serial << "gesendet: " << MSG->id << ": " << MSG->data << endl;
}