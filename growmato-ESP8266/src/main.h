#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <Wire.h>
#include "MachineDataStruct.h"
 
#ifndef MAIN_H_
  #define MAIN_H_

  // #define DEBUG
  #define WAIT_FOR_I2C_INIT

  // #define NET_SSID    "Makerspace"
  // #define NET_PASS    "blics101"
  #define NET_SSID    "STONEYBROOK D1"
  #define NET_PASS    "twicetwice"
  #define NET_IP      
  #define NET_TIMEOUT 0   // Timeout in seconds, set to 0 for no timeout.
  #define I2C_ADDR    0x04

  // prototypes
  void notifyClients(void);
  bool initI2C();
  void receiveI2CEvent(int numBytes);

  void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
    AwsEventType type, void* arg, uint8_t* data, size_t len);
  void handleWebSocketMessage(void* arg, uint8_t* data, size_t len);
  bool initWiFi(NetworkSettings netInfo);
  bool initWebSocket(void);
  bool initMachine(MachineSettings newMachine);
  
  void routes(void);
  String processor(const String& var);
  
#endif