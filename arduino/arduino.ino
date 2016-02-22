// required an ethernet module

#include <SPI.h>
#include <Ethernet.h>
#include <ShiftRegister74HC595.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 113);

EthernetServer server(80);

// create shift register object (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr (2, 2, 3, 4); 

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
            
            // convert string to byte and show the number on the seven-segment display
            char bytebuf[32]; // make this at least big enough for the whole string
            visitorsOnlineString.toCharArray(bytebuf, sizeof(bytebuf));
            byte b = displayByte((byte)atoi(bytebuf));
            
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

void displayByte(byte x)
{
  x %= 100;
  uint8_t data[] = { dec27(x / 10), dec27(x % 10) };
  sr.setAll(data);
}

byte dec27(byte decimal) {
  if (decimal == 0)
    return B00111111;
  if (decimal == 1)
    return B00000110;
  if (decimal == 2)
    return B01011011;
  if (decimal == 3)
    return B01001111;
  if (decimal == 4)
    return B01100110;
  if (decimal == 5)
    return B01101101;
  if (decimal == 6)
    return B01111101;
  if (decimal == 7)
    return B0000111;
  if (decimal == 8)
    return B01111111;
  if (decimal == 9)
    return B01101111;
}

