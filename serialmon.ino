#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

static constexpr auto dc_pin = 9;
static constexpr auto cs_pin = 10;
static constexpr auto baudrate = 115200;

Adafruit_ILI9341 tft = Adafruit_ILI9341(cs_pin, dc_pin);

void setup() {
  Serial.begin(baudrate);
 
  tft.begin();
  delay(10);
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println(" -- char -- | ---- hex ---->");
}

static constexpr size_t char_area = 12;
static constexpr size_t hex_area = char_area * 3;
char line_buf[char_area + hex_area + 1];
size_t current_length = 0;
size_t current_line   = 0;

void loop(void) {
  if (Serial.available()) {
    const auto c = Serial.read();
    sprintf(line_buf + current_length, "%c", isprint(c) ? c : '.');
    
    const auto next_pos = char_area + 1 + current_length * 3;
    for (unsigned i = current_length + 1; i < next_pos; i++) {
      line_buf[i] = ' ';
    }
    
    sprintf(line_buf + next_pos, "%02x", c);
    
    tft.setCursor(0, 0);
    for (unsigned i = 0; i < current_line + 1; i++) {
      tft.println();
    }
    tft.print(line_buf);
    
    current_length++;

    if (current_length >= char_area) {
      current_length = 0;
      current_line++;
    }
  }
}
