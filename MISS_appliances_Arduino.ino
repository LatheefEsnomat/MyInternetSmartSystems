#include <ESP8266WiFi.h>
#include <FS.h>

//Wi-Fi name and pass here
const char *ssid = "MISS";
const char *password = "misspass";

const int numLeds = 8;
int ledPins[numLeds] = {D0,D1};v

WiFiServer server(80);

void serveHtmlFile(WiFiClient client, const char *filename);
void handleLEDControl(WiFiClient client, String request);

void setup()
{
  Serial.begin(115200);
  delay(10);

  for (int i = 0; i < numLeds; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/index.html") != -1)
  {
    // Serve the HTML file
    serveHtmlFile(client, "/index.html");
  }
  else
  {
    // Handle LED control requests
    handleLEDControl(client, request);
  }

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}

void serveHtmlFile(WiFiClient client, const char *filename)
{
  // Open the HTML file for reading
  File file = SPIFFS.open(filename, "r");

  if (!file)
  {
    Serial.println("Failed to open file");
    return;
  }

  // Send the contents of the HTML file to the client
  while (file.available())
  {
    client.write(file.read());
  }
  file.close();
}

void handleLEDControl(WiFiClient client, String request)
{
  int value = LOW;

  for (int i = 0; i < numLeds; i++)
  {
    if (request.indexOf("/LED" + String(i) + "=ON") != -1)
    {
      digitalWrite(ledPins[i], HIGH);
      value = HIGH;
    }
    else if (request.indexOf("/LED" + String(i) + "=OFF") != -1)
    {
      digitalWrite(ledPins[i], LOW);
      value = LOW;
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>LED Control Panel</title>");
  client.println("<style>");
  client.println("body { font-family: Arial, sans-serif; }");
  client.println(".card { border: 1px solid #ccc; padding: 15px; margin: 15px; width: 200px; text-align: center; display: inline-block;}");
  client.println(".button-container { margin-top: 10px; }");
  client.println("button { padding: 10px; margin: 5px; font-size: 14px; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");

  // Add Cards with On/Off buttons for each LED
  for (int i = 0; i < numLeds; i++)
  {
    client.println("<div class='card'>");
    client.print("<h3>Device ");
    client.print(i + 1);
    client.println(" Control</h3>");
    client.print("LED is now: ");
    client.print(digitalRead(ledPins[i]) == HIGH ? "On" : "Off");
    client.println("<br>");

    // On button
    client.print("<button onclick=\"window.location.href='/LED" + String(i) + "=ON'\">Turn On</button>");

    // Off button
    client.print("<button onclick=\"window.location.href='/LED" + String(i) + "=OFF'\">Turn Off</button>");

    client.println("</div>");
  }

  client.println("</body>");
  client.println("</html>");
}
