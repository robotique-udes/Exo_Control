#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "*Your SSID*";         /*Enter Your SSID*/
const char* password = "*Your Password*"; /*Enter Your Password*/

WiFiServer server(80); /* Instance of WiFiServer with port number 80 */
String request;
#define  LED 21

int LED_Status; 

WiFiClient client;

void setup() 
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi ");
  Serial.println(WiFi.SSID());
  delay(1000);
  server.begin(); /* Start the HTTP web Server */
  Serial.print("Connect to IP Address: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
}

void html (){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<head>");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    client.println("<style>");
      client.println("html { font-family: Roboto; display: inline-block; margin: 0px auto; text-align: center;}");
      client.println(".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;");
      client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}");
      client.println(".button_ON {background-color: white; color: black; border: 2px solid #4CAF50;}");
      client.println(".button_OFF {background-color: white; color: black; border: 2px solid #f44336;}");
    client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h2>ESP32 WiFi Station Mode</h2>");
  client.println("<p>Click to Turn ON and OFF the LED</p>");

  if(LED_Status == LOW) 
  {
    client.print("<p><a href=\"/LED_ON\n\"><button class=\"button button_ON\">ON</button></a></p>"); 
  } 
 
  else
  {
    client.print("<p><a href=\"/LED_OFF\n\"><button class=\"button button_OFF\">OFF</button></a></p>"); 
  } 
 
  client.println("</body>");
 client.println("</html>");     
}

void loop()
{
  client = server.available();
  if(!client)
  {
    return;
  }

  while (client.connected())
  {
    if (client.available())
    {
      char c = client.read();
      request += c;

      if (c == '\n')
      {
        if (request.indexOf("GET /LED_ON") != -1) 
        {
          Serial.println("LED in ON");
          digitalWrite(LED, HIGH);
          LED_Status = HIGH;
        }

        if (request.indexOf("GET /LED_OFF") != -1)
        {
          Serial.println("LED in OFF");
          digitalWrite(LED, LOW);
          LED_Status = LOW;
        }
        html();
        break;
    }
    }
  }

  delay(1);
  request="";
  client.stop();
}