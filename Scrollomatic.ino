#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// initialize OLED screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// initialize web server
WebServer server(80);

bool textReceived = false;
String text;

void setup() {
  // initialize serial communication
  Serial.begin(115200);

  // initialize OLED screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  // set rotation to portrait mode
  display.setRotation(0);

  // clear OLED screen
  display.clearDisplay();
  display.display();

  WiFi.softAP("Marquee_Screen");
  
  server.on("/", HTTP_GET, []() {
    String html = "<html><body>";
    html += "<h1>ESP32 OLED Web Server</h1>";
    html += "<div id='message'></div>";
    html += "<form method='POST' id='form'>";
    html += "<input type='text' name='text' placeholder='Enter text'>";
    html += "<button type='submit'>Submit</button>";
    html += "</form>";
    html += "<script>";
    html += "var form = document.getElementById('form');";
    html += "var message = document.getElementById('message');";
    html += "form.addEventListener('submit', function(event) {";
    html += "  event.preventDefault();";
    html += "  var formData = new FormData(form);";
    html += "  fetch('/', {";
    html += "    method: 'POST',";
    html += "    body: formData";
    html += "  })";
    html += "  .then(function(response) {";
    html += "    return response.text();";
    html += "  })";
    html += "  .then(function(text) {";
    html += "    message.innerText = text;";
    html += "  });";
    html += "});";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });
  // define web server route for receiving text input

  server.on("/", HTTP_POST, []() {
    text = server.arg("text");
    textReceived = true;
    server.send(200, "text/plain", "Received: " + text);
  });

  // start web server
  server.begin();
}

void loop() {

  // display IP address on OLED screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("");
  display.println("Access Point Name: ");
  display.println("");
  display.print(WiFi.softAPSSID());
  display.println("");
  display.println("");
  display.println("IP Address: ");
  display.println("");
  display.print(WiFi.softAPIP());


  // initialize scrolling text variables
  String scrollingText = "";
  int textY = SCREEN_HEIGHT + 2;
  int textSpeed = 1;

  while (true) {
    // check for new text input
    if (textReceived) {
      // update scrolling text
      scrollingText = text;
      textReceived = false;
      textY = SCREEN_HEIGHT;
    }

    // calculate number of lines needed to display text
    int numLines = ceil(scrollingText.length() / 21.0) + 1;
    numLines += 2; // Add 2 lines to the number of lines

    // display scrolling text on OLED screen
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, textY);

    // handle scrolling
    if (textY < -((numLines) * 8)) {
      textY = SCREEN_HEIGHT;
    }
    display.println(scrollingText.substring(0, 21));
    for (int i = 1; i < numLines; i++) {
      if (textY + (i * 8) > -16) {
        display.setCursor(0, textY + (i * 8));
        display.println(scrollingText.substring(i * 21, (i * 21) + 21));
      }
    }

    // update scrolling text position
    textY -= textSpeed;

    // handle incoming web requests
    server.handleClient();
    
    // check for empty text input and display IP address
    if (text.isEmpty()) {
      display.setCursor(0, 0);
      display.println("");      
      display.println("Access Point Name: ");
      display.println("");
      display.print(WiFi.softAPSSID());
      display.println("");
      display.println("");
      display.println("IP Address: ");
      display.println("");
      display.print(WiFi.softAPIP());
    }

    // display the OLED screen
    display.display();

    // slow down scrolling text
    delay(50);
  }
}
