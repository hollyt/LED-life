#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

static uint16_t
  board_bmp[] = {
    0b0000000000000000,
    0b0110000000000000,
    0b0100000000000000,
    0b0110000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000 },
  buffer_bmp[] = { 0,0,0,0,0,0,0,0,0,0 };
static uint8_t
  draw_bmp[] = { 0,0,0,0,0,0,0,0 };
    
void setup() {
  Serial.begin(9600);
  Serial.println("Game of Life");
  
  matrix.begin(0x70);  // pass in the address
}

void loop() {
  matrix.clear();
  matrix.drawBitmap(0, 0, draw_bmp, 8, 8, LED_GREEN);
  matrix.writeDisplay();
  update_buffer();
  merge_boards();
  make_8x8();
  delay(500);
}

void update_buffer() {
  for (uint8_t i = 1; i < 9; i++) {
    for (uint8_t j = 14; j >= 7; j--) {
      if (!(board_bmp[i] & (1<<j))) {
        uint8_t neighbour_count = 0;
        if (board_bmp[i-1] & (1<<j+1)) {
          neighbour_count++;
        }
        if (board_bmp[i-1] & (1<<j)) {
          neighbour_count++;
        }
        if (board_bmp[i-1] & (1<<j-1)) {
          neighbour_count++;
        }
        if (board_bmp[i+1] & (1<<j+1)) {
          neighbour_count++;
        }
        if (board_bmp[i+1] & (1<<j)) {
          neighbour_count++;
        }
        if (board_bmp[i+1] & (1<<j-1)) {
          neighbour_count++;
        }
        if (board_bmp[i] & (1<<(j+1))) {
          neighbour_count++;
        }
        if (board_bmp[i] & (1<<(j-1))) {
          neighbour_count++;
        }
        if (neighbour_count >= 3) {
          buffer_bmp[i] |= (1<<j);
        }
      }
    }
  }
}

void merge_boards() {
  for (uint8_t i = 1; i < 9; i++) {
    board_bmp[i] |= buffer_bmp[i];
    // Clear buffer
    buffer_bmp[i] &= 0;
  }
}

void make_8x8() {
  for (uint8_t i=0; i < 8; i++) {
    draw_bmp[i] = board_bmp[i+1]>>7;
  }
}
