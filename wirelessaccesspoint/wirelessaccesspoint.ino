#include <SPI.h>
#include <WiFi101.h>


char apssid[] = "MKR1000AP2";
int status = WL_IDLE_STATUS;
WiFiServer server(88);

String HTTP_req;
boolean readingNetwork = false;
boolean readingPassword = false;
String password = "";
String network = "";
boolean needCredentials = true;
boolean needWiFi = false;

boolean setupWebServer = false;

int onBoardLed = 6;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(onBoardLed, OUTPUT);

  Serial.println("Access Point Web Server");

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  Serial.print("Creating access point named: ");
  Serial.println(apssid);

  if (WiFi.beginAP(apssid) != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }

  delay(1000);
  server.begin();
  printAPStatus();
}

void loop() {
  if (needCredentials) {
    getCredentials();
  }
  if (needWiFi) {
    getWiFi();
    setupWebServer = true;
 }
  if (setupWebServer) {
    server.begin();

    while (true) {
      // listen for incoming clients
      WiFiClient client = server.available();
      if (client) {                             // if you get a client,
        Serial.println("new client");           // print a message out the serial port
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
                client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
                client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

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

            // Check to see if the client request was "GET /H" or "GET /L":
            if (currentLine.endsWith("GET /H")) {
              digitalWrite(onBoardLed, HIGH);               // GET /H turns the LED on
            }
            if (currentLine.endsWith("GET /L")) {
              digitalWrite(onBoardLed, LOW);                // GET /L turns the LED off
            }
          }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
      }
    }
  }
}


void getCredentials() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        if (c == '?') readingNetwork = true;
        if (readingNetwork) {
          if (c == '!') {
            readingPassword = true;
            readingNetwork = false;
          }
          else if (c != '?') {
            network += c;
          }
        }
        if (readingPassword) {
          if (c == ',') {
            Serial.println();
            Serial.print("Network Name: ");
            Serial.println(network);
            Serial.print("Password: ");
            Serial.println(password);
            Serial.println();
            client.stop();
            WiFi.end();
            readingPassword = false;
            needCredentials = false;
            needWiFi = true;
          }
          else if (c != '!') {
            password += c;
          }
        }
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html>");
            client.println("<head>");
            client.println("<style type=\"text/css\"> body {font-family: sans-serif; margin:50px; padding:20px; line-height: 250% } </style>");
            client.println("<title>Arduino Setup</title>");
            client.println("</head>");
            client.println("<body>");

            client.println("<h2>WIFI CREDENTIALS</h2>");
            client.print("NETWORK NAME: ");
            client.print("<input id=\"network\"/><br>");
            client.print("PASSWORD: ");
            client.print("<input id=\"password\"/><br>");

            client.print("<button type=\"button\" onclick=\"SendText()\">Enter</button>");
            client.println("</body>");
            client.println("<script>");
            client.println("var network = document.querySelector('#network');");
            client.println("var password = document.querySelector('#password');");

            client.println("function SendText() {");
            client.println("nocache=\"&nocache=\" + Math.random() * 1000000;");
            client.println("var request =new XMLHttpRequest();");
            client.println("netText = \"&txt=\" + \"?\" + network.value + \"!\" + password.value + \",&end=end\";");
            client.println("request.open(\"GET\", \"ajax_inputs\" + netText + nocache, true);");
            client.println("request.send(null)");
            client.println("network.value=''");
            client.println("password.value=''}");
            client.println("</script>");
            client.println("</html>");
            client.println();
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("client disconnected");
    Serial.println();
  }
}

void getWiFi () {
  if (network == "" or password == "") {
    Serial.println("Invalid WiFi credentials");
    while (true);
  }
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(network);
    WiFi.begin(network.c_str(), password.c_str());
    delay(10000);
  }
  Serial.println("WiFi connection successful");
  printWiFiStatus();
  needWiFi = false;
  delay(1000);
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void printAPStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print("To connect, open a browser to http://");
  Serial.println(ip);
}

