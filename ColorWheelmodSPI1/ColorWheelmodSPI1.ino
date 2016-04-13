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
#define FASTLED_ALLOW_INTERRUPTS 1
#include <RFduinoBLE.h>
#include "FastLED.h"
#include <SPI.h>

// Define PIN 5 to be the neopixel data
#define DATA_PIN 5
#define CLOCK_PIN 4
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

const float pi = 3.14159;

static CRGB COLOR = CRGB::Aqua;
static float SPEED = 128;
static float DELAY;
static bool CONNECTED = false;
static int FUNCTION = 127;
void setup() {
  //SPI.setFrequency(8000);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  
  // Create the FastLED object
  FastLED.addLeds< APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( 50 );
  FastLED.setMaxRefreshRate(650);
  // Flash the strip on
  wholeStrip(CRGB::White);FastLED.show(); delay(200);
  wholeStrip(CRGB::Blue); FastLED.show(); interrupts();delay(200);
  wholeStrip(CRGB::Red);FastLED.show();delay(200);
  wholeStrip(CRGB::Green);FastLED.show();delay(200);
  wholeStrip(CRGB::Black);FastLED.show();delay(200);
  // setup the leds Define the for output
  

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.deviceName = "Helios Hoop";
  //RFduinoBLE.advertisementInterval = 100;
  RFduinoBLE.advertisementData = "Hooper";
  //RFduinoBLE.txPowerLevel = -24;
  // start the BLE stack
  RFduinoBLE.begin();
  delay(100);



}

void loop() {
  if(CONNECTED){
   functionChooser(FUNCTION);
  }
  //else if(!CONNECTED){
  //  RFduino_ULPDelay(INFINITE);
  //}
}

void RFduinoBLE_onConnect() {
  CONNECTED = true;
  // the default starting color on the iPhone is white
  for(uint8_t i=0; i < 3; i++){
    wholeStrip(CRGB::White); FastLED.show();delay(20);
    wholeStrip(CRGB::Black); FastLED.show();delay(20);
  }
}

void RFduinoBLE_onDisconnect() {
  CONNECTED = false;
  FastLED.setBrightness( 255);
  for(uint8_t i=0;i<3;i++){
    wholeStrip(CRGB::Red);FastLED.show();delay(100);
    wholeStrip(CRGB::Black);FastLED.show();delay(50);
  }
  int COOLDOWN = 5*32;
  int COOLDELAY = 5000;
  for(uint8_t i=0; i < COOLDOWN; i++){
      FastLED.setBrightness( FastLED.getBrightness() * (COOLDOWN - i)/COOLDOWN );
      wholeStrip(CRGB::Red);FastLED.show(); delay(COOLDELAY/COOLDOWN);
  }
  wholeStrip(CRGB::Black);FastLED.show(); delay(500);
}
void RFduinoBLE_onReceive(char *data, int len) {
  // each transmission should contain an RGB triple  
  //delay(20);
  if (len >= 3)
  {
    // get the RGB values
    char *r = &data[0];
    char *g = &data[1];
    char *b = &data[2];
    COLOR = CRGB(*r, *g, *b);
    FUNCTION = 127;
    if (len >=6){
      char *r1 = &data[3];
      char *g1 = &data[4];
      char *b1 = &data[5];
      SPEED = (float)*g1;
      DELAY = 100000 / pow(500,(SPEED / 255) +1 );
      FUNCTION = (int)*b1;
      FastLED.setBrightness( *r1);
    }
  }
  delay(20);
}
// Which function to display
static void functionChooser(int choice){
  switch (choice){
    case 0:
      //DELAY = 50000 / pow(220,(SPEED / 255) +1 );
      chase(0);
      break;
    case 1:
      //DELAY = 50000 / pow(220,(SPEED / 255) +1 );
      chase(5);
      break;
    case 2:
      //DELAY = 50000 / pow(220,(SPEED / 255) +1 );
      chase(7);
      break;
    case 3:
      rainbow_sine();
      break;
    case 4:
      rainbow_HSV();
      break;
    case 127:
      wholeStrip();
      break;
    default:
      wholeStrip();
  }
}
// Set whole strip to stored color
static void wholeStrip(){
  for(uint16_t i=0; i < NUM_LEDS; i++){
    leds[i]=COLOR;
  }
  FastLED.show();
}
// Set whole strip to specified color
static void wholeStrip(CRGB c){
  for(uint16_t i=0; i < NUM_LEDS; i++){
    leds[i]=c;
  }
  FastLED.show();
}

// LED chase function for NeoPixel strip
static void chase(int trail) {
  for(int dot = 0; dot < NUM_LEDS+trail; dot++) { 
    if(dot < NUM_LEDS){
      leds[dot]=COLOR; // Draw new pixel
    }
      FastLED.show();
      leds[dot-trail] = CRGB::Black; // Erase pixel a few steps back
      delay(DELAY);
  }
}

// LED Rainbow sine function for NeoPixel strip
static void rainbow_sine() {
  uint8_t gHue = 0;
  uint8_t gHueDelta = 3;
  for(int i = 0; i < NUM_LEDS; i++){
    for(int dot = 0; dot < NUM_LEDS; dot++) { 
        leds[dot] = CRGB(255/2*sin((4*pi/NUM_LEDS)*(dot+(2*i+0))),55+255/4*sin((2*pi/NUM_LEDS)*(dot-(2*1+255%3))),255/6*sin((2*pi/NUM_LEDS)*(dot+(i+2*255%3))));   
    }
    FastLED.show();
    delay(DELAY);
  }
}
static void rainbow_HSV() {
  uint8_t gHue = 0;
  uint8_t gHueDelta = 3;
  for(int dot = 0; dot < NUM_LEDS; dot++){
    gHue += gHueDelta; // compute new hue value, automatically in range
    leds[dot].setHSV(gHue,0,0);
    FastLED.show();
    delay(DELAY);
  }
}
  
