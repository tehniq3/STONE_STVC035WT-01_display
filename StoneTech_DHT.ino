// original skech by Nicu FLORICA (niq_ro)
// DTH22 + STONE STVC035WT-01 display (see http://www.stone-hmi.com/product/278082427)

#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial screenserial(2, 3); // RX, TX

#include "DHT.h"
#define DHTPIN 4     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);

#define temperature_addr 0x01
#define humidity_addr 0x05

unsigned char temperature_send[8]= {0xA5, 0x5A, 0x05, 0x82, 0x00, temperature_addr, 0x00, 0x00};
unsigned char humidity_send[8]= {0xA5, 0x5A, 0x05, 0x82, 0x00, humidity_addr, 0x00, 0x00};

#define REPORTING_PERIOD_MS 5000
uint32_t tsLastReport = 0;

int temperature = 0;
int humidity = 0;
int nr;

void setup()
{
  screenserial.begin(115200);
  Serial.begin(9600);
  Serial.println("Initializing temperature/humidity station..");
}

void loop()
{ 
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    temperature = 10*dht.readTemperature();
    humidity = dht.readHumidity();
    Serial.print("Temperature:");
    Serial.print(temperature/10.);
    Serial.print("C, Relative humidity:");
    Serial.print(humidity);
    Serial.println("%RH");
    
if (temperature < 0)
{
  temperature = 65280 + temperature;
}
   temperature_send[6]=temperature/256;  
   temperature_send[7]=temperature%256;    
    screenserial.write(temperature_send,8);// send heart rate to display

   humidity_send[6]=humidity/256;  
   humidity_send[7]=humidity%256;    
    screenserial.write(humidity_send,8);// send oxygen level to display. 
   
   tsLastReport = millis();
  }

} // end main loop
