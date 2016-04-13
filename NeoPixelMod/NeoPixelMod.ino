// WS2812_RFduino_Test
// By Thomas Olson
// teo20140220.01
// teo20140719.01 Modified for Arduino 1.5.7
// 20141022.. verified works with Arduino 1.5.8
// No complicated Pixel Library needed.
// Tested with WS2812B 4 pin versions.
// Modified by Sam Decrock to resemble NeoPixel API

const int ws2812pin = 6;
const int nPIXELS = 60;

const int nLEDs = nPIXELS * 3;
uint8_t ledBar[nLEDs];
uint8_t brightness = 20;

void setup() {
  pinMode(ws2812pin, OUTPUT);
  digitalWrite(ws2812pin, LOW);
  clearStrip();
  delay(1);
}


// Taken from neopixel strandtest:
void loop() {
  colorWipe(Color(255, 0, 0), 10); // Red
  chase(Color(255, 0, 255), 10); // Green
  colorWipe(Color(0, 255, 0), 10); // Red
  chase(Color(255, 255, 0), 10); // Green
  colorWipe(Color(0, 0, 255), 10); // Blue
  chase(Color(0, 255, 255), 10); // Green
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint8_t wait) {
  for(uint16_t i=0; i<nPIXELS; i++) {
      setPixelColor(i, color);
      showStrip();
      delay(wait);
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
      NRF_GPIO->OUTSET = (1UL << ws2812pin);
      if(ledBar[wsOut] & (0x01 << x)) {
        __ASM ( \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              " NOP\n\t" \
              );
        NRF_GPIO->OUTCLR = (1UL << ws2812pin);
      
      }else{
        NRF_GPIO->OUTCLR = (1UL << ws2812pin);
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
