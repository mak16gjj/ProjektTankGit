/*
    Name:       E32_ESP8266_RangeTestSendWLAN.ino
    Created:	08.05.2019 21:28:16
    Author:     DESKTOP-8FDJO8V\kaufm
*/

/*
*	Sender ist der zu bewegende Teil

		E32		Arduino				ESP8266
		M0		4					0
		M1		5					5
		RXD		3 (TX), 5kOhm		6 (TX), 5kOhm
		TXD		2 (RX), 5kOhm		7 (RX), 5kOhm
		AUX		6					8
		VCC		5V					5V
		GND		GND					GND

*/
#include <Streaming.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include "EBYTE.h"

#define ESP
//#define Arduino
#ifdef Arduino
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6
SoftwareSerial ESerial(2, 3);  //RX, TX mit 5 kOhm
#endif
#ifdef ESP
#define PIN_M0 0
#define PIN_M1 5
#define PIN_AX 8
SoftwareSerial ESerial(7, 6);  //RX, TX mit 5 kOhm
#endif // ESP


struct DATA {
	int eigeneZahl;
	int fremdeZahl;
};

DATA myDataS;

DATA myDataR;

unsigned long sendeprevmillis = 0;
unsigned long sendeIntervall = 2000;
int taskcounter = -1;  //0 = Senden/Empfangen  //1 = WLAN  //
int taskcounteralt = 1;
unsigned int timeout = 1800;
unsigned long lastSend = 0;
unsigned long lastRec = 0;


bool empfangen = false;


EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//Adafruit_SSD1306 display(4);

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.display();

	ESerial.begin(9600);

	Serial.println("Starting Sender");

	
	Transceiver.init();/*
	Transceiver.SetTransmitPower(OPT_TP30);


	Transceiver.SetMode(MODE_NORMAL);
	Transceiver.Reset();


	Transceiver.SetUARTBaudRate(UDR_9600);
	Transceiver.SetAirDataRate(ADR_2K);
	//Transceiver.SetAddressH(0);
	//Transceiver.SetAddressL(0);
	Transceiver.SetChannel(5);
	Transceiver.SetOptTo70();
	Transceiver.SaveParameters(PERMANENT);

	Transceiver.PrintParameters();
	*/

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	Serial.println("Setup done");


	myDataR.eigeneZahl = -1;
	myDataR.fremdeZahl = -1;
	myDataS.eigeneZahl = -1;
	myDataS.fremdeZahl = -1;
}

void loop() {
	
	
	if (millis() - sendeprevmillis >= sendeIntervall)
	{
		taskcounter++;
		if (taskcounter > 1)
			taskcounter = 0;

		taskcounteralt = 0;

		sendeprevmillis = millis();
	}
	if (taskcounter == 0)
	{
		/*
		if (taskcounter != taskcounteralt)
		{
			
			myDataS.eigeneZahl = random(0, 10001);
			// Transceiver.SendByte('A');
			Transceiver.SendStruct(&myDataS, sizeof(myDataS));
			Serial << "Sending: " << myDataS.eigeneZahl << endl;

			display.clearDisplay();
			display.setCursor(0,0);
			display << "sending " << myDataS.eigeneZahl << endl;
			display.display();

			lastSend = millis();
			taskcounteralt = taskcounter;
		}

		if (ESerial.available())
		{
			Transceiver.GetStruct(&myDataR, sizeof(myDataR));
			lastRec = millis();
			Serial << "Fremd: " << myDataR.fremdeZahl << " \tEigen: " << myDataR.eigeneZahl;
			display << "received " << myDataR.fremdeZahl << endl;
			display.display();
			empfangen = true;
		}
		if (lastRec - lastSend < timeout)
		{
			if (empfangen)
			{
				empfangen = false;

				if (myDataR.fremdeZahl == myDataS.eigeneZahl)
				{

					digitalWrite(LED_BUILTIN, 1);
					Serial << "\tGUT";
					Serial << "\tAirtime: " << lastRec - lastSend << endl;
				}

			}
		}
		else
		{
			digitalWrite(LED_BUILTIN, 0);
			Serial << "TIMEOUT" << endl;
		}
		*/
	}
	if (taskcounter == 1)
	{
		if (taskcounter != taskcounteralt)
		{
			display.clearDisplay();
			display.setTextSize(1);
			display.setTextColor(WHITE);
			display.setCursor(0, 10);



			Serial.println("scan WLAN");

			display.print("scan WLAN ");
			display.display();



			// WiFi.scanNetworks will return the number of networks found
			int n = WiFi.scanNetworks(false, false);
			Serial.println("scan done");
			if (n == 0) {
				Serial.println("no networks found");
				display.println("-> 0 found");
				display.display();
			}
			else {
				Serial.print(n);
				Serial.println(" networks found");
				display << "-> " << n << " found" << endl;
				display.display();
				for (int i = 0; i < n; ++i) {
					// Print SSID and RSSI for each network found
					Serial.print(i + 1);
					Serial.print(": ");
					Serial.print(WiFi.SSID(i));
					Serial.print(" (");
					Serial.print(WiFi.RSSI(i));
					Serial.print(")");
					Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");

					display.print(i + 1);
					display.print(": ");
					display.print(WiFi.SSID(i));
					display.print(" (");
					display.print(WiFi.RSSI(i));
					display.print(")");
					display.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
					display.display();

					delay(10);
				}
			}
			Serial.println("");
			taskcounteralt = taskcounter;
		}
	}

}
