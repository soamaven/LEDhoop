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

// pin 2 on the RGB shield is the red ledadafruit bluefruit le connect control pad
int led1 = 2;
int RED_LED_PIN = *&led1;
// pin 3 on the RGB shield is the green led
int led2 = 3;
int GREEN_LED_PIN = *&led2;
// pin 4 on the RGB shield is the blue led
int led3 = 4;
int BLUE_LED_PIN = *&led3;

void setup() {
  // setup the leds for output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);  
  pinMode(led3, OUTPUT);
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
  
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "rgb";
  
  // start the BLE stack
  RFduinoBLE.begin();

}

void loop() {
  // switch to lower power mode
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
  int sizedata = sizeof(data);
  Serial.println(sizedata);
  if (len >= 3)
  {
    // get the RGB values
    uint8_t r = data[0];
    uint8_t g = data[1];
    uint8_t b = data[2];

    // set PWM for each led
    analogWrite(led1, r);
    analogWrite(led2, g);
    analogWrite(led3, b);
  }
}
