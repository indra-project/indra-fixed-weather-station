#include <Ethernet.h>
#include <SPI.h>

IPAddress server(192, 168, 0, 104); // numeric IP for Google (no DNS)
//char server[] = "www.google.com";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

void doRequest(const char* method, const char* path, String* response);

void setup()
{
    Serial.begin(9600);
    
    Serial.println("Ethernet: Connecting");
    Ethernet.begin(mac);
    Serial.println("Ethernet: Connected");
}

void loop()
{

    String res = "";

    doRequest("GET", "/api/v1/stations/10:20:30:40:50:80/config", &res);

    Serial.println(res);

    delay(5000);
}

void doRequest(const char* method, const char* path, String* response)
{
    Serial.println("HTTP: Connecting");
    if (client.connect(server, 9000)) {
        Serial.println("HTTP: Connected");
        //client.println("GET /api/v1/stations/10:20:30:40:50:80/config HTTP/1.1");
        client.print(method);
        client.print(" ");
        client.print(path);
        client.println(" HTTP/1.1");
        client.println("Host: 192.168.0.104");
        client.println("Connection: close");
        client.println();
        
        Serial.println("HTTP: Request");
        Serial.print(method);
        Serial.print(" ");
        Serial.print(path);
        Serial.println(" HTTP/1.1");
        Serial.println("Host: 192.168.0.104");
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
    }
    else {
        Serial.println("HTTP: Connection Failed");
    }
}

