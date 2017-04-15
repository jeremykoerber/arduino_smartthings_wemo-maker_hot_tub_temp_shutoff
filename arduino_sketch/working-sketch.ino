//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Shield LED Example 
//*****************************************************************************
#include <SoftwareSerial.h>   //TODO need to set due to some weird wire language linker, should we absorb this whole library into smartthings
#include <SmartThings.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_THING_RX    3
#define PIN_THING_TX    2


SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor


#define ONE_WIRE_BUS 5   // Data wire from temp probe is plugged into port 5 on the Arduino

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int maker_sensor = 4;    // Wemo Maker sensor input connects to port 4

bool isDebugEnabled;     // enable or disable debug



void setup()
{
  // setup default state of global variables
  isDebugEnabled = true;
  
  // setup hardware pins 
  pinMode(maker_sensor, OUTPUT);     // define maker_sensor pin as an output
  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
  }
  // Start up the sensor library
  sensors.begin();
  // set LED to blue for initial state
  smartthing.shieldSetLED(0, 0, 1);
}


void loop()
{
  // run smartthing logic
  smartthing.run();

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempFByIndex(0)); 
  if(sensors.getTempFByIndex(0) > 103)
    {
      Serial.println("TEMP > 103 DETECTED!");
      Serial.println("Shutting off Hot Tub Outlet!");
      maker_untrigger_contact_open();
    }
  else if(sensors.getTempFByIndex(0) < 102)
    {
      Serial.println("TEMP < 102 DETECTED.");
      Serial.println("Turning on Hot Tub Outlet!");
      maker_trigger_contact_close();
    }
  Serial.println("sleep 2 mins");
  delay(2*60000);
  smartthing.send(String(sensors.getTempFByIndex(0)));
}

void maker_untrigger_contact_open()
{ 
  analogWrite(maker_sensor,255);
  Serial.println("Wemo Maker status: not triggered, contact open");
  smartthing.shieldSetLED(1, 0, 0);
}

void maker_trigger_contact_close()
{ 
  analogWrite(maker_sensor,0);
  Serial.println("Wemo Maker status: triggered, contact closed");
  smartthing.shieldSetLED(0, 1, 0);
} 
  
void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }
}
