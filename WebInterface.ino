#include <WiFiNINA.h>
#include <Base64.h>
//#include "wifi_secrets.h"
#include <Arduino_LSM6DS3.h>
#define ACCESS_POINT  1
#define DEBUG 1
#ifdef DEBUG
#define Sprint(a) (Serial.print(a))
#define Sprintln(a) (Serial.println(a))
#else
#define Sprint(a)
#define Sprintln(a)
#endif

int volumecounter = 0;
unsigned long webTimer = 0;
unsigned long webInterval = 5000;
int webInterfaceRetryCounter = 0;
long volume;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
IPAddress ip(192, 168, 4, 1);
WiFiServer server(80);
WiFiClient sse;
boolean on = false;
boolean goodSettings = false;
boolean gotSSE = false;
int cowbell = 1000;
int ledState = 0;
unsigned long previousMillis = 0;
//original // char webpage_base64[] = "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";
// hello world //char webpage_base64[] = "PCFET0NUWVBFIGh0bWw+CjxodG1sIGxhbmc9ImVuIj4KPGhlYWQ+CiAgPG1ldGEgY2hhcnNldD0idXRmLTgiPgogIDxtZXRhIG5hbWU9InZpZXdwb3J0IgogICAgIGNvbnRlbnQ9IndpZHRoPWRldmljZS13aWR0aCwgaW5pdGlhbC1zY2FsZT0xLCB1c2VyLXNjYWxhYmxlPXllcyI+CiAKICA8dGl0bGU+SGVsbG8gV29ybGQhPC90aXRsZT4KPC9oZWFkPgo8Ym9keT4KSGVsbG8gV29ybGQhCiAKPC9ib2R5Pgo8L2h0bWw+";
// functional...  char webpage_base64[] = "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";
//custom for OpenGlider
char webpage_base64[] = "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";


void setupWebInterface() {
#ifdef ACCESS_POINT
  WiFiConnectAccessPoint();
#else
  WiFiConnect();
#endif

}

void logToWebClientSSE(String tolog) {
  if (!NO_WEB) {
    Serial.print("logToWebClientSSE: ");
    Serial.println(tolog);
    //sse.println("event: volume");
    sse.print("data: ");
    sse.println(tolog);
    sse.println("");
  }
}

void logToWebClientSSE_RAW(String tolog) {
  if (!NO_WEB) {
    sse.println(tolog);
    sse.println("");
  }
}
void loopSSE_send() {
  if (gotSSE) {
    if (currentMillis - webTimer > webInterval) {
      webTimer = currentMillis;

    }
  }
}
void loopWebInterface() {
  if (!gotWebInterface && webInterfaceRetryCounter < 10) {
    delay(2000);
    logln("retrying web setup.............");
    webInterfaceRetryCounter++;
    setupWebInterface();
  }
  //loopSSE_send();
  WiFiClient client = server.available();

  if (client) {
    Sprintln("\n--New client");
    String header = "";
    String elname = "";
    String value = "";

    while (client.connected()) {            // loop while the client is connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte
        if (c != '\r') {
          header += c;
        }
        if (header.substring(header.length() - 2) == "\n\n") {
          Sprint(header.substring(0, header.length() - 1));

          if (header.indexOf("HEAD /?") > -1) {
            client.println("HTTP/1.1 200 OK");
#ifdef DEBUG
            client.println("Access-Control-Allow-Origin: *");
#endif
            client.println();
            client.stop();
            header.remove(0, header.indexOf("?") + 1);
            header.remove(header.indexOf(" "));
            elname = header.substring(0, header.indexOf("="));
            value = header.substring(header.indexOf("=") + 1);
            goodSettings = true;
            if (elname == "deployONSwitch") {
              if (value.equals("true")) {
                  logln("deployONSwitch = true");
                  logln("deploy glider=========");
                  deployGlider();
                
              } else {
                  logln("deployONSwitch = false");
              }
              on = (value == "true");
            } else if (elname == "deployOFFSwitch") {
              logln("deployOFFSwitch");
              
            }  else if (elname == "pumpINSwitch") {
              logln("pumpINSwitch");
              pumpIn();
              sse.println("event: pumpOFFSwitchListener\ndata: false\n");
              sse.println("event: pumpINSwitchListener\ndata: true\n");
              sse.println("event: pumpOUTSwitchListener\ndata: false\n");
            }  else if (elname == "pumpOUTSwitch") {
              logln("pumpOUTSwitch");
              pumpOut();
              sse.println("event: pumpOFFSwitchListener\ndata: false\n");
              sse.println("event: pumpINSwitchListener\ndata: false\n");
              sse.println("event: pumpOUTSwitchListener\ndata: true\n");
            }  else if (elname == "pumpOFFSwitch") {
              logln("pumpOFFSwitch");
              pumpOff();
              sse.println("event: pumpOFFSwitchListener\ndata: true\n");
              sse.println("event: pumpINSwitchListener\ndata: false\n");
              sse.println("event: pumpOUTSwitchListener\ndata: false\n");
            } else if (elname == "volume") {
              volume = (value.toFloat() * 255) / 100;
            } else if (elname == "cowbell") {
              cowbell = 1000 - (value.toInt() * 10);
            } else {
              goodSettings = false;
            }
            if (goodSettings) {
              sse.print("data: ");
              sse.print(elname);
              sse.print(" set to ");
              sse.println(value);
            } else {
              sse.println("Bad data; ignored");
            }
            sse.println("");
            gotSSE = true;
            Sprintln("--Client disconnected");
          } else if (header.indexOf("event-stream") > -1) {
            sse.stop();
            client.println("HTTP/1.1 200 OK");
            client.println("Connection: Keep-Alive");
#ifdef DEBUG
            client.println("Access-Control-Allow-Origin: *");
#endif
            client.println("Content-Type: text/event-stream");
            client.println("Cache-Control: no-cache");
            client.println("");
            sse = client;
            if(glider.deployState == DEPLOY_OFF){
              sse.println("event: deployONSwitchListener\ndata: false\n");
              sse.println("event: deployOFFSwitchListener\ndata: true\n");
            } else if(glider.deployState == DEPLOYED){
              sse.println("event: deployONSwitchListener\ndata: true\n");
              sse.println("event: deployOFFSwitchListener\ndata: false\n");
            } 
            sse.println("");
            gotSSE = true;
            Sprintln("--SSE client established");
            break;
          } else if (header.indexOf("POST / HTTP") > -1) {
            client.println("HTTP/1.1 200 OK");
            client.println("");
            client.stop();
            sse.stop();
            gotSSE = false;
            Sprintln("--Interface page was closed; all clients disconnected");
          } else if (header.indexOf("GET / HTTP") > -1) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Content-Encoding: gzip");
            client.println("");
            const int webpage_base64_length = sizeof(webpage_base64);
            const int webpage_gz_length = base64_dec_len(webpage_base64, webpage_base64_length);
            char webpage_gz[webpage_gz_length];
            base64_decode(webpage_gz, webpage_base64, webpage_base64_length);
            client.write(webpage_gz, webpage_gz_length);
            client.stop();
            Sprintln("--Interface webpage sent; client disconnected");
          } else if (header.indexOf("GET /author HTTP") > -1) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain; charset=utf-8");
            client.println("");
            client.println("John Reine  jreine@whoi.edu");
            client.stop();
            Sprintln("--Email address sent; client disconnected");
          } else {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Content-Type: text/plain; charset=utf-8");
            client.println("");
            client.println("404 Not Found");
            client.stop();
            Sprintln("--Page not found; client disconnected");
          }
        }
      }
    }
  }

}

void WiFiConnectAccessPoint() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    logln("Communication with WiFi module failed!");
    // don't continue
    //while (true);
    gotWebInterface = false;
  }
  else {
    String fv = WiFi.firmwareVersion();
    log("WiFi.firmwareVersion = ");
    logln(fv);
    if (fv < "1.0.0") {
      logln("Please upgrade the firmware");
    }

    log("Server starting.. ");
    logln(ssid);
    status = WiFi.beginAP(ssid);
    delay(9000);
    webTimer = millis();
    if (status != WL_AP_LISTENING) {
      logln("Creating access point failed");
      gotWebInterface = false;
    }
    else {
      gotWebInterface = true;
      server.begin();
      logln("server good");
      buzz(buzzerOnPin, 3500, 200);
      delay(100);
      buzz(buzzerOnPin, 3500, 200);
      // you're connected now, so print out the status:
      printWifiStatus();

    }
  }

}
void WiFiConnect() {
  while (WiFi.status() != WL_CONNECTED) {
    Sprint("Connecting to ");
    Sprintln(ssid);
    WiFi.setHostname("nano-webserver");
    WiFi.begin(ssid, pass);
    delay(5000);
  }
  WiFi.config(ip);
  Sprint("Connected to ");
  Sprintln(ssid);
  server.begin();
#ifdef DEBUG
  printWifiStatus();
#endif
  //WiFi.lowPowerMode();
  //digitalWrite(LED_BUILTIN, HIGH);
}

#ifdef DEBUG
void printWifiStatus() {
  Sprint("SSID: ");
  Sprintln(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Sprint("IP Address: ");
  Sprintln(ip);
  long rssi = WiFi.RSSI();
  Sprint("Signal strength (RSSI): ");
  Sprint(rssi);
  Sprintln(" dBm");
  Sprint("Server is at http://");
  Sprint(ip);
  Sprintln("/");
}
#endif
