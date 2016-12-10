#define IP_FORWARD 1

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

/* Set these to your desired credentials for ESP8266 AP. */
const char *ssid = "ESPap";
const char *password = "thereisnospoon";
IPAddress apIP(192, 168, 4, 1);

//const char* ssidExt = "dlink";
//const char* passwordExt = "";

// Setup credentials for original WiFi, that we plan to repeat
const char* ssidExt = "RT-WiFi_eea5";
const char* passwordExt = "";

const byte DNS_PORT = 53;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
byte packetBuffer[512]; //buffer to hold incoming and outgoing packets

WiFiUDP Udp8;

// Update these with values suitable for your network.
IPAddress ip(192, 168, 0, 13); //Node static IP
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  Serial.begin(115200);
  Serial.println();

   WiFi.mode(WIFI_AP_STA);
   //WiFi.mode(WIFI_AP);

  Serial.println("!!!!!!!!!!!!!!!!!!!++");
  WiFi.begin(ssidExt, passwordExt);
  WiFi.config(ip, gateway, subnet);

  //Wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssidExt);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("dnsIP address: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("gatewayIP address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("subnetMask address: ");
  Serial.println(WiFi.subnetMask());


  Serial.println("");
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Udp.begin(DNS_PORT);
  Udp8.begin(DNS_PORT);
}

void loop()
{

  int noBytes = Udp.parsePacket();
  if ( noBytes ) {
    //    Serial.print(millis() / 1000);
    //    Serial.print(":Packet of ");
    //    Serial.print(noBytes);
    //    Serial.print(" received from ");
    //    Serial.print(Udp.remoteIP());
    //    Serial.print(":");
    //    Serial.println(Udp.remotePort());
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, noBytes); // read the packet into the buffer

    // display the packet contents in HEX
    //    for (int i = 1; i <= noBytes; i++) {
    //      Serial.print(packetBuffer[i - 1], HEX);
    //      if (i % 32 == 0) {
    //        Serial.println();
    //      }
    //      else Serial.print(' ');
    //    } // end for
    //    Serial.println();

    IPAddress ip8(8, 8, 8, 8);
    Udp8.beginPacket(ip8, DNS_PORT); //NTP requests are to port 123
    Udp8.write(packetBuffer, noBytes);
    Udp8.endPacket();
    delay(100);

    int cb = Udp8.parsePacket();
    if (!cb) {
     // Serial.println("no packet yet");
    }
    else {
      //      Serial.print("packet received, length=");
      //      Serial.println(cb);
      byte packetBuffer8[cb];
      Udp8.read(packetBuffer8, cb);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(packetBuffer8, cb);
      Udp.endPacket();
    }

  } // end if
}
