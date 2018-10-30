/**************************************
 *      AUTORS                        *
 *                                    *
 *    Casini Lorenzo   0000800942     *
 *                                    *
 *    Fantoni Sophia   0000789462     *
 *                                    *
 *    Del Gatto Simone 0000794362     * 
 **************************************/


/*
 *  This part of software have to control the game loop.
 */

//possible game status
#define WAITING_PLAYER1 1               //game waiting that player 1 push the button
#define WAITING_PLAYER2 2               //game waiting that player 2 push the button
#define WAIT_START_GAME 0               //game waiting that someone push the start game button
#define OVER 3                          //game over (when someone one player press button when it isn't his turn or when his turn is too long)
#define MIDDLE 4                        //the ball is between the player
#define START_BUTTON 7

volatile int state = WAIT_START_GAME;
volatile int P1pressButton = 0;         //set 1 if the player 1 push the button
volatile int P2pressButton = 0;         //set 1 if the player 2 push the button
int RT = 1000;                          //Reactive Time
long t = 0;                             //time when start one of two players turn
int numShots = 0;                       //number of shoots
volatile int winner;                    //who is the winner

void setup() {
  Serial.begin(9600);
  pinMode(START_BUTTON, INPUT);
  attachInterrupt(1, P1Press, FALLING);
  attachInterrupt(0, P2Press, FALLING);
  startVisualization();                       //begin the game view element
}

/********interrupt actions********/

void P1Press(){                       
  if(state != WAIT_START_GAME && state != OVER){                        //before someone press the starting button and when the game state is over the game don't handle player button interrupt
    if(state != WAITING_PLAYER1){                                       //if it is the other player turn the game over and the winner is palyer 2
      state = OVER;
      winner=2;
      Serial.println("[P1 preme il bottone al momento sbagliato]");
    }else{
      P1pressButton = 1;                                                //everything is correct
    }
  }
}

void P2Press(){
  if(state != WAIT_START_GAME && state != OVER){                        //before someone press the starting button and when the game state is over the game don't handle player button interrupt
    if(state != WAITING_PLAYER2) {                                      //if it is the other player turn the game over and the winner is palyer 1
      state = OVER;
      winner = 1;
      Serial.println("[P2 preme il bottone al momento sbagliato]");
    }else{
      P2pressButton = 1;                                                //everything is correct
    }
  }  
}

void updateRT(){
 RT = RT - (RT/8); 
}

/************Game Loop *************/

void loop() {
  noInterrupts();
  while(state == WAIT_START_GAME){                     //game waiting that someone press the start button
    interrupts();                                      //in the meanwhile run a view presentation
    runPresentation();
    if(digitalRead(START_BUTTON)){                     //if the buttun is pressed select in a random way  
      stopPresentation();                              //the first player and start the game
      randomSeed(millis());
      int r = random(0, 3201) % 2;
      noInterrupts();
      state =  r ? WAITING_PLAYER1 : WAITING_PLAYER2;
      interrupts();
      startMovement(r);
      t = millis();
      break;
    }
    delay(100);
  }
  interrupts();
  
  noInterrupts();
  if(state == WAITING_PLAYER1){                               //player 1 turn
    interrupts();
    long now = millis();
    noInterrupts();
    if(state != OVER){                                        //if state is over do nothing
      if(now - t <= RT){                                      //true when player is still in time to press the button
        if(P1pressButton) {                                   //true when player 1 push the button 
          P1pressButton = 0;                           
          state = MIDDLE;
          interrupts();
          fromP1toP2();                                       //show that the ball go from player 1 to player 2
          noInterrupts();
          state = state != OVER ? WAITING_PLAYER2 : OVER;     //if there no problem start the player 2 turn
          interrupts();
          t = millis();                                       //set player 2 turn starting time
          numShots++;                                         //update the statistic counters and RT 
          updateRT();
          Serial.println("P1 preme il bottone correttamente");
        }
      } else {
        noInterrupts();                                       
        state = OVER;                                         //no more time player 1 lose
        winner = 2;
        interrupts();
      }
    }
  }
  interrupts();
  
  noInterrupts();
  if(state == WAITING_PLAYER2){                                 //player 2 turn
    interrupts();                                         
    long now = millis();                                  
    noInterrupts();
    if(state != OVER){                                          //if state is over do nothing
      if(now - t <= RT){                                        //true when player is still in time to press the button
        if(P2pressButton){                                      //true when player 2 push the button 
          P2pressButton = 0;
          state = MIDDLE;
          interrupts();
          fromP2toP1();                                         //show that the ball go from player 2 to player 1
          noInterrupts();
          state = state != OVER ? WAITING_PLAYER1 : OVER;       //if there no problem start the player 1 turn
          interrupts();
          t = millis();                                         //set player 1 turn starting time
          numShots++;
          updateRT();                                           //update the statistic counters and RT 
          Serial.println("P2 preme il bottone correttamente");
        }
      } else {
        noInterrupts();
        state = OVER;
        winner = 1;
        interrupts();
      }  
    }
    
  }
  interrupts();
  noInterrupts();
  
  if(state == OVER){                                                                                                    //someone press uncorrectly the button or the turn time for do something is finished
    Serial.println("Game Over - The winner is Player " + String(winner) + ", after " + String(numShots) + " shots.");
    interrupts();
    endGame();                                                                                                          //end game visualization
    noInterrupts();
    state = WAIT_START_GAME;                                                                                            //restart game operations
    P1pressButton = 0;
    P2pressButton = 0;
    numShots = 0;
    winner = 0;
    RT = 1000;
    interrupts();
  }  
  interrupts();
  delay(10);
}
