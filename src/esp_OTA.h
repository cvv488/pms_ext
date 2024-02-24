/********************************************************
  ESP-32 "192.168.1.159",
  OTA  
  При ошибке повторить
*********************************************************/
#include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
// #include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "cvv_include.h"

void OTA_setup() 
{
  Serial.print("OTA Connection...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  int c = 0;
  while (1) 
  {
    // int w = WiFi.status();
    // Serial.print(w); Serial.print(" ");  //7 OK Ready
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      // Serial.print("OK ");
      break;
    }
    // Serial.println();
    delay(1000);
    if (c > 10) ESP.restart();
  }
  Serial.print("ok ");
  /*
   * typedef enum {
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_WRONG_PASSWORD   = 6, ------- был ???
    WL_DISCONNECTED     = 7
} wl_status_t;
  */


  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("ESP32_uno");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void OTA_loop() {
  ArduinoOTA.handle();
}
