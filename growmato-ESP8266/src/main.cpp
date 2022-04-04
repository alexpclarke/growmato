#include "main.h"

// Global variables.
MachineSettings myMachine;
EnvironmentData myEnvData;
bool initialized;
IPAddress IPAddr;

// Create an AsyncWebServer object on port 80.
AsyncWebServer server(80);

// Create an AsyncWebSocket object to handle the connections on the /ws path.
AsyncWebSocket ws("/ws");

// Set up JSON buffer on the stack.
StaticJsonDocument<256> doc;

/* ----- MACHINE STUFF ----- */

// Initial set up on boot. Set up LittleFS and I2C and then wait for the system
// to be initialized over I2C.
void setup() {
  // Reset initialized.
  initialized = false;

  #ifdef DEBUG
    // Initialize the Serial Port
    Serial.begin(115200);
    delay(1000);
  #endif

  // Initialize and mount the LittleFS file system.
  if (!LittleFS.begin()){
    #ifdef DEBUG
      Serial.println("ERROR: failed to mount LittleFS");
    #endif
    return;
  } else {
    #ifdef DEBUG
      Serial.println("LittleFS mounted");
    #endif
  }

  // Initialize the I2C.
  if (!initI2C()){
    #ifdef DEBUG
      Serial.println("ERROR: failed to set up I2C");
    #endif
    return;
  } else {
    #ifdef DEBUG
      Serial.println("I2C set up");
    #endif
  }

  #ifndef WAIT_FOR_I2C_INIT
    myMachine.name = "test";
    NetworkSettings net = {NET_SSID, NET_PASS, NET_TIMEOUT};
    if (!initWiFi(net)) {
      Serial.println("ERROR: failed to connect to WiFi");
      return;
    };

    // Initialize the web socket.
    if (!initWebSocket()) {
      Serial.println("ERROR: failed to set up the web socket");
      return;
    }  

    // Successfully Initialized the machine
    initialized = true;
  #endif
}

void loop() {
  // // If the system has not been initialized, dont do anything.
  // if (!initialized) return;

  // // Remove any dead clients.
  // ws.cleanupClients();

  delay(100);
}

// Initialize the machine.
bool initMachine(MachineSettings newMachine) {
  // Set the current machine to the new one.
  myMachine = newMachine;

  // Connect to WiFi network
  if (!initWiFi(myMachine.net)) {
    #ifdef DEBUG
      Serial.println("ERROR: failed to connect to WiFi");
    #endif
    return false;
  }

  #ifdef DEBUG
    else  {
      Serial.println("WiFi Connected");
    }
  #endif

  // Initialize the web socket.
  if (!initWebSocket()) {
    #ifdef DEBUG
      Serial.println("ERROR: failed to set up the web socket");
    #endif
    return false;
  }

  #ifdef DEBUG
    else  {
      Serial.println("web socket set up");
    }
  #endif

  // Successfully Initialized the machine
  initialized = true;
  return true;
}

/* ----- I2C STUFF ----- */

// Initialize the I2C bus.
bool initI2C() {
  // Join i2c bus with provided address.
  Wire.begin(2, 0, I2C_ADDR);

  // Register event.
  Wire.onReceive(receiveI2CEvent);

  // Successfully initialized the I2C bus.
  return true;
}

// Recieve an I2C event from the Arduino and parse what to do with it.
void receiveI2CEvent(int numBytes) {
  uint8_t command = Wire.read();

  switch (command) {
    case ESP8266_CMD_INIT: {
      // Recieve the MachineData struct data.
      MachineSettings newMachine;
      for (int i = 0; Wire.available(); i++) {
        ((uint8_t*)(&newMachine))[i] = Wire.read();
      }

      // Pass the struct to the machine initialization and return the IP address
      // if successful.
      if (initMachine(newMachine)) {
        uint32_t test = (uint32_t)IPAddr;
        Wire.write((char*)(&test), 4);
      } else {
        Wire.write("\0");
      }

      break;
    }
    case ESP8266_CMD_UPDATE_ENV: {
      // Recieve the env data.
      EnvironmentData newEnv;
      for (int i = 0; Wire.available(); i++) {
        ((uint8_t*)(&newEnv))[i] = Wire.read();
      }

      // Update the env to reflect 
      myEnvData = newEnv;

      break;
    }
    default: {
      break;
    }
  }
}

/* ----- WIFI STUFF ----- */

// Initialize the WiFi.
// TODO: allow the user to choose their IP.
bool initWiFi(NetworkSettings netInfo) {
  #ifdef DEBUG
    Serial.print("Connecting to ");
    Serial.println(netInfo.SSID);
  #endif
  
  WiFi.begin(netInfo.SSID, netInfo.pass);
  uint32_t s = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    #ifdef DEBUG
      Serial.print(".");
    #endif
    s++;
    if (netInfo.timeout && s >= netInfo.timeout) {
      #ifdef DEBUG
        Serial.println("\nConnection timed out.");
      #endif
      return false;
    }
  }

  #ifdef DEBUG
    Serial.println("\nWiFi connected");

    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
  #endif

  
  // Successfully Connected.
  myMachine.net = netInfo;
  IPAddr = WiFi.localIP();
  return true;
}

// Initializes the web socket.
bool initWebSocket() {
  // Setup the web socket.
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // Set up the the routes.
  routes();

  // Start server
  server.begin();  
  return true;
}

String processor(const String& var) {
  if (var == "DEVICE_NAME") {
    return myMachine.name;
  } else if (var == "NET_SSID") {
    return myMachine.net.SSID;
  } else if (var == "NET_PASS") {
    return myMachine.net.pass;
  } else if (var == "NET_TIMEOUT") {
    return String(myMachine.net.timeout);
  } else if (var == "SELECT_BACKLIGHT") {
    if (myMachine.flags | _BV(ESP8266_FLAG_BACKLIGHT)) {
      return "<option value=\"on\" selected>On</option>\
      <option value=\"off\">Off</option>";
    } else {
      return "<option value=\"on\">On</option>\
      <option value=\"off\" selected>Off</option>";
    }
  } else if (var == "SELECT_LED") {
    if (myMachine.flags | _BV(ESP8266_FLAG_LED)) {
      return "<option value=\"on\" selected>On</option>\
      <option value=\"off\">Off</option>";
    } else {
      return "<option value=\"on\">On</option>\
      <option value=\"off\" selected>Off</option>";
    }
  }
  return String();
}

// Handle web socket events. 
void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
AwsEventType type, void* arg, uint8_t* data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      #ifdef DEBUG
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
        client->remoteIP().toString().c_str());
      #endif
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}


void notifyClients(String str) {
  ws.textAll(str);
}

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
  AwsFrameInfo* info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    // Ensure our output is null terminated
    data[len] = 0;

    // Parse the request.
    // DynamicJsonDocument doc(len);
    // deserializeJson(doc, (char*)data);

    notifyClients((char*)data);
  }
}

void routes() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(LittleFS, "/style.css", String(), false);
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(LittleFS, "/script.js", String(), false);
  });

  server.on("/env", HTTP_GET, [](AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse();
    response->addHeader("Server","ESP Async Web Server");
    JsonVariant& root = response->getRoot();
    root["soil_moisture"] = myEnvData.SoilMoisture;
    root["air_temp"] = myEnvData.AirTemp;
    response->setLength();
    request->send(response);
  });

  server.onNotFound([](AsyncWebServerRequest* request) {
    request->send(404);
  });
}