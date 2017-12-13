#include <Ethernet.h>
#include <SPI.h>

IPAddress server(192, 168, 0, 104); // numeric IP for Google (no DNS)
//char server[] = "www.google.com";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

void setup()
{
    Serial.begin(9600);
    Ethernet.begin(mac);
}

void loop()
{
    if (client.connect(server, 9000)) {
        client.println("GET /api/v1/stations/10:20:30:40:50:80/config HTTP/1.1");
        client.println("Host: 192.168.0.104");
        client.println("Connection: close");
        client.println();

        //delay(40);
        char c;
        bool httpBody = false, currentLineIsBlank = false;
        while (client.connected()) {
            if (client.available()) {

                c = client.read();

                if (httpBody) {
                    Serial.print(c);
                } else {
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
        Serial.println("conexão gg");
    }
    else {
        Serial.println("Falha na conexão");
    }

    delay(5000);
}
