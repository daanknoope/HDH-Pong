// Pong HDH Script
// Marjolein & Daan
// de Volksbank, 2019

// Libs
#include "LedControl.h"

// Init
int player_x = 4;
int player_y = 4;
int dx = 1;
int dy = 0;
int snake_len = 3;
int trail_x[32];
int trail_y[32];
int pellet_x;
int pellet_y;
int max_snake_len = 32;

LedControl lc=LedControl(12,11,10,1);
unsigned long delaytime=1000;

// object states

int button_1 = HIGH;
int button_2 = HIGH;

int button_1_pin = 5;
int button_2_pin = 4;



void setup() {
  Serial.begin(9600);  
  // put your setup code here, to run once:
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  
  int frame[8][8] = {1};
  displayFrame(frame);

  for (int i=0; i< max_snake_len; i++) {
    trail_x[i] = player_x;
    trail_y[i] = player_y;
  }
}

void loop() {
    place_pellet();
    
    while(true){
        int button_1_now = digitalRead(button_1_pin);
        int button_2_now = digitalRead(button_2_pin);
    
        if(button_1_now != button_1){
          button_1 = button_1_now; 
          Serial.println("button 1 was pressed");
          if(button_1 == LOW){
            //then go up
            turn_right();
          }
        }
        else if(button_2_now != button_2){
          //update game state
          button_2 = button_2_now;
          Serial.println("button 2 was pressed");
          if(button_2 == LOW){
            turn_left();
          }
        }

        move_player();
        check_pellet();
        update_trail();
    
        int frame[8][8] = {0};
        renderFrame(frame);
        displayFrame(frame);
        delay(100);
    }
}

void turn_left() {
  if (dx == 1) {
    dx = 0; dy = 1;
    return;
  }
  if (dx == -1) {
    dx = 0; dy = -1;
    return;
  }
  if (dy == 1) {
    dx = -1; dy = 0;
    return;
  }
  if (dy == -1) {
    dx = 1; dy = 0;
    return;
  }
}

void turn_right() {
  if (dx == 1) {
    dx = 0; dy = -1;
    return;
  }
  if (dx == -1) {
    dx = 0; dy = 1;
    return;
  }
  if (dy == 1) {
    dx = 1; dy = 0;
    return;
  }
  if (dy == -1) {
    dx = -1; dy = 0;
    return;
  }
}

void move_player() {
  player_x += dx;
  player_y += dy;
  if (player_x > 7) player_x = 0;
  if (player_x < 0) player_x = 7;
  if (player_y > 7) player_y = 0;
  if (player_y < 0) player_y = 7;
}

void check_pellet() {
  if (player_x == pellet_x and player_y == pellet_y) {
    snake_len++;
    if (snake_len == max_snake_len) {
      snake_len = 1;
    }
    place_pellet();
  }
}

void place_pellet() {
  pellet_x = random(0, 7);
  pellet_y = random(0, 7);
}

void update_trail() {
  int next_x = player_x;
  int next_y = player_y;
  int tmp_x;
  int tmp_y;
  
  for (int i = 0; i < snake_len; i++) {
    tmp_x = trail_x[i];
    tmp_y = trail_y[i];

    trail_x[i] = next_x;
    trail_y[i] = next_y;

    next_x = tmp_x;
    next_y = tmp_y;
  }
}

// assume display is empty
void displayFrame(int frame[8][8]){
    lc.clearDisplay(0);
    for (int i = 0; i < 8; i++){
      for (int j = 0; j < 8; j++){
        if(frame[i][j] > 0){
          lc.setLed(0, i, j, true);
        }
       }
      }
    }

void renderFrame(int frame[8][8]){
  // Add pellet
  frame[pellet_y][pellet_x] = 1;
  
  // Add head 
  frame[player_y][player_x] = 1;
  
  // Add tail
  for (int i = 0; i < snake_len - 1; i++) {
    frame[trail_y[i]][trail_x[i]] = 1;
  }
}
