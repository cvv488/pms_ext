/*===================================================================
  Blynk
==================================================================*/
#ifndef NET_H
#define NET_H

/* Fill in information from Blynk Device Info here */
/* Read more: https://bit.ly/BlynkSimpleAuth */
#define BLYNK_TEMPLATE_ID "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME "Device"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include "cvv_include.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// char auth[] = "2ef874cba4a84f16ad35f29b5861319d";	//UNO in Pasp1 prj
char auth[] = "10e0630e1b434d59ba8fff9b865fcc2b"; //ESP2 !
//#include "ota.h"

//PMS7003
#define  VIRT_PMS_C V10 //count
#define  VIRT_PMS_3	V11 //03um
#define  VIRT_PMS_5 V12	//05um
#define  VIRT_PMS_10 V13 //10um
#define  VIRT_PMS_25 V14 //25um
// #define  VIRT_PMS_50	V15 //50um
// #define  VIRT_PMS_100	V16 //100um


//--- V31 max

WidgetTerminal terminal(V0);
// #include <WidgetRTC.h>
// WidgetRTC rtc;

// void StringDT()  //rtc to gss
//  {
//    gss += String(day(), DEC) + "-" + String(month(), DEC) + "-" + String(year(), DEC) + "_";
//    gss += String(hour(), DEC) + ":";
//    gss += String(minute(), DEC) + ":";
//    gss += String(second(), DEC) + " ";
//  }

void TerminalP(String msg)
{
  msg += " "; terminal.print(msg); terminal.flush();
  delay(111);
}

void TerminalP(String msg, int val)
{
  msg += "="; msg += String(val, DEC);  msg += " "; 
  //terminal.print(msg); terminal.flush();
  TerminalP(msg);
}


void Send(int virtpin, int value)   
{
  Blynk.virtualWrite(virtpin, value);
   //sp("<<-- ");
//   TerminalP("< ");
}
void Send(int virtpin, float value)   //fixed float
{
  Blynk.virtualWrite(virtpin, value);
  //sp("<<== ");  
  //TerminalP("<=" + String(value));
}



BLYNK_CONNECTED(){
  // rtc.begin();
  //Blynk.syncAll();          //для генерации BLYNK_WRITE - синхронизация значения с сервера
  // Blynk.syncVirtual(VIRT_BCCOUNT);
}



void setupNet()
{
//   sp("Net:****************** ");
  
//   //Serial.println(""); Count=0;
//   Serial.print("WiFi_connected "); Serial.println(WiFi.localIP());
//   //Blynk.begin(auth, ssid, pass, "cvv.hldns.ru", 18080);   //работает - нужен порт 18080 NAT to 8080
  Blynk.begin(auth, STASSID, STAPSK, "192.168.1.147", 8080);
//   //Serial.println("Blynk_connected ");
//   //Serial.print("Duration="); Serial.println(millis() / 1000);
//   //WiFi.printDiag(Serial);
//   delay(250);
//   //    bool result = Blynk.connected();

//    TerminalP("UNO_Run ");
// //  Blynk.notify(gss);	//work
//   //Blynk.virtualWrite(VIRT_TELEGR, gss);


//   /*...if(WebClient_Connect())
//   	Blynk.notify("WebConnect");
//     else
//   	Blynk.notify("WebConnectFAIL");*/
}


#endif
