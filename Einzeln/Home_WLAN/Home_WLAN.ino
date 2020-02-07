/*
  Name:    WLAN_Home.ino
  Created: 22.06.2019 13:08:51
  Author:  kaufm
  Board:   NodeMCU 1.0 (ESP-12E Module)
*/
#include <SoftwareSerial.h>
//#include <SoftEasyTransfer.h>
#include <Streaming.h>
#include "network_secrets"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>

/**WLAN**/
const char* ssid  = SECRET_SSID;
const char* password = SECRET_PASS;
/**End WLAN**/

/**Telegram**/
// Initialize Telegram BOT
const char BotToken[] = SECRET_BOT_TOKEN;
WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);
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

  if(connectWLAN())
    connectTelegram();
  

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
    prevmillis1 = millis();
  }



}

bool connectWLAN()
{
  Serial.print("Connecting Wifi: ");
  WiFi.begin(ssid, password);
  int versuche = 0;
  while ((WiFi.status() != WL_CONNECTED) && versuche < 60) {
    delay(500);
    Serial.print(".");
    versuche++;
  }
  if (versuche < 60)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    return true;
  }
  else
  {
    Serial.println("");
    Serial.println("WiFi failed");
  }
  return false;
}

void connectTelegram()
{
  bot.begin();
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
  message m = bot.getUpdates(); // Read new messages
  if ( m.chat_id != 0 ) { // Checks if there are some updates
    Serial.println(m.text);
    bot.sendMessage(m.chat_id, "OK");  // Reply to the same chat with "OK"
  } else {
    //keine neuen Nachrichten
  }
}

void TelegramSendMSG()
{}
