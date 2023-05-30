#include <Arduino.h>
//#include "ThingSpeak.h"
//#include "WiFiManager.h"
#include "ESP8266WiFi.h"    // Incluimos la librería encargada de las conexiones wifi para los ESP8266

String apiKey = "EUZSHHU9VD74S8PO";
const char* ssid = "Depto 2.4GHz";
const char* password = "BocaJuniors12";
const char* server = "api.thingspeak.com";

WiFiClient client; 
  
const char startlimitetemp = '<';         
const char endlimitetemp   = '>';

bool humedad = false;
bool capacidad = false;
void processInput ();
void processtemp(int);
void processhume(int);
void processcap(int);
float temp;
float hume;
float cap;

int t = 0; 
int tiempo;

  
   
 
void setup () { 
  Serial.begin (9600);
  delay(10);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");

}


void loop (){
  
    while (Serial.available ())
        processInput ();


       if (client.connect(server,80)) {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(temp);
    postStr +="&field2=";
    postStr += String((int)hume);
    postStr +="&field3=";
    postStr += String((int)cap);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("% send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting…");
  delay(20000);
      
     } 
  


 void processInput () {
  static long recivetemp = 0;
  static long recivehume = 0;
  static long recivecapa = 0;
  static boolean negative = false;
  
  byte c = Serial.read ();
  
  switch (c)
    {
      
    case endlimitetemp:  
      if (negative) {
        processtemp (- recivetemp); 
        
      }
      else{
        processtemp (recivetemp); 
        processhume (recivehume);
        processcap (recivecapa);
      }

    case 'h':  
     humedad = true;
     capacidad = false; 
    break;
    case 'c':
     capacidad = true;
     humedad = false; 
     
      break;
    case startlimitetemp: 
      recivetemp = 0; 
      recivehume = 0;
      recivecapa = 0;
      negative = false;
      humedad = false;
      capacidad = false;
      break;
      
    case '0' ... '9': 
    if (humedad==true){
      recivehume *= 10;
      recivehume += c - '0';
    }
    else if(capacidad == true){
       recivecapa *= 10;
       recivecapa += c - '0';
    }
  else{
      recivetemp *= 10;
      recivetemp += c - '0';
    }
      break;
      
    case '-':
      negative = true;
      break;
      
    } 


  }  

  void processtemp (int n)
  {
  temp = n;
  temp =temp/10;
  Serial.print ("temperatura: ");
  Serial.println (temp,1);
   
  }  

  void processhume (int n)
  {
  hume = n;
  hume =hume/10;
  Serial.print ("humadead: ");
  Serial.println (hume,1);
 
  }  

   void processcap (int n)
  {
   cap = n;
  cap = cap/100;
  Serial.print ("capacidad: ");
  Serial.println (cap,1);
  
  }  