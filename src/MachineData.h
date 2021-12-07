#include <stdlib.h>
#include <stdbool.h>

#ifndef MACHINE_DATA_H_
  #define MACHINE_DATA_H_

  #define ESP8266_FLAG_BACKLIGHT  0x00
  #define ESP8266_FLAG_LED        0x01
  #define ESP8266_FLAG_INVERT     0x02

  #define ESP8266_CMD_INIT        0x01
  #define ESP8266_CMD_UPDATE_ENV  0x02
  #define ESP8266_CMD_READ_UPDATE 0x03

  typedef struct NetworkSettings_ {
    char SSID[32];
    char pass[32];
    uint32_t timeout;
  } NetworkSettings;

  typedef struct EnvironmentSettings_ {
    int SoilMoisture_Set;
    int SoilMoisture_Tol;
    int AirTemp_Set;
    int AirTemp_Tol;
  } EnvironmentSettings;

  typedef struct MachineSettings_ {
    char name[20];
    uint8_t flags;
    NetworkSettings net;
    EnvironmentSettings env;
  } MachineSettings;

  typedef struct EnvironmentData_ {
    int SoilMoisture;
    float AirTempF;
    float AirRH;
  } EnvironmentData;

  volatile MachineSettings myMachine;
  volatile EnvironmentData myEnv;

#endif