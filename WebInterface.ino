#define MAX_FRAME_LENGTH 64
#define CALLBACK_FUNCTIONS 1
#define DEBUGGING 1

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
boolean clientConnected = false;
boolean socketServerConnected = false;
char serverAddress[] = "192.168.4.200";
int port = 80;
//WiFiClient client;
WiFiServer server(80);
unsigned long webTimer = 0;
unsigned long webInterval = 5000;
String allTheSerial;
boolean notFirstReload = false;
int webInterfaceRetryCounter = 0;
int count = 0;

WebSocketsServer webSocket = WebSocketsServer(80);

File webPage;

char webpageww[] PROGMEM = R"=====(
<html><meta charset='utf-8'/>
<head>
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('wss://192.168.4.1/', ['protocolOne', 'protocolTwo']);
      Socket.onmessage = function(event){
        document.getElementById("rxConsole").value += event.data;
        console.log(event.data);
      }
      Socket.onopen = function(event){
        Socket.send('QWEQWEQWE');
      }
    }
    function sendText(){
      Socket.send(document.getElementById("txBar").value);
      document.getElementById("txBar").value = "";
    }
    function sendBrightness(){
      Socket.send("#"+document.getElementById("brightness").value);
    }    
  </script>
</head>
<body onload="javascript:init()">
  <div>
    <textarea id="rxConsole"></textarea>
  </div>
  <hr/>
  <div>
    <input type="text" id="txBar" onkeydown="if(event.keyCode == 13) sendText();" />
  </div>
  <hr/>
  <div>
    <input type="range" min="0" max="1023" value="512" id="brightness" oninput="sendBrightness()" />
  </div>  
</body>
</html>
)=====";

void setupWebInterface() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    //while (true);
    gotWebInterface = false;
  }
  else {
    gotWebInterface = true;
    String fv = WiFi.firmwareVersion();
    Serial.print("WiFi.firmwareVersion = ");
    Serial.println(fv);
    if (fv < "1.0.0") {
      Serial.println("Please upgrade the firmware");
    }

    Serial.print("Server starting.. ");
    Serial.println(ssid);
    status = WiFi.beginAP(ssid);
    delay(10000);
    webTimer = currentMillis;
    if (status != WL_AP_LISTENING) {
      Serial.println("Creating access point failed");
      gotWebInterface = false;
    }
    else {
      server.begin();
      Serial.println("beginning websocket server....");
      // Start WebSocket server and assign callback
      webSocket.begin();
      webSocket.onEvent(webSocketEvent);
      // you're connected now, so print out the status:
      printWifiStatus();

    }
  }

}

void loopSocketInterface() {
  webSocket.loop();
  String s = "socket test";
  //char c[] = {(char)s};
  if (currentMillis - webTimer > webInterval) {
    webTimer = currentMillis;
    Serial.println("sending socket test ping");
    webSocket.broadcastTXT("socket test", sizeof("socket test"));
  }
}

void loopWebInterface() {
  String data;
  loopSocketInterface();

  if (!gotWebInterface && webInterfaceRetryCounter < 10) {
    delay(2000);
    Serial.println("retrying web setup.............");
    webInterfaceRetryCounter++;
    setupWebInterface();
  }

  // listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      webSocket.loop();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header

          //webPage = SD.open("acro.htm"); // open web page file
          if (webPage) {
            while (webPage.available()) {
              //Serial.println("sending WebSocketServer for realz.....");
              client.write(webPage.read()); // send web page to client
            }
            webPage.close();
          } else {
            Serial.println("......no webPage.......");
            //client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: text/html");
            //client.println("Connection: close");  // the connection will be closed after completion of the response
            //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.write(webpageww);

          }
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }

    }
    webSocket.loop();
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  //Serial.print("webSocketEvent type=");
  //Serial.println(type);
  if (type == WStype_TEXT) {
    if (payload[0] == '#') {
      uint16_t brightness = (uint16_t) strtol((const char *) &payload[1], NULL, 10);
      brightness = 1024 - brightness;
      //analogWrite(pin_led, brightness);
      Serial.print("brightness= ");
      Serial.println(brightness);
    }
    else {
      Serial.println("--------------FROM WEBSOCKET----------------");
      for (int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      Serial.println();
    }
  }
  switch(type) {
        case WStype_DISCONNECTED:
            //Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            {
                //IPAddress ip = webSocket.remoteIP(num);
                Serial.print("--------------------WEBSOCKET Connected from"); 
            }
            break;
        case WStype_TEXT:            
            Serial.print("get Text: ");
            //Serial.println(payload);
            // echo data back to browser
            webSocket.sendTXT(num, payload, length);
            // send data to all connected clients
            webSocket.broadcastTXT(payload, length);
            break;
        case WStype_BIN:
            Serial.println("get binary length:");
            //hexdump(payload, length);
            // echo data back to browser
            //webSocket.sendBIN(num, payload, length);
            break;
        case WStype_ERROR:
            break;

        default:
            break;
    }

}
