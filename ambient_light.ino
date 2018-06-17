/*

AMBIENT LIGHT CONTROLLED WITH IR REMOTE
Uses WS2812 addressable RGB LEDS
and ADAFRUIT TRINKET PRO 5V

IR Remote button codes:
CH +    = 0xFFE21D
CH      = 0xFF629D
CH -    = 0xFFA25D
BACK    = 0xFF22DD
NEXT    = 0xFF02FD
PLAY    = 0xFFC23D
MINUS   = 0xFFE01F
PLUS    = 0xFFA857
EQ      = 0xFF906F
0       = 0xFF6897
100+    = 0xFF9867
200+    = 0xFFB04F
1       = 0xFF30CF
2       = 0xFF18E7
3       = 0xFF7A85
4       = 0xFF10EF
5       = 0xFF38C7
6       = 0xFF5AA5
7       = 0xFF42BD
8       = 0xFF4AB5
9       = 0xFF52AD
*/

#include <FastLED.h>
#include <IRremote.h> 

// LED SETTINGS
#define DATA_PIN    12
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];

// IR REMOTE SETTINGS
int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;

// LIGHT SETTINGS
int BRIGHTNESS = 255;
int SATURATION = 255;
int HUE = 0;
int COUNTER = 0;
int INCR = 32;

void setup() {
//  Serial.begin(9600);
  irrecv.enableIRIn();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CHSV(random(0,255), SATURATION, BRIGHTNESS));
  FastLED.show();
}

void loop() {
    if (BRIGHTNESS == 0) delay(1000);
 
    if (irrecv.decode(&results)){    // DECODE AND INTERPERET IR RECIEVER SIGNALS
        irrecv.resume();
//        delay(100);

        if (results.value == 0xFFA857){  // PLUS = BRIGHTNESS
            if ((BRIGHTNESS + INCR) < 255) BRIGHTNESS += INCR;
            else BRIGHTNESS = 255;
        }
        else if (results.value == 0xFFE01F){  // MINUS = BRIGHTNESS
            if ((BRIGHTNESS - INCR) > 0) BRIGHTNESS -= INCR;
            else BRIGHTNESS = 0;
        }
        else if (results.value == 0xFF906F){  // EQ -- toggle MAX / ZERO brightness
            if (BRIGHTNESS < 255) BRIGHTNESS = 255;
            else BRIGHTNESS = 0;
        }
        else if (results.value == 0xFFE21D){  // CHANNEL + = SATURATION UP
            if ((SATURATION + INCR) < 255) SATURATION += INCR;
            else SATURATION = 255;
        }
        else if (results.value == 0xFF629D){  // CHANNEL = SATURATION TOGGLE
            if (SATURATION < 255) SATURATION = 255;
            else SATURATION = 0;
        }
        else if (results.value == 0xFFA25D){  // CHANNEL DOWN = SATURATION DOWN
            if ((SATURATION - INCR) > 0) SATURATION -= INCR;
            else SATURATION = 0;
        }
        else if (results.value == 0xFF02FD) HUE = (HUE + INCR) % 255;  // NEXT UP = HUE
        else if (results.value == 0xFF22DD) HUE = (HUE - INCR) % 255;  // NEXT DOWN = HUE
        else if (results.value == 0xFFC23D) COUNTER = 1 - COUNTER; // PLAY BUTTON - TOGGLE COUNTER INCREMENT
    }

    HUE += COUNTER;
    fill_solid(leds, NUM_LEDS, CHSV(HUE, SATURATION,  BRIGHTNESS));
    FastLED.show();
    delay(1000 / 40);

}

