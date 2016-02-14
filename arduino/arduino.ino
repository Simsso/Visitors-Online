// required an ethernet module

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 113);

EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) { ;}

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Local IP-address: ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    int i = 0; // chars received counter
    String visitorsOnlineString = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (i >= 5) { // from 6th car to the next space read the number
          if (c != ' ') {
            visitorsOnlineString += c;
          }
          else {
            Serial.println(visitorsOnlineString);
            
            // stop after receiving the data
            client.stop();
          }
        }
        i++;
      }
    }
    
    Ethernet.maintain();
  }
}

