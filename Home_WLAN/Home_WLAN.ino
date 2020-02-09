/*
  Name:    WLAN_Home.ino
  Created: 22.06.2019 13:08:51
  Author:  kaufm
  Board:   NodeMCU 1.0 (ESP-12E Module)
*/
#include <Arduino.h>
#include <CTBotInlineKeyboard.h>
#include <CTBotDataStructures.h>
#include <CTBot.h>
#include <SoftwareSerial.h>
//#include <SoftEasyTransfer.h>
#include <Streaming.h>
#include "network_secrets.h"


/**WLAN**/
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;
/**End WLAN**/

/**Telegram**/
//Initialize Telegram BOT
CTBot bot;
String token = SECRET_BOT_TOKEN;
//Nessesary variables
TBMessage msg;
String msgtext;
uint64_t msgsenderid;
String gebastel;
//Intervall
long millizyklen = -1;
bool millizyklenset = false;
unsigned long telegram_intervall = 500;
unsigned long telegram_prevmillis = 0;
/**End Telegram**/

/**Arduino**/
#define SSRX D3
#define SSTX D4
SoftwareSerial ASerial(SSRX, SSTX);  //kommt von/geht an Arduino Nano
int Areceived_id = -1;
String Areceived_data = "-1";
//SoftEasyTransfer AET;

int id_armsg[20];
String data_armsg[20];
/**End Arduino**/






char incomming;

unsigned long intervall1 = 100;  //für Arduino Nano Serial
unsigned long prevmillis1 = 0;
unsigned long intervall2 = 1000;  //für Telegramabfrage
unsigned long prevmillis2 = 0;





// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	ASerial.begin(9600);
	//AET.begin(details(A_RMSG), &ASerial);

	int versuche = 0;
	while (!connectWLAN() && versuche < 60)
	{
		versuche++;
		Serial.print(".");
		delay(500);
	}
	if (versuche > 60)
	{
		Serial.println("");
		Serial.println("WiFi failed");
	}



}

// the loop function runs over and over again until power down or reset
void loop() {

	if (millis() - prevmillis1 >= intervall1)
	{
		waitForSerial();
		prevmillis1 = millis();
	}

	if (millis() - prevmillis2 >= intervall2)
	{
		waitForTelegram();
		prevmillis2 = millis();
	}



}

bool connectWLAN()
{
	Serial.print("Connecting Wifi: ");

	// connect the ESP8266 to the desired access point
	bot.wifiConnect(ssid, pass);

	// set the telegram bot token
	bot.setTelegramToken(token);

	// check if all things are ok
	if (bot.testConnection())
	{
		Serial.println("\ntestConnection OK");
		return true;
	}
	else
	{
		return false;
	}
}


void serialSendMSG(int id, String data)
{
	ASerial.print("i");
	ASerial.print(id);
	ASerial.print("d");
	ASerial.print(data);
	ASerial.print("e");

	Serial << "gesendet: " << id << ": " << data << endl;
}

void NTPAbrufen()
{}

void WetterAbrufen()
{}

void waitForSerial()
{
	if (ASerial.available() > 0)
	{
		char inbyte;
		inbyte = ASerial.read();
		if (inbyte == 'i')
		{
			Areceived_id = ASerial.parseInt();
		}
		else if (inbyte == 'd')
		{
			Areceived_data = ASerial.readString();
		}
		else if (inbyte == 'e')
		{
			analyseSerial();
		}
	}


}

void analyseSerial()
{
	if (Areceived_id == 1)
	{
		// mach irgendwas mit data
	}
}


void waitForTelegram()
{
	if (bot.getNewMessage(msg))
	{
		// ...forward it to the sender
		//bot.sendMessage(msg.sender.id, msg.text);
		Serial << "Empfangen: " << msg.date << " von " << msg.sender.firstName << " (" << msg.sender.id << ")" << " : " << msg.text << endl;
		Serial.println("-------------------------");

		msgtext = msg.text;
		msgsenderid = msg.sender.id;

		if (msg.messageType == CTBotMessageText)
		{
			if (msgtext == "?") //Hilfenachricht senden
			{
				telegramPrintHelp(msgsenderid);
			}
			if (msgtext == "alive")
			{
				telegramPrintAlive(msgsenderid);
			}
			if (msgtext == "Hallo")
			{
				CTBotInlineKeyboard kb1;
				kb1.addButton("Button1", "kb1_b1", CTBotKeyboardButtonQuery);
				kb1.addButton("Button2", "kb1_b2", CTBotKeyboardButtonQuery);
				kb1.addButton("Button3", "kb1_b3", CTBotKeyboardButtonQuery);
				kb1.addRow();
				kb1.addButton("Button4", "kb1_b4", CTBotKeyboardButtonQuery);
				kb1.addButton("Button5", "kb1_b5", CTBotKeyboardButtonQuery);
				kb1.addButton("Button6", "kb1_b6", CTBotKeyboardButtonQuery);
				bot.sendMessage(msgsenderid, "Was kann ich für Dich tun?\nhm?", kb1);
			}
		}
		if (msg.messageType == CTBotMessageQuery)
		{
			if (msg.callbackQueryData == "kb1_b1")
			{
				bot.sendMessage(msgsenderid, "Du hast Button 1 gedrückt, toll.");
				msg.callbackQueryData = "-1";
			}
		}


	}
	else
		Serial << "waiting " << millis() << endl; //debugging
}

void telegramPrintHelp(uint64_t senderid)
{
	gebastel = "Hilfe-Menu:\n\n";
	gebastel += "Hier koennte eine gut beschriebene und benutzfreundliche Anleitung zur Verwendung des Bots ihren Platz finden. Jedoch hat der Programmierer noch nicht die Zeit gefunden, um lange und ueberschwingliche Texte zu implementieren. \n";
	bot.sendMessage(senderid, gebastel);
}
void telegramPrintAlive(uint64_t senderid)
{
	unsigned long mil = millis();
	int sekunden = mil / (1000);
	int minuten = sekunden / 60;
	int stunden = minuten / 60;
	int tage = (stunden / 24) + millizyklen * 48;

	gebastel = "ESP laeuft seit ";
	if (sekunden < 120)
	{
		gebastel += sekunden;
		gebastel += " Sekunden.  \n";
	}
	else if (minuten < 120)
	{
		gebastel += minuten;
		gebastel += " Minuten.  \n";
	}
	else if (stunden < 48)
	{
		gebastel += stunden;
		gebastel += " Stunden.  \n";
	}
	else if (tage < 65000)
	{
		gebastel += sekunden;
		gebastel += " Tagen.  \n";
	}

	Serial.println("Alive1/3");//

	delay(5);
	serialSendMSG(1, "42");
	delay(5);
	int timeout = 0;
	while ((!(Areceived_id == 101 || Areceived_id == 102 || Areceived_id == 103 || Areceived_id == 104)) && timeout < 1000)
	{
		waitForSerial();
		timeout++;
		delay(1);
	}
	Serial.println("Alive2/3");//
	if (timeout < 1000)  //Antwort erhalten
	{
		if (Areceived_id == 101)
		{
			gebastel += "Arduino laeuft seit ";
			gebastel += Areceived_data;
			gebastel += " Sekunden.";
		}
		if (Areceived_id == 102)
		{
			gebastel += "Arduino laeuft seit ";
			gebastel += Areceived_data;
			gebastel += " Minuten.";
		}
		if (Areceived_id == 103)
		{
			gebastel += "Arduino laeuft seit ";
			gebastel += Areceived_data;
			gebastel += " Stunden.";
		}
		if (Areceived_id == 104)
		{
			gebastel += "Arduino laeuft seit ";
			gebastel += Areceived_data;
			gebastel += " Tagen.";
		}
		bot.sendMessage(senderid, gebastel);
		Serial.println(gebastel);//debug
		Serial.println("Alive3/3");//debug
	}
	else
	{
		gebastel += "Arduino reagiert nicht.";
		Serial.println(gebastel);//debug
		bot.sendMessage(senderid, gebastel);
	}
}
