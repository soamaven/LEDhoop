#include <RFduinoBLE.h>

// Define input/output pins.
// Input pins for buttons, and output pins for LED RGB On-Off control
// GPIO2 on the RFduino RGB shield is the Red LED
// GPIO3 on the RFduino RGB shield is the Green LED
// GPIO4 on the RFduino RGB shield is the Blue LED

#define RED_LED_PIN   2
#define GREEN_LED_PIN 3
#define BLUE_LED_PIN  4

void setup() {
  // Enable outputs.
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  
  // Enable serial debug, type cntrl-M at runtime.
  Serial.begin(9600);
  Serial.println("RFduino example started");
  Serial.println("Serial rate set to 9600 baud");
  
  // Turn Off all LEDs initially
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  
  // Indicate RGB LED is operational to user.
  digitalWrite(RED_LED_PIN, HIGH);    // red
  delay (500);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);  // green
  delay (500);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, HIGH);   // blue
  delay (500);
  digitalWrite(RED_LED_PIN, LOW);     // lights out
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  // configure the RFduino BLE properties
  RFduinoBLE.advertisementData = "ledbtn";
  RFduinoBLE.advertisementInterval = 500;
  RFduinoBLE.deviceName = "RFduino";
  RFduinoBLE.txPowerLevel = -20;
  Serial.println("RFduino BLE Advertising interval is 500ms");
  Serial.println("RFduino BLE DeviceName: RFduino");
  Serial.println("RFduino BLE Tx Power Level: -20dBm");
  
  // start the BLE stack
  RFduinoBLE.begin();
  Serial.println("RFduino BLE stack started");

  

}

void loop() {
  // switch to lower power mode
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onAdvertisement()
{
  Serial.println("RFduino is doing BLE advertising ...");
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void RFduinoBLE_onConnect()
{
  Serial.println("RFduino BLE connection successful");
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void RFduinoBLE_onDisconnect()
{
  Serial.println("RFduino BLE disconnected");
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void RFduinoBLE_onReceive(char *data, int len)
{
  // If the first byte is 0x01 / on / true
  Serial.println("Received data over BLE");
  if (data[0])
  {
    digitalWrite(BLUE_LED_PIN, HIGH);
    Serial.println("Turn RFduino Blue LED On");
  }
  else
  {
    digitalWrite(BLUE_LED_PIN, LOW);
    Serial.println("Turn RFduino Blue LED Off");
  }
}

