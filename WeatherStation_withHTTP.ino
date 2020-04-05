#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C
float temp_sensor;
const char* ssid = "SSID";
const char* password = "PWD";
String context;
/* REST ENDPOINT htat expects json with:
* {'temperature': ['22.6'], 'pressure': ['859'], 'room': ['bedroom']}
*/
const char* endpoint = "http://traabant.pythonanywhere.com/api/weather/save-home-temps"; 

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  // Initialize Serial port
  Serial.begin(9600);
  while (!Serial) continue;

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  delay(1000);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(500);
  
  Serial.println("\nConecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(""); 
  Serial.println("WiFi connected");
  digitalWrite(BUILTIN_LED, LOW);
  delay(500);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED, LOW);
  delay(500);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(500);

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

}

void loop() {


  if (bmp.begin(0x76)){

  context = "";
  temp_sensor = bmp.readTemperature();
  float temp = temp_sensor;

  /* 
   * preparing string for POST message  
   * using application/x-www-form-urlencoded
   * from https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST
  */
  context = "temperature=22&pressure=0&room=outside";
  context = "temperature=" + String(temp) + "&pressure=0&room=outside";
  
  Serial.println(context);
  Serial.println("sending HTTP POST");

  
  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){   
     //Declare object of class HTTPClient
     HTTPClient http;    
  
     //Specify request destination
     http.begin(endpoint);      
     //Specify content-type header
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");     
  
     //Send the request
     int httpCode = http.POST(context);   
     //Get the response payload
     String payload = http.getString();                  
   
     Serial.println(httpCode);   //Print HTTP return code
     Serial.println(payload);    //Print request response payload
  
     //Close connection
     http.end();  
    }
  }
  else{
     while (1);
    }
  // Start a new line
  Serial.println();
  delay(300000);
}
