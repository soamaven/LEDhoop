/*
The sketch receives a Bluetooth Low Energy 4 connection,
then uses PWM on the RGB shield to approximate the color
selected.

This sketch is supposed to be used with the rfduinoColorWheel
iPhone application.
*/

/*
 Copyright (c) 2014 OpenSourceRF.com.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <RFduinoBLE.h>
//#include <Adafruit_NeoPixel_RFduino.h>


// pin 2 on the RGB shield is the red ledadafruit bluefruit le connect control pad
int led1 = 2;
int RED_LED_PIN = *&led1;
int r = 100;
// pin 3 on the RGB shield is the green led
int led2 = 3;
int GREEN_LED_PIN = *&led2;
int g =100;
// pin 4 on the RGB shield is the blue led
int led3 = 4;
int BLUE_LED_PIN = *&led3;
int b =100;
// Define PIN 5 to be the neopixel data
const int PIN =5;
const int nPIXELS =60;
const int nLEDs = nPIXELS * 3;
uint8_t ledBar[nLEDs];
uint8_t brightness = 100;
// not compatible with RFduino yet
//Adafruit_NeoPixel_RFduino strip = Adafruit_NeoPixel_RFduino(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // setup the leds for output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);  
  pinMode(led3, OUTPUT);

  // for use with neopixel like functions below
  pinMode(PIN, OUTPUT);
  //digitalWrite(PIN, LOW);
  clearStrip();
  // Enable serial debug, type cntrl-M at runtime.
  Serial.begin(9600,0,1);
  Serial.println("RFduino example started");
  Serial.println("Serial rate set to 9600 baud");
  // Turn Off all LEDs initially
  //digitalWrite(RED_LED_PIN, LOW);
  //digitalWrite(GREEN_LED_PIN, LOW);
  //digitalWrite(BLUE_LED_PIN, LOW);
  
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
  
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.deviceName = "Helios Hoop";
  RFduinoBLE.advertisementData = "hello";
  RFduinoBLE.txPowerLevel = -20;
  // start the BLE stack
  RFduinoBLE.begin();
  //Serial.println("The BLE stack has started");
  //strip.begin();
  //strip.setPixelColor(1, strip.Color(255,0,0));
  //Serial.println("The NeoPixel strip has started");

}

void loop() {
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onConnect() {
  // the default starting color on the iPhone is white
  analogWrite(led1, 255);
  analogWrite(led2, 255);
  analogWrite(led3, 255);
}

void RFduinoBLE_onDisconnect() {
  // turn all leds off on disconnect and stop pwm
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void RFduinoBLE_onReceive(char *data, int len) {
  // each transmission should contain an RGB triple
  if (len >= 6)
  {
    // get the RGB values
    uint8_t r = data[0];
    uint8_t g = data[1];
    uint8_t b = data[2];
    uint8_t r1 = data[3];
    uint8_t g1 = data[4];
    uint8_t b1 = data[5];

    // set PWM for each led
    analogWrite(led1, r);
    analogWrite(led2, g);
    analogWrite(led3, b);
    chase(Color(r,g,b),10);
    clearStrip();
    delay(1000);
    chase(Color(r1,g1,b1),10);
    clearStrip();
    wholeStrip(Color(r, g, b));
    //delay(1000);
    //wholeStrip(Color(r1, g1, b1));    
    //clearStrip();
  }
}


// LED chase function for NeoPixel strip
static void chase(uint32_t c, uint8_t d) {
  for(uint16_t i=0; i < nPIXELS+10; i++) {
      c >> i;
      setPixelColor(i  , c); // Draw new pixel
      setPixelColor(i-10, 0); // Erase pixel a few steps back
      showStrip();
      delay(d);
  }
}

// LED Constant For WHole Stripe
static void wholeStrip(uint32_t c){
  for(uint16_t i=0; i < nPIXELS; i++){
    setPixelColor(i , c);
  }
  showStrip();
}

// ============================
// NeoPixel API like functions:
// ============================

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

void setPixelColor(int pixel, uint32_t color) {
  uint8_t red = (uint8_t)(color >> 16);
  uint8_t green = (uint8_t)(color >>  8);
  uint8_t blue = (uint8_t)color;
  
  red = (red * brightness) >> 8;
  green = (green * brightness) >> 8;
  blue = (blue * brightness) >> 8;
  
  setPixelColor(pixel, red, green, blue);
}

void setPixelColor(int pixel, byte red, byte green, byte blue) {
  ledBar[3*pixel] = green;
  ledBar[3*pixel+1] = red;
  ledBar[3*pixel+2] = blue;
}


void clearStrip() {
  for(uint32_t i; i<64; i++) {
    setPixelColor(i, 0);
  }
  showStrip();
}

void showStrip(){
  noInterrupts();
  for(int wsOut = 0; wsOut < nLEDs; wsOut++){
    for(int x=7; x>=0; x--){
      NRF_GPIO->OUTSET = (1UL << PIN);
      if(ledBar[wsOut] & (0x01 << x)) {
        __ASM ( \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              );
        NRF_GPIO->OUTCLR = (1UL << PIN);
      
      }else{
        NRF_GPIO->OUTCLR = (1UL << PIN);
        __ASM ( \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              );      
      }
    }
  }
  delayMicroseconds(50); // latch and reset WS2812.
  interrupts();  
}
