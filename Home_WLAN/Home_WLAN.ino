/*
  Name:    WLAN_Home.ino
  Created: 22.06.2019 13:08:51
  Author:  kaufm
  Board:   NodeMCU 1.0 (ESP-12E Module)
*/
#include <CTBotInlineKeyboard.h>
#include <CTBotDataStructures.h>
#include <CTBot.h>
#include <SoftwareSerial.h>
//#include <SoftEasyTransfer.h>
#include <Streaming.h>
#include "network_secrets.h"



/**WLAN**/
const char* ssid  = SECRET_SSID;
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
//SoftEasyTransfer AET;
/**End Arduino**/




String inid;
String indata;
char incomming;

unsigned long intervall1 = 100;  //für Arduino Nano Serial
unsigned long prevmillis1 = 0;
unsigned long intervall2 = 1000;  //für Telegramabfrage
unsigned long prevmillis2 = 0;

struct MSG_STRUCT
{
  int id;
  unsigned int data;
};

MSG_STRUCT A_RMSG;
MSG_STRUCT A_TMSG;


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


void serialSendMSG(MSG_STRUCT *MSG)
{
  ASerial.print("i");
  ASerial.print(MSG -> id);
  ASerial.print("d");
  ASerial.print(MSG -> data);
  ASerial.print("e");

  Serial << "gesendet: " << MSG -> id << ": " << MSG -> data << endl;
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
      A_RMSG.id = ASerial.parseInt();
    }
    else if (inbyte == 'd')
    {
      A_RMSG.data = ASerial.parseInt();
    }
    else if (inbyte == 'e')
    {
      Serial << "Zahl: " << A_RMSG.id << ": " << A_RMSG.data << endl;
      //A_TMSG.id = A_RMSG.id + 1;
      //A_TMSG.data = A_RMSG.data + 1;
      //serialSendMSG(&A_TMSG);
      analyseSerial();
    }
  }
}

void analyseSerial()
{}


void waitForTelegram()
{
	if (bot.getNewMessage(msg))
	{
		// ...forward it to the sender
		bot.sendMessage(msg.sender.id, msg.text);
		Serial << "Empfangen: " << msg.date << " von " << msg.sender.firstName << " (" << msg.sender.id << ")" << " : " << msg.text << endl;
		Serial.println("-------------------------");

		msgtext = msg.text;
		msgsenderid = msg.sender.id;
		if (msgtext == "?")
		{
			
		}
		

	}
}

void TelegramSendMSG()
{}
