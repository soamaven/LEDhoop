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
#define FASTLED_ALLOW_INTERRUPTS 0
#include <RFduinoBLE.h>
#include "FastLED.h"

// Define PIN 5 to be the neopixel data
const int PIN =5;
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

void setup() {

  Serial.begin(9600,0,1);
  Serial.println("RFduino example started");
  Serial.println("Serial rate set to 9600 baud");
  // setup the leds Define the for output
  pinMode(PIN, OUTPUT);

  // Create the FastLED object
  FastLED.addLeds<WS2812, PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( 100 );
  //FastLED.setMaxRefreshRate( 800 );
  
  wholeStrip(CRGB::White); FastLED.show(); delay(100);
  wholeStrip(CRGB::Blue); FastLED.show(); delay(100);
  wholeStrip(CRGB::Red); FastLED.show(); delay(100);
  wholeStrip(CRGB::Green); FastLED.show(); delay(100);
  wholeStrip(CRGB::Black); FastLED.show(); delay(100);
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.deviceName = "Helios Hoop";
  RFduinoBLE.advertisementData = "hello";
  RFduinoBLE.txPowerLevel = -8;
  // start the BLE stack
  RFduinoBLE.begin();
  Serial.println("The BLE stack has started");


}

void loop() {
    leds[0] = CRGB::Blue;
 FastLED.show(); delay(50);
    leds[0] = CRGB::Black;
    FastLED.show(); delay(50);
}

void RFduinoBLE_onConnect() {
  // the default starting color on the iPhone is white
  for(uint8_t i=0; i < 3; i++){
    wholeStrip(CRGB::White); FastLED.show(); delay(100);
    wholeStrip(CRGB::Black); FastLED.show(); delay(100);
  }

}

void RFduinoBLE_onDisconnect() {

}


// LED Constant For WHole Stripe
static void wholeStrip(CRGB c){
  for(uint16_t i=0; i < NUM_LEDS; i++){
    leds[i]=c;
  }
}
