// Pong HDH Script
// Marjolein & Daan
// de Volksbank, 2019

// Libs
#include "LedControl.h"

// Init
int bat_player;
int bat_enemy;
int ball_i;
int ball_j;
int horizontal_direction;
int vertical_direction;

bool game_ended = false;
bool we_won;


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

  setup_game_state();

  int frame[8][8] = {1};
  displayFrame(frame);

}

void setup_game_state(){
  bat_player = 3;
  bat_enemy = 5;

  ball_i = 3;
  ball_j = 5;

  horizontal_direction = -1;
  vertical_direction = -1;
}

void loop() {
    while(!game_ended){
        int button_1_now = digitalRead(button_1_pin);
        int button_2_now = digitalRead(button_2_pin);
    
        if(button_1_now != button_1){
          button_1 = button_1_now; 
          Serial.println("button 1 was pressed");
          if(button_1 == LOW){
            //then go up
            move_player_up();
          }
        }
        else if(button_2_now != button_2){
          //update game state
          button_2 = button_2_now;
          Serial.println("button 2 was pressed");
          if(button_2 == LOW){
            move_player_down();
          }
        }
    
        move_random_bot();
        move_ball();
    
        int frame[8][8] = {0};
        renderFrame(frame);
        displayFrame(frame);
        delay(100);
    }


    //game has ended

    //display fun graphics
    if(we_won){
      lc.setRow(0,7,B00100100);
      lc.setRow(0,6,B00100100);
      lc.setRow(0,5,B00100100);
      lc.setRow(0,4,B10000001);
      lc.setRow(0,3,B01000010);
      lc.setRow(0,2,B00100100);
      lc.setRow(0,1,B00011000);
      lc.setRow(0,0,B00000000);
    } else{
      lc.setRow(0,7,B00100100);
      lc.setRow(0,6,B00100100);
      lc.setRow(0,5,B00100100);
      lc.setRow(0,4,B00000000);
      lc.setRow(0,3,B00011000);
      lc.setRow(0,2,B00100100);
      lc.setRow(0,1,B01000010);
      lc.setRow(0,0,B10000001);
    }

    delay(1000);
    //reset gamestate
    setup_game_state();
    game_ended = false;
  
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

void move_random_bot(){
  int r = random(0,7);
  if(r == 0){
    move_enemy_up();
  } else if (r == 1){
    move_enemy_down();
  }
}

void move_player_up(){
  if(bat_player > 0){
    bat_player--;
  }
}

void move_player_down(){
  if(bat_player < 5){
    bat_player++;
  }
}
void move_enemy_up(){
  if(bat_enemy > 0){
    bat_enemy--;
  }
}

void move_enemy_down(){
  if(bat_enemy < 5){
    bat_enemy++;
  }
}
// assumed: coordinates are valid (ie within range)
void renderFrame(int frame[8][8]){
  // add ball 
  frame[ball_i][ball_j] = 1;

  // add bat_player
  frame[bat_player][0] = 1;
  frame[bat_player+1][0] = 1;
  frame[bat_player+2][0] = 1;
  // add bat_enemy
  frame[bat_enemy][7] = 1;
  frame[bat_enemy+1][7] = 1;
  frame[bat_enemy+2][7] = 1;
}

void move_ball(){
  move_ball_vertically();
  move_ball_horizontally();
}


void move_ball_vertically(){
  //move
  ball_i += vertical_direction;
  if(ball_i == 0  || ball_i==7){
    vertical_direction *= -1;
  }  
}

void move_ball_horizontally(){
  ball_j += horizontal_direction;
  if(ball_j == 0 || ball_j == 7){
    if(bounce_on_bat()){
      horizontal_direction *= -1;
      if(ball_i > 1 && ball_i < 6){
        ball_i += horizontal_direction;
      }
    } else{
      game_ended = true;
      if(ball_j == 7){
        we_won = true;
      } else if(ball_j == 0){
        we_won = false;
      }
    }
  }
}

bool bounce_on_bat(){
  if(ball_j == 0){ //left bat
    if(ball_i >= bat_player && ball_i < bat_player+3){
      return(true);
    }
  } else if (ball_j == 7) { //right bat
    if(ball_i >= bat_enemy && ball_i < bat_enemy+3){
      return(true);
    }
  }
  return(false);
}
