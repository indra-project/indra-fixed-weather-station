#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <DHT.h>

//Webservice Configs
  //IPAddress server(192, 168, 0, 104);
  char server[] = "www.indra-project.tk";
  char pathToRoute[] = "/public/index.php";
  int port = 80;

//Ethernet Shield Config
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  char MAC_ADDRESS_STATION[] = "DE:AD:BE:EF:FE:ED";

//Ethernet Client
  EthernetClient client;

//GET and POST methods
  void getData(String path, String* response);
  void postData(String path, String params, String* response);

//Function Station Config
  void stationConfig();

//Function Create Station
  void createStation();

//Function Create Sensor
  void createSensor(String name, String type, int intervals, String unit);

//Function Create Sensor
  void createDataSensor(String label, String value);
  
//Config Station
  bool STATION_ACTIVE = true;

//StaticJsonBuffer<1000> jsonBuffer;  
StaticJsonBuffer<1000> jsonBuffer;  

//Sensors Config
  //WATER_FLUX
  bool    SENSOR_WATER_FLUX_ACTIVE = true;
  String  SENSOR_WATER_FLUX_LABEL = "WATER_FLUX";
  int     SENSOR_WATER_FLUX_INTERVALS = 60;

  //AIR_HUMIDITY
  bool    SENSOR_AIR_HUMIDITY_ACTIVE = true;
  String  SENSOR_AIR_HUMIDITY_LABEL = "AIR_HUMIDITY";
  int     SENSOR_AIR_HUMIDITY_INTERVALS = 60;

  //SOIL_MOISTURE
  bool    SENSOR_SOIL_MOISTURE_ACTIVE = true;
  String  SENSOR_SOIL_MOISTURE_LABEL = "SOIL_MOISTURE";
  int     SENSOR_SOIL_MOISTURE_INTERVALS = 60;
  #define SOILPIN A0

  //VIBRATION
  bool    SENSOR_VIBRATION_ACTIVE = true;
  String  SENSOR_VIBRATION_LABEL = "VIBRATION";
  int     SENSOR_VIBRATION_INTERVALS = 60;

  //FIRE_FLAME
  bool    SENSOR_FIRE_FLAME_ACTIVE = true;
  String  SENSOR_FIRE_FLAME_LABEL = "FIRE_FLAME";
  int     SENSOR_FIRE_FLAME_INTERVALS = 60;

  //TEMPERATURE
  bool    SENSOR_TEMPERATURE_ACTIVE = true;
  String  SENSOR_TEMPERATURE_LABEL = "TEMPERATURE";
  int     SENSOR_TEMPERATURE_INTERVALS = 60;
  #define DHTPIN A1
  #define DHTTYPE DHT22
  DHT dht(DHTPIN, DHTTYPE);

  //PRESENCE_SENSOR
  bool    SENSOR_PRESENCE_SENSOR_ACTIVE = true;
  String  SENSOR_PRESENCE_SENSOR_LABEL = "PRESENCE_SENSOR";
  int     SENSOR_PRESENCE_SENSOR_INTERVALS = 60;
  #define PRESENCEPIN 4
void setup()
{
    Serial.begin(9600);
    pinMode(PRESENCEPIN, INPUT);
    
    
    Serial.println("Ethernet: Connecting");
    Ethernet.begin(mac);
    Serial.print("Ethernet: Connected \nIP: ");
    Serial.println(Ethernet.localIP());

    dht.begin();

    //STATION CONFIG
    stationConfig();
}

void loop()
{
    if (SENSOR_TEMPERATURE_ACTIVE) {
        String value = "";
               value += dht.readTemperature();
        createDataSensor(SENSOR_TEMPERATURE_LABEL, value);
    }

    if (SENSOR_AIR_HUMIDITY_ACTIVE) {
        String value = "";
               value += dht.readHumidity();
        createDataSensor(SENSOR_AIR_HUMIDITY_LABEL, value);
    }

    if (SENSOR_SOIL_MOISTURE_ACTIVE) {
        int output_value ;
            output_value = analogRead(SOILPIN);
            output_value = map(output_value,1024,0,0,100);
        String value = "";
               value += output_value;
        createDataSensor(SENSOR_SOIL_MOISTURE_LABEL, value);
    }

    if (SENSOR_PRESENCE_SENSOR_ACTIVE) {
        int sensorRead = digitalRead(PRESENCEPIN);
        String value = "";
               value += sensorRead;
        createDataSensor(SENSOR_PRESENCE_SENSOR_LABEL, value);
    }
    
    stationConfig();
      
      
    delay(5000);
}

void createDataSensor(String label, String value)
{
    String response = "";
    String path = "/api/v1/stations/";
           path += MAC_ADDRESS_STATION;
           path += "/sensors/";
           path += label;
           path += "/data";
           
    String params = "value=";
           params += value;
           params += "&date=2017-12-13 18:40:40";
           params += "&altitude=0";
           params += "&latitude=0";
           params += "&longitude=0";
    postData(path, params, &response);
}

void stationConfig()
{
    String response = "[]";
    while(response == "[]") {
        response = "";
        Serial.println("################Buscando configuracao da estação#####################");
        String path = "/api/v1/stations/";
               path += MAC_ADDRESS_STATION;
               path += "/config";
               
        getData(path, &response);
        Serial.println(response);
        Serial.println("###########Terminou busca de configuracao da estacao#################");
        if (response == "[]"){
            createStation();
        }else{
            JsonObject& jsonConfig = jsonBuffer.parseObject(response);
            
            STATION_ACTIVE = jsonConfig["active"];
            
            SENSOR_TEMPERATURE_ACTIVE = jsonConfig["sensors"][SENSOR_TEMPERATURE_LABEL]["active"];
            SENSOR_TEMPERATURE_INTERVALS = jsonConfig["sensors"][SENSOR_TEMPERATURE_LABEL]["intervals"];

            SENSOR_AIR_HUMIDITY_ACTIVE = jsonConfig["sensors"][SENSOR_TEMPERATURE_LABEL]["active"];
            SENSOR_AIR_HUMIDITY_INTERVALS = jsonConfig["sensors"][SENSOR_TEMPERATURE_LABEL]["intervals"];

        }
    }
}

void createStation()
{
    Serial.println("################Cadastrando estação#####################");
    String response = "";
    String path = "/api/v1/stations";
    String params = "name=Estação Meteorológica Fixa";
           params += "&description=Lorem ipsum dolor sit amet, nam no utamur adipiscing contentiones, apeirian sapientem vulputate per ad. Lorem ipsum dolor sit amet, nam no utamur adipiscing contentiones, apeirian sapientem vulputate per ad.";
           params += "&type=FIX";
           params += "&active=1";
           params += "&mac_address=";
           params += MAC_ADDRESS_STATION;
    postData(path, params, &response);
    Serial.println(response);
    Serial.println("################Terminou cadastro da estação#####################");
    createSensor("Fluxo de agua", SENSOR_WATER_FLUX_LABEL, SENSOR_WATER_FLUX_INTERVALS, "L/min");
    createSensor("Umidade do ar", SENSOR_AIR_HUMIDITY_LABEL, SENSOR_AIR_HUMIDITY_INTERVALS, "%");
    createSensor("Umidade do solo", SENSOR_SOIL_MOISTURE_LABEL, SENSOR_SOIL_MOISTURE_INTERVALS, "%");
    createSensor("Vibração", SENSOR_VIBRATION_LABEL, SENSOR_VIBRATION_INTERVALS, "~~");
    createSensor("Chamas", SENSOR_FIRE_FLAME_LABEL, SENSOR_FIRE_FLAME_INTERVALS, "--");
    createSensor("Temperatura", SENSOR_TEMPERATURE_LABEL, SENSOR_TEMPERATURE_INTERVALS, "ºC");
    createSensor("Presença", SENSOR_PRESENCE_SENSOR_LABEL, SENSOR_PRESENCE_SENSOR_INTERVALS, "--");
}

void createSensor(String name, String type, int intervals, String unit)
{
    Serial.println("################Cadastrando sensor#####################");
    String response = "";
    String path = "/api/v1/stations/";
           path += MAC_ADDRESS_STATION;
           path += "/sensors";
           
    String params = "name=";
              params += name;
           params += "&type=";
              params += type;
           params += "&intervals=";
              params += intervals;
           params += "&unit=";
              params += unit;
           params += "&active=1";
           
    postData(path, params, &response);
    Serial.println(response);
    Serial.println("################Terminou cadastro do sensor#####################");
}

void postData(String path, String params, String* response)
{
    Serial.println("HTTP: Connecting");
    if (client.connect(server, port)) {
        Serial.println();
        Serial.println("#############Enviando dados POST#############");
        Serial.println("HTTP: Connected");
        client.print("POST ");
        client.print(pathToRoute);
        client.print(path);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Connection: close");
        client.println("User-Agent: Arduino/1.0");
        client.print("Content-Length: ");
        client.println(params.length());
        client.println();
        client.print(params);
        client.println();

        delay(100);
        
        Serial.println("HTTP: Request");
        Serial.print("POST ");
        Serial.print(pathToRoute);
        Serial.print(path);
        Serial.println(" HTTP/1.1");
        Serial.print("Host: ");
        Serial.println(server);
        Serial.println("Content-Type: application/x-www-form-urlencoded");
        Serial.println("Connection: close");
        Serial.println("User-Agent: Arduino/1.0");
        Serial.print("Content-Length: ");
        Serial.println(params.length());
        Serial.println();
        Serial.print(params);
        Serial.println();

        //delay(40);
        char c;
        bool httpBody = false;
        bool currentLineIsBlank = false;
        
        while (client.connected()) {
            if (client.available()) {

                c = client.read();

                if (httpBody) {
                    response->concat(c);
                }
                else {
                    if (c == '\n' && currentLineIsBlank) {
                        httpBody = true;
                    }

                    if (c == '\n') {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    }
                    else if (c != '\r') {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
        }
        client.stop();
        Serial.println("HTTP: Request Finished");
        Serial.println("###############DADOS ENVIADOS################");
    }
    else {
        Serial.println("HTTP: Connection Failed");
    }
}
void getData(String path, String* response)
{
    
    Serial.println("HTTP: Connecting");
    if (client.connect(server, port)) {
        Serial.println();
        Serial.println("#############Pegando dados GET#############");
        Serial.println("HTTP: Connected");
        //client.println("GET /api/v1/stations/10:20:30:40:50:80/config HTTP/1.1");
        client.print("GET ");
        client.print(pathToRoute);
        client.print(path);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Connection: close");
        client.println();

        delay(100);
        
        Serial.println("HTTP: Request");
        Serial.print("GET ");
        Serial.print(pathToRoute);
        Serial.print(path);
        Serial.println(" HTTP/1.1");
        Serial.print("Host: ");
        Serial.println(server);
        Serial.println("Connection: close");
        Serial.println();
        Serial.println();

        //delay(40);
        char c;
        bool httpBody = false;
        bool currentLineIsBlank = false;
        
        while (client.connected()) {
            if (client.available()) {

                c = client.read();

                if (httpBody) {
                    response->concat(c);
                }
                else {
                    if (c == '\n' && currentLineIsBlank) {
                        httpBody = true;
                    }

                    if (c == '\n') {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    }
                    else if (c != '\r') {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
        }
        client.stop();
        Serial.println("HTTP: Request Finished");
        Serial.println("##############DADOS RECEBIDOS##############");
    }
    else {
        Serial.println("HTTP: Connection Failed");
    }
}
