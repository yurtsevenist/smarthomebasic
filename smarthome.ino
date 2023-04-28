
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Servo.h> // including servo library.
#define FIREBASE_HOST "mysmart-bd8b8.firebaseio.com"
#define FIREBASE_AUTH "xHD331r9slRy28z1wgFsZreQdxOO75mbts2WP9Zj"
//#define WIFI_SSID "f@tih"
//#define WIFI_PASSWORD "!1qaz2WSX3edc4RFV%56" 
#define WIFI_SSID "my"
#define WIFI_PASSWORD "mustafa2549" 
//WiFiServer server(80);

int deger=0;
Servo kapi;/* D8 SERVO KONTROL*/
int klima=1;//D3
int blue = 13;  /* D7 RGB MAVİ*/
int green = 14; /* D5 RGB YEŞİL*/
int red = 12;  /* D6 RGB KIRMIZI  */
int fbase=16;//D0 //FIREBASE BAĞLANDIĞINDA YANACAK
int bahce=5;//D1;// BAHÇE IŞIKLARI KONTROL
//int mutfak=4;//D2;//MUTFAK IŞIK KONTROL
//int salon= 2;     /* D4 SALON IŞIK KONTROL  */  
//int perde = 0;    /* D3   */
//int oda_1=3;//RX
//int oda_2 = 10;    /* S3   */
//int oda_3 = 9;    /* S2   */
//String device;
//String stat;
FirebaseData data;
//unsigned long eskiZaman=0;
//unsigned long yeniZaman;
String r,g,b;
void setup() {
  Serial.begin(115200);
  kapi.attach(15);//servo D8 e bağlanacak
  //pinMode(oda_1, OUTPUT);
    //pinMode(oda_2, OUTPUT);
      //pinMode(oda_3, OUTPUT);
      //  pinMode(salon,OUTPUT);
       //   pinMode(mutfak, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(fbase, OUTPUT);  
  pinMode(bahce, OUTPUT);
  pinMode(klima, OUTPUT);
 
    delay(1000);
    analogWrite(red,0);
    analogWrite(green,0);
    analogWrite(blue,0);
    digitalWrite(bahce, LOW);
    digitalWrite(klima, LOW);
    digitalWrite(fbase,LOW);

    kapi.write(180);
       // digitalWrite(salon,LOW);
      // digitalWrite(mutfak, LOW);
      // digitalWrite(oda_1,LOW);    
    //digitalWrite(oda_3,LOW);
    //digitalWrite(oda_2,LOW);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.print("Connecting");
 
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Start the server
  //server.begin();
  Serial.println("Server started");
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  sifirla();
  digitalWrite(fbase, HIGH);

}
   void sifirla(){
      
    
      
  
      Firebase.setString(data,"klima/status","false");  
      
      Firebase.setString(data,"kapi/status","false");
        Firebase.setString(data,"rgb1/value","0");
    Firebase.setString(data,"rgb2/value","0");
    Firebase.setString(data,"rgb3/value","0");
       // Firebase.setString(data,"bahce/status","false");
      //Firebase.setString(data,"mutfak/status","false");
       // Firebase.setString(data,"oda1/status","false");
        //Firebase.setString(data,"oda2/status","false");
         // Firebase.setString(data,"oda3/status","false");
        // Firebase.setString(data,"salon/status","false");
        // Firebase.setString(data,"perde/value","0");
   }
void loop() {  
 
    Firebase.getString(data,"bahce/status");     
    Serial.println("bahçe değer "+ data.stringData());
    if(data.stringData()=="true")
    {
     digitalWrite(bahce, HIGH);    
    
    }
    else 
    {
      digitalWrite(bahce, LOW);
       analogWrite(red,0);
     analogWrite(green,0);
     analogWrite(blue,0);    
    }
   /* Firebase.getString(data,"salon/status");     
    Serial.println("salon değer "+ data.stringData());
     if(data.stringData()=="true")
    {
    digitalWrite(salon, HIGH);
    }
    else 
    {
      digitalWrite(salon, LOW);
    }
     Firebase.getString(data,"mutfak/status"); 
    Serial.println("mutfak değer "+ data.stringData());
    if(data.stringData()=="true")
    {
    digitalWrite(mutfak, HIGH);
    }
    else
    {
      digitalWrite(mutfak, LOW);
    }*/
    Firebase.getString(data,"kapi/status"); 
    Serial.println("kapı değer "+ data.stringData());
    if(data.stringData()=="true")
    {
      kapi.write(5);
        
    }
    else
    {
       kapi.write(175);
     
    }
    Firebase.getString(data,"klima/status"); 
    Serial.println("klima değer "+ data.stringData());
    if(data.stringData()=="true")
    {
       digitalWrite(klima, HIGH);
    }
    else
    {
      digitalWrite(klima, LOW);
    }
    // Firebase.getString(data,"perde/value"); 
    //Serial.println("perde değer "+ data.stringData());
    Firebase.getString(data,"rgb1/value"); 
    Serial.println("rgb1 değer "+ data.stringData());
    r= data.stringData();
    Firebase.getString(data,"rgb2/value"); 
    Serial.println("rgb2 değer "+ data.stringData());
    g= data.stringData();
    Firebase.getString(data,"rgb3/value"); 
    Serial.println("rgb3 değer "+ data.stringData());    
    b= data.stringData();
    analogWrite(red,r.toInt());
    analogWrite(green,g.toInt());
    analogWrite(blue,b.toInt());  
   
   
}
/*void serverkontrol()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  //int sensorValue = analogRead(A0);
  String m;
  //API for device one off
  if (req.indexOf("/kapi/0") != -1){
    device="kapı";
    stat = "kapat";
    kapi.write(0);
  }
  //API for device one on
  else if (req.indexOf("/kapi/1") != -1){
    device="kapı";
    stat = "aç";
    kapi.write(180);   
  }
  //API for device two off
  else if (req.indexOf("/salon/0") != -1){
    device="salon";
    stat = "kapat";
    digitalWrite(salon, 0);
  }
  //API for device two off
  else if (req.indexOf("/salon/1") != -1){
    device="salon";
    stat = "aç";
    digitalWrite(salon, 1);
  }
   //API for device three off
  else if (req.indexOf("/oda1/0") != -1){
    device="oda1";
    stat = "kapat";
    digitalWrite(oda_1, 0);
  }
  //API for device three off
  else if (req.indexOf("/oda1/1") != -1){
    device="oda1";
    stat = "kapat";
    digitalWrite(oda_1, 1);
  }
   //API for device four off
  else if (req.indexOf("/bahce/0") != -1){
    device="bahçe";
    stat = "kapat";
    digitalWrite(bahce, 0);
  }
  //API for device four off
  else if (req.indexOf("/bahce/1") != -1){
    device="bahçe";
    stat = "kapat";
    digitalWrite(bahce, 1);
  }
   //API for device five off
  else if (req.indexOf("/perde/0") != -1){
    device="perde";
    stat = "kapat";
    digitalWrite(perde, 0);
  }
  //API for device five off
  else if (req.indexOf("/perde/1") != -1){
    device="perde";
    stat = "kapat";
    digitalWrite(perde, 1);
  }
     //API for device six off
  else if (req.indexOf("/mutfak/0") != -1){
    device="perde";
    stat = "kapat";
    digitalWrite(mutfak, 0);
  }
  //API for device five off
  else if (req.indexOf("/mutfak/1") != -1){
    device="perde";
    stat = "kapat";
    digitalWrite(mutfak, 1);
  }
  //API for reading sensor data
 // else if (req.indexOf("/A0") != -1){
   // device="Sensor";
    //stat = sensorValue;
  //}
  else {
    Serial.println("invalid request");
    client.stop();
    m = "Invalid Request";
  }

  
  
  client.flush();

  // Prepare the response

  // Send the response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println("");
   
  //send json response
  client.print("{\""+device+"\": \""+stat+"\"}");
  
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}


void oda1kontrol()
{
   Firebase.getString(data,"oda1/status"); 
    Serial.println("oda1 değer "+ data.stringData());
    if(data.stringData()=="true")
    {
    digitalWrite(oda_1, HIGH);
    }
    else
    {
      digitalWrite(oda_1, LOW);
    }
}
void oda2kontrol()
{
   Firebase.getString(data,"oda2/status"); 
    Serial.println("oda2 değer "+ data.stringData());
    if(data.stringData()=="true")
    {
    digitalWrite(oda_2, HIGH);
    }
    else
    {
      digitalWrite(oda_2, LOW);
    }
}
void oda3kontrol()
{
   Firebase.getString(data,"oda3/status"); 
    Serial.println("oda3 değer "+ data.stringData());
    if(data.stringData()=="true")
    {
    digitalWrite(oda_3, HIGH);
    }
    else
    {
      digitalWrite(oda_3, LOW);
    }
}*/
