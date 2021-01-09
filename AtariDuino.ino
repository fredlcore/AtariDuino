/*
 * ATARIduino demo sketch (c) 2018 by Frederik Holst
 * Receives commands from the Atari via the R: device using Bob-Verter
 * 
 * This demo expects a 0x02 / 0x03 enclosed string and returns the result
 * of that command.
 * The following commands are implemented:
 * ATARIDUINO_REQ_WWW=
 * Returns a website (with HTML tags filtered)
 * ATARIDUINO_REQ_TEMP
 * Returns the temperature of a LM35DZ temperature sensor connected to pin A0
 * ATARIDUINO_REQ_TIME
 * Returns the number of milliseconds since booting the Arduino
 * 
 */

#include <SPI.h>
#include <Ethernet.h>

unsigned long timeout = millis();
char input[256] = {0};
char server[50] = {0};
char url[200] = {0};
int counter = 0;
boolean network_available = true;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient client;

void setup() {
  // initialize serial communication at 1200 bits per second:
  Serial.begin(1200);

  Serial.println("Waiting for network...");
  Serial.write(0x0d);
  int retries = 3;
  while (!Ethernet.begin(mac) && retries > 0) {
    delay(1000);
    retries--;
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    network_available = false;
  } else {
    Serial.println("Got network connection");
  }
  Serial.write(0x0d);
  Serial.flush();
  delay(1000);
}

void loop() {
  boolean process_input = false;
  while (Serial.available()) {
    byte in = Serial.read();
    input[counter] = in;
    counter++;
    if (counter > 256) {
      counter = 0;
    }
    if (in == 0x02) {
      counter = 0;
    }
    if (in == 0x03) {
      process_input = true;
    }
  }
  
  if (process_input == true) {
    input[counter] = '\0';
    if (strncmp(input, "ATARIDUINO_REQ_TIME", 18) == 0) {
//  Returns milliseconds since boot of Arduino
      Serial.print(millis());
      Serial.write(0x0d);
    } else if (strncmp(input, "ATARIDUINO_REQ_TEMP", 18) == 0) {
// Returns temperature from LM35DZ temperature sensor on pin A0
      int rawvoltage= analogRead(A0);
      float millivolts = (rawvoltage/1024.0) * 3300;
      float temp = millivolts/10;
      Serial.print(temp);
      Serial.print(" Grad Celsius");
      Serial.write(0x0d);
    } else if (strncmp(input, "ATARIDUINO_REQ_WWW=", 16) == 0) {
//  Returns website specified after "ATARIDUINO_REQ_WWW=" (no http:// !)
      if (network_available == false) {
        Serial.print("No network available");
      } else {
        strtok(input,"="); // discard Atariduino command
        strcpy(server, strtok(NULL, "/"));
        strcpy(url,strtok(NULL, "\x03"));
        strcpy(server, strtok(server, "\x03"));        
        int retries = 5;
        if (client.connect(server, 80)) {
          client.print("GET /"); client.print(url); client.println(" HTTP/1.1");
          client.print("Host: "); client.println(server);
          client.println("Connection: close");
          client.println();
          retries = 20;
          boolean skip_html = false;

          while (!client.available() && retries > 0) {
            retries--;
            delay(500);
          }
          while (client.available()) {
            char c = client.read();
            if (c == '<') { // filter HTML tags for output
              skip_html = true;
            }
            if (skip_html == false) {
              if (c == 0x0a) {
                Serial.write(0x0d);
              } else {
                Serial.write(c);
              }
            }
            if (c == '>') { // HTML tag is done, continue printing...
              skip_html = false;
            }
          }
          client.stop();
        }
      }
      Serial.write(0x0d);
    } else {
//  None of the above? Return an error message
      Serial.print("Unknown command: ");
      Serial.print(input);
      Serial.write(0x0d);
    }
    counter = 0;
  }
}
