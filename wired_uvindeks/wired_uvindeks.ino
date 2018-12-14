/*
  Web Server

  A simple web server that shows the value of the analog input pins.
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13
   Analog inputs attached to pins A0 through A5 (optional)

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

*/

#include <SPI.h>
#include <Ethernet2.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0x55, 0x87
};


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(88);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

double randomDouble(double minf, double maxf)
{
  return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      /*if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply


        Serial.println("CURRENT " +currentLine);

        if (currentLine.length() == 0) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          String s = String(round(randomDouble(19.01, 26.00) * 100.0) / 100.0 , DEC);
          // s.replace(".", ",");

          client.print(s);


          client.println("</html>");
          break;
        } else {      // if you got a newline, then clear currentLine:
          currentLine = "";
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLine += c;
        }

        if (currentLine.endsWith("GET /config")) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/xml");
          client.println();
          // the content of the HTTP response follows the header:
          client.print("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><configuration><ipaddress>");
          client.print(Ethernet.localIP());
          client.print("</ipaddress><observable>12</observable><macaddress>");

          client.print(mac[5], HEX);
          client.print(":");
          client.print(mac[4], HEX);
          client.print(":");
          client.print(mac[3], HEX);
          client.print(":");
          client.print(mac[2], HEX);
          client.print(":");
          client.print(mac[1], HEX);
          client.print(":");
          client.print(mac[0], HEX);
          client.print("</macaddress><time>1</time><port>88</port><type>Temperature</type></configuration>");

          //client.print("</macaddress><commands><command><name>Tænd lys</name><url>TU</url><min>0</min><max>0</max></command><command><name>Dæmp lys</name><url>DL</url><min>5</min><max>20000</max></command></commands></configuration>");

          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;
        }
        }*/






      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
 
             // client.print(random(5, 10));
              client.print(7);
              /* client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
                client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");
              */
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            }
            else {      // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          }
          else if (c != '\r') {    // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }


          if (currentLine.endsWith("GET /config")) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/xml");
            client.println();

            // the content of the HTTP response follows the header:


            client.print("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><configuration><ipaddress>");
            client.print(Ethernet.localIP());
            client.print("</ipaddress><observable>13</observable><macaddress>");

            client.print(mac[0], HEX);
            client.print(":");
            client.print(mac[1], HEX);
            client.print(":");
            client.print(mac[2], HEX);
            client.print(":");
            client.print(mac[3], HEX);
            client.print(":");
            client.print(mac[4], HEX);
            client.print(":");
            client.print(mac[5], HEX);
            client.print("</macaddress><time>1</time><port>88</port><protocol>http</protocol><type>UV</type></configuration>");

            //client.print("</macaddress><commands><command><name>Tænd lys</name><url>TU</url><min>0</min><max>0</max></command><command><name>Dæmp lys</name><url>DL</url><min>5</min><max>20000</max></command></commands></configuration>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
        }
      }
      // close the connection:

      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }
  }
}

