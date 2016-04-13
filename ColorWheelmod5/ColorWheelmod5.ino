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

// Define PIN 5 to be the neopixel data
#define PIN 5
#define NUM_LEDS 120
CRGB leds[NUM_LEDS];

static CRGB color = CRGB::Aqua;
void setup() {
  
  pinMode(PIN, OUTPUT);
  
  // Create the FastLED object
  FastLED.addLeds< WS2812B, PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( 50 );
  FastLED.setMaxRefreshRate(120);
  // Flash the strip on
  wholeStrip(CRGB::White); noInterrupts(); FastLED.show(); interrupts();delay(200);
  wholeStrip(CRGB::Blue);noInterrupts(); FastLED.show(); interrupts();delay(200);
  wholeStrip(CRGB::Red);noInterrupts(); FastLED.show(); interrupts();delay(200);
  wholeStrip(CRGB::Green); noInterrupts(); FastLED.show(); interrupts();delay(200);
  wholeStrip(CRGB::Black);noInterrupts(); FastLED.show(); interrupts();delay(200);
  // setup the leds Define the for output
  

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.deviceName = "Helios Hoop";
  RFduinoBLE.advertisementInterval = 1000;
  RFduinoBLE.advertisementData = "Hooper";
  //RFduinoBLE.txPowerLevel = -24;
  // start the BLE stack
  RFduinoBLE.begin();
  delay(100);



}

void loop() {
  
    for(int dot = 0; dot < NUM_LEDS; dot++) { 
        leds[dot] = color;
        noInterrupts();
        FastLED.show();
        interrupts();
        // clear this led for the next time around the loop
        leds[dot] = CRGB::Black;
        delay(10);
    }
}

void RFduinoBLE_onConnect() {
  // the default starting color on the iPhone is white
  for(uint8_t i=0; i < 3; i++){
    wholeStrip(CRGB::White); noInterrupts(); FastLED.show(); interrupts();delay(20);
    wholeStrip(CRGB::Black); noInterrupts(); FastLED.show(); interrupts();delay(20);
  }
}

void RFduinoBLE_onDisconnect() {
  FastLED.setBrightness( 255);
    for(uint8_t i=0; i < 2; i++){
      FastLED.setBrightness( FastLED.getBrightness() * .2 );
      wholeStrip(CRGB::Red);noInterrupts(); FastLED.show(); interrupts();delay(20);
      wholeStrip(CRGB::Black);noInterrupts(); FastLED.show(); interrupts();delay(20);
  }
}
void RFduinoBLE_onReceive(char *data, int len) {
  // each transmission should contain an RGB triple
  if (len >= 6)
  {
    // get the RGB values
    char *r = &data[0];
    char *g = &data[1];
    char *b = &data[2];
    char *r1 = &data[3];
    char *g1 = &data[4];
    char *b1 = &data[5];
    FastLED.setBrightness( *r1);
    wholeStrip(CRGB(*r, *g, *b));
    noInterrupts(); FastLED.show();interrupts();
    delay(1000);
    color = CRGB(*r, *g, *b);
  }
}

// LED Constant For WHole Stripe
static void wholeStrip(CRGB c){
  for(uint16_t i=0; i < NUM_LEDS; i++){
    leds[i]=c;
  }
}
// LED chase function for NeoPixel strip
static void chase(CRGB c, uint16_t d) {
  for(uint16_t i=0; i < NUM_LEDS+10; i++) {
      c >> i;
      leds[i]=c; // Draw new pixel
      leds[i-10] = CRGB::Black; // Erase pixel a few steps back
      noInterrupts();
      FastLED.show();
      interrupts();
      delay(d);
  }
}
