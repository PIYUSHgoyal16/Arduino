#include <ESP8266WiFi.h>

int input1 = 5; //D1
int input2 = 4; //D2
int firstInter=0;
int secondInter=0;
String apiWritekey = "K6NIWBKALS56O2CS"; // replace with your THINGSPEAK WRITEAPI key here
const char* ssid = "MyWifi"; // your wifi SSID name
const char* password = "06092002" ;// wifi pasword
float count=0;

const char* server = "api.thingspeak.com";
float resolution=3.3/1023;// 3.3 is the supply volt  & 1023 is max analog read value
WiFiClient client;
 
void setup() {
  pinMode(input2, INPUT);
  pinMode(input1, INPUT);
  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
}
 
void loop() {
  firstInter= digitalRead(input1);
  Serial.print("First INTER : ");
  Serial.print(firstInter);
  Serial.print("\n");

  
  while(firstInter == 1)
  {
      secondInter = 0;
      Serial.print("First Detected \n");
        for (int i=0;i<100;i++)
        {
              secondInter=digitalRead(input2);
              Serial.print("Second INTER : ");
              Serial.println(secondInter);
              
              if(secondInter == 1)
              {
                     for(int j=0;j<100;j++)
                     {
                      delay(30);
                     }  
                     i = 0;
                     Serial.print("First: ");
                     Serial.println(firstInter);
                     Serial.print("Second: ");
                     Serial.println(secondInter);
                     count++;
              }
              else
              {
                Serial.println("Car undetected");
              }
              Serial.print("First: ");
              Serial.println(firstInter);
              Serial.print("Second: ");
              Serial.println(secondInter);
              Serial.print("Loop no.: ");
              Serial.println(i);
              delay(30);
        }
        firstInter = 0;
    
  }

  
  if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(count);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);
 
     Serial.print("Count: ");
     Serial.print(count);
     Serial.println("uploaded to Thingspeak server....");
  }
  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak needs minimum 15 sec delay between updates
  delay(1500);
}
