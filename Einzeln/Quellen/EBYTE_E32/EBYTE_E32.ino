/*
    Name:       EBYTE_E32.ino
    Created:	16.04.2019 20:47:31
    Author:     DESKTOP-8FDJO8V\kaufm
*/
/*

		E32		Arduino				ESP8266
		M0		4					0
		M1		5					5
		RXD		3 (TX), 5kOhm		6 (TX), 5kOhm
		TXD		2 (RX), 5kOhm		7 (RX), 5kOhm
		AUX		6					8
		VCC		5V					5V
		GND		GND					GND
*/

#include <SoftwareSerial.h>
#include <RH_E32.h>
//#include <EBYTE.h>


#define PIN_M0 D1
#define PIN_M1 D2
#define PIN_AX D5


struct DATA {
	int eigeneZahl;
	int fremdeZahl;
};

DATA myDataS;
DATA myDataR;


unsigned long lastSend = 0;
unsigned long lastRec = 0;

unsigned long prevmillis = 0;
unsigned long intervall = 500;
int counter = 0;

SoftwareSerial ESerial(D7, D6);  //RX, TX

//EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

RH_E32 RHTrans(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup()
{
	


	Serial.begin(9600);

	ESerial.begin(9600);

	Serial.println("Starting Echo-Sender");

	//Transceiver.init();
	if (!RHTrans.init())
		Serial.println("init failed");
	else
		Serial.println("init success");
	


	//Transceiver.SetMode(MODE_NORMAL);

	Serial.println("A");
	delay(500);

	//Transceiver.Reset();

	Serial.println("B");
	delay(500);

	//Transceiver.SetWORTIming(OPT_WAKEUP250);

	Serial.println("C");
	delay(500);

	//Transceiver.SetUARTBaudRate(UDR_9600);

	Serial.println("D");
	delay(500);

	//Hier die Luft-Übertragungsgeschwindigkeit einstellen (muss bei beiden übereinstimmen)
	//Transceiver.SetAirDataRate(ADR_2K);
	/*
	* ADR_1K für 1 kbaud (nicht benutzen, zu langsam)
	* ADR_2K für 1 kbaud (funktioniert)
	* ADR_5K für 1 kbaud
	* ADR_8K für 1 kbaud
	* ADR_10K für 1 kbaud
	* ADR_15K für 1 kbaud
	* ADR_20K für 1 kbaud
	*/

	Serial.println("E");
	delay(500);

	//Transceiver.SetChannel(6);

	Serial.println("F");
	delay(500);

	//Transceiver.SetOptTo70();

	Serial.println("G");
	delay(500);

	//Hier die Sendeleistung einstellen
	//Transceiver.SetTransmitPower(OPT_TP24);
	/*
	* OPT_TP21 für 21 dB (= 125 mW)
	* OPT_TP24 für 24 dB (= 250 mW)
	* OPT_TP27 für 27 dB (= 500 mW)
	* OPT_TP30 für 30 dB (= 1000 mW)
	*/

	Serial.println("H");
	delay(500);

	//Transceiver.SaveParameters(PERMANENT);

	Serial.println("I");
	delay(500);

	//Transceiver.PrintParameters();

	myDataR.eigeneZahl = -1;
	myDataR.fremdeZahl = -1;
	myDataS.eigeneZahl = -1;
	myDataS.fremdeZahl = -1;

}

void loop()
{

	if (millis() - prevmillis >= intervall)
	{
		
		Serial.print("J");
		delay(500);
		Serial.println(counter++);
		prevmillis = millis();
	}
}
