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
 * This part of software control the visualization part
 */
#define PRESENTATION_PIN 5               //pin for fading during game introduction
#define TRANS_PIN1 10                    //led that identify player 1
#define TRANS_PIN2 12                    //led used to simulate ball movement
#define TRANS_PIN3 11                    //led that identify palyer 2
#define POT_PIN A0                       //pin used for read potentiometer

int SPEED;                               //blinky middle led speed
int winnerLed = HIGH;                    //winner led status
int currentIntensity = 0;                //used for beginnig fading
int fadeAmount = 10;

void startVisualization(){
  pinMode(PRESENTATION_PIN, OUTPUT);
  pinMode(TRANS_PIN1, OUTPUT);
  pinMode(TRANS_PIN2, OUTPUT);
  pinMode(TRANS_PIN3, OUTPUT);
  pinMode(START_BUTTON, INPUT);
  Serial.println("Welcome to Led Pong. Press Key T3 to Start. ");
}

/*red led fade at the beginning*/
void runPresentation(){                         
  analogWrite(PRESENTATION_PIN, currentIntensity);
  currentIntensity = currentIntensity + fadeAmount;
  if(currentIntensity == 0 || currentIntensity == 255){
    fadeAmount = -fadeAmount;
  }
  delay(50);
}


/* 
 *Stop the initial presentation the game start and the SPEED is setted. 
 */
void stopPresentation(){
  digitalWrite(PRESENTATION_PIN, LOW);
  int value = analogRead(POT_PIN);
  SPEED = map(value, 0, 1023, 1, 5);
  Serial.println("SPEED: " + String(SPEED));
}

/*
 * Show the first ball movement one second in the middle and then 
 * decide  wich pin turn off using p that is the player that have the ball.
 */
void startMovement(int p){
  Serial.println("Go!");
  digitalWrite(TRANS_PIN2, HIGH);
  delay(1000);
  digitalWrite(TRANS_PIN2, LOW);
  if(p){
    digitalWrite(TRANS_PIN1, HIGH);
    Serial.println("[startMovement] INIZIA G1");
  } else {
    digitalWrite(TRANS_PIN3, HIGH);
    Serial.println("[startMovement] INIZIA G2");
  }
}

/*
 * Simulate ball movement from player 1 to player 2.
 */
void fromP1toP2(){
  digitalWrite(TRANS_PIN1, LOW);
  digitalWrite(TRANS_PIN2, HIGH);
  delay(1000/SPEED);
  digitalWrite(TRANS_PIN2, LOW);
  digitalWrite(TRANS_PIN3, HIGH);
}

/*
 * Simulate ball movement from player 2 to player 1.
 */
void fromP2toP1(){
  digitalWrite(TRANS_PIN3, LOW);
  digitalWrite(TRANS_PIN2, HIGH);
  delay(1000/SPEED);
  digitalWrite(TRANS_PIN2, LOW);
  digitalWrite(TRANS_PIN1, HIGH);
}

/*
 * Blinky the winner player led when the game is over.
 */
void endGame(){
  int pin = (winner == 1) ? TRANS_PIN1 : TRANS_PIN3;
  digitalWrite(TRANS_PIN1, LOW);
  digitalWrite(TRANS_PIN2, LOW);
  digitalWrite(TRANS_PIN3, LOW);
  long now = millis();
  while(millis() - now <= 2000){
    digitalWrite(pin, winnerLed);
    winnerLed = winnerLed ? LOW : HIGH;
    delay(50);
  }
  digitalWrite(pin, LOW);
}
