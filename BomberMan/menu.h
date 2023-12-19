#include<EEPROM.h>
#include "LiquidCrystal.h"
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int LCDBrightness = 6;

int currentRoom = 0;

//MAX7219 connection
const byte dinPin = 12; // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11; // pin 11 is connected to the CLK pin 13
const byte loadPin = 10; // pin 10 is connected to LOAD pin 12
const byte matrixSize = 8; // 1 as we are only using 1 MAX7219
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

// start and reset button
const int startButton = 3;

bool startButtonPressed = false;
bool gameState = false; // true - game start, false - game reset 
bool pauseGame = false;

int startValue;
int resetValue;

const int buttonDebounce = 400;
long long lastPress = 0;


// LED pins
const int blueLedPin = A2;
const int greenLedPin = A3;
const int redLedPin = A4;

int blueLedValue;
int greenLedValue;
int redLedValue;

// buzzer pin
const int buzzPin = 13;

//auxialiars for joystick movement
//last position for player
int lastX;
int lastY;
//read values from joystick
int xValue;
int yValue;

// game state
bool playerState = true; // it will change if the bomb will kill the player , if it's false => game over
bool openGame = true; // show first animation (smiley face)
bool startGame = false; // first 3 seconds show arrow animation, after start the game
bool endGame = false; // set end game when player will won/lose
bool winner = false;// set when the game end
                    // no walls on the matrix and playerState is true => player won
                    // playerState false => player lost the game
//bomb coordonates
int bombX = -1;
int bombY = -1;

long long endGameTimer = 0;

const int setBombTimer[4] = {5,4,3,2}; // depends in which room are you in
int bombTimer = 5; 
const int second = 1000;
long long bombSet = 0;

long long startTimer = 0;
int timer = 0;

const int bombBlink = 100;
long long lastBombBlink = 0; 
bool bombLed = true;

// player coordonates
int playerX = 0;
int playerY = 0;

const int playerBlink = 100;
long long lastPlayerBlink = 0; 
bool playerLed = true;

// Wall points
const int numWallPoints = 4;
const int totalWalls = 16;
int wallsRemain = 16;
int wallsDestroyed = 0;

// matrix 
const int numCols = 8;
const int numRows = 8;
const int numRooms = 4;
const int firstRoom = 0;
const int lastRoom = 3;

// joystick pins
const int joystickX = A0;
const int joystickY = A1;
const int joystickSW = 2;

const int minThreshold = 300;
const int maxThreshold = 700;
const int joystickDebounce = 500; 
long long lastMove = 0;
bool joyMoved = false;

long long lastJoystickPressed = 0;


//sign for scrolling trough menu
const byte scroll[] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};

//left parenthesis
const byte leftParenthesis[] = {
  B00001,
  B00010,
  B00100,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000
};

//right parenthesis
const byte rightParenthesis[] = {
  B01000,
  B00100,
  B00010,
  B00001,
  B00010,
  B00100,
  B01000,
  B00000
};

//hourglass for time
const byte time[] = {
  B11111,
  B11111,
  B01110,
  B00100,
  B00100,
  B01110,
  B11111,
  B11111
};

//walls 
const byte walls[] = {
  B00000,
  B00000,
  B00000,
  B10101,
  B11111,
  B11111,
  B00000,
  B00000
};

//bright icon
const byte bright[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

int firstChoice = -1;
int secondChoice = -1;

const String principalMenuItems[7] = {
  "Start Game",
  "Your Name",
  "Settings",
  "About",
  "How to play",
  "Highscore",
  "Reset highscore"
};

const int startIndex = 0;
const int nameIndex = 1;
const int settingsIndex = 2;
const int aboutIndex = 3;
const int howToPlayIndex = 4;
const int highscoreIndex = 5; 
const int resetIndex = 6;

long long lastText = 0;

const byte principalMenuIcon[3][8] = {
  {
    B10000,
    B11000,
    B11100,
    B11110,
    B11110,
    B11100,
    B11000,
    B10000
  },
  {
    B10001,
    B01110,
    B10001,
    B01110,
    B01010,
    B01110,
    B10001,
    B01110
  },
  {
    B01110,
    B10001,
    B10101,
    B10101,
    B10001,
    B10101,
    B10001,
    B01110
  }
};

const String settings[] = {
  "LCD bright",
  "Matrix bright",
  "Sound"
};
int brightMatrix;
int brightLCD;

int brightness[3] = {
  brightLCD,brightMatrix, 0
};

byte soundState;

int currentItem = 0;

int currentSetting = 0;

const String aboutText = "Game name: BomberMan | Made by Antonia Oancea | GitHub : github.com/AntoniaOancea";
const String howToPlay = "To win you have to destroy the walls placing bombs on the map pressing the joystick and get out of its way. You can pause the game pressing the black button!";

int currentPlayer = 0;
const int nrOfPLayers = 3; 


int score[3] = {EEPROM.read(4), EEPROM.read(5), EEPROM.read(6)};
char name[3][3] = {EEPROM.read(7), EEPROM.read(8), EEPROM.read(9)};
//int score[3] = {255, 255, 255};
//char name[5][3] = {"NaN", "NaN", "NaN"}; 
bool update = false; 

int currentLetter = 0;
char nameCurrent[3] = "AAA";

const int timeBlink = 50;
long long lastBlink;
int stateBlink = true;


void moveMenu()
{
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value

  lastX = playerX;
  lastY = playerY;

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentItem = (currentItem + 1) % 7;
    lcd.clear();
  }
  else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentItem = currentItem ? (currentItem - 1) : 6;
    lcd.clear();
  }

  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}

void moveSetting()
{
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value

  lastX = playerX;
  lastY = playerY;

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentSetting = (currentSetting + 1) % 3;
    lcd.clear();
  }
  else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentSetting = currentSetting ? (currentSetting - 1) : 2;
    lcd.clear();
  }

  if (yValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    if(currentSetting == 2) {
      soundState = !soundState;
      EEPROM.update(currentSetting, soundState);
    }
    else{
      brightness[currentSetting] = brightness[currentSetting] < 2 ? (brightness[currentSetting] + 1) :  2;
      //update brightness with new values and save it in EEPROM
      EEPROM.update(currentSetting, brightness[currentSetting]);
      lc.shutdown(0, false); // turn off power saving, enables display
      lc.setIntensity(0, (brightness[1]+1)*5); // sets brightness (0~15 possible values)
      lc.clearDisplay(0);// clear screen
      analogWrite(LCDBrightness, (brightness[0]+1)*50); // sets brightness (0~15 possible values)
    }
  }
  else if (yValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    if(currentSetting == 2) {
      soundState = !soundState;
      EEPROM.update(currentSetting, soundState);
    }
    else{
      brightness[currentSetting] = brightness[currentSetting] ? (brightness[currentSetting] - 1) : brightness[currentSetting];
      //update brightness with new values and save it in EEPROM
      EEPROM.update(currentSetting, brightness[currentSetting]);
      lc.shutdown(0, false); // turn off power saving, enables display
      lc.setIntensity(0, (brightness[1]+1)*5); // sets brightness (0~15 possible values)
      lc.clearDisplay(0);// clear screen
      analogWrite(LCDBrightness, (brightness[0]+1)*50); // sets brightness (0~15 possible values)
    }
  }

  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}

void selectItem(){
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  
  if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){
      if(currentItem == startIndex) { // the game starts
        startButtonPressed = true;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Room ");
        lcd.print(currentRoom + 1);
        //Serial.println("Start game");
        firstChoice = startIndex;
      }
      else{
        firstChoice = currentItem; // enter submenu
      }
      lastJoystickPressed = millis();
      lcd.clear();
  }
}

void settingsMenu(){ // move trough settings submenu and update values on joystick move
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){
    firstChoice = -1;
    lastJoystickPressed = millis();
    lcd.clear();
    return;
  }
  if(currentSetting == 2){
    lcd.setCursor(2,0);
    lcd.write(1);
    lcd.print("SOUND");
    lcd.setCursor(7, 1);
    if(soundState)
      lcd.print("< ON  >");
    else
      lcd.print("< OFF >");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.write(1);
    lcd.print(settings[currentSetting]);
    lcd.setCursor(5,1);
    lcd.print("-");
    for (int i = 0; i < 3; i++)
    {
      if(brightness[currentSetting] >= i)
        lcd.write(1);
      else
        lcd.print(" ");
    }
    lcd.print("+");
  }

  moveSetting();
}

void changeCurrentPlayer()
{
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  
  if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){
    firstChoice = -1;
    lastJoystickPressed = millis();
    lcd.clear();
    return;
  }

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentPlayer = (currentPlayer + 1) % nrOfPLayers;
    lcd.clear();
  }
  else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentPlayer = currentPlayer ? (currentPlayer - 1) : 2;
    lcd.clear();
  }

  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}

void showHighscore(){
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("<HIGHSCORE>");
  lcd.setCursor(0,1);
  lcd.print(currentPlayer + 1);
  //Serial.println(name[currentPlayer]);
  if(score[currentPlayer] == 255){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("<HIGHSCORE>");
    lcd.setCursor(0,1);
    lcd.print(currentPlayer + 1);
    lcd.setCursor(2,1);
    lcd.print("None");
  }
  else{
    lcd.setCursor(2,1);
    lcd.print(name[currentPlayer][0]);
    lcd.print(name[currentPlayer][1]);
    lcd.print(name[currentPlayer][2]);
    lcd.setCursor(6, 1);
    lcd.print("---- ");
    lcd.print(score[currentPlayer]);
  }
  
  changeCurrentPlayer();
}

void changeLetter(){
  //Serial.println(currentLetter);
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  
  if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){
    firstChoice = -1;
    lastJoystickPressed = millis();
    lcd.clear();
    return;
  }

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    if(nameCurrent[currentLetter] == 'A')
      nameCurrent[currentLetter] = 'Z';
    else
      nameCurrent[currentLetter] = nameCurrent[currentLetter] - 1;
  }
  else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    if(nameCurrent[currentLetter] == 'Z')
      nameCurrent[currentLetter] = 'A';
    else
      nameCurrent[currentLetter] = nameCurrent[currentLetter] + 1;
  }

  if (yValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentLetter = currentLetter - 1;
    if(currentLetter == -1)
      currentLetter = 0;
  }
  else if (yValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentLetter = (currentLetter + 1) % 3;
  }

  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}

void blinkLetter(){
  if(millis() - lastBlink > timeBlink || !lastBlink){
    lcd.setCursor(currentLetter + 2, 1);
    if(stateBlink){
      lcd.print(" ");
    }
    else{
      lcd.print(nameCurrent[currentLetter]);
    }
    lastBlink = millis();
    stateBlink = !stateBlink;
  }
}
void setName(){
  lcd.setCursor(2,0);
  lcd.print("YOUR NAME:");

  for(int i = 0; i < 3; i++){
    if(i != currentLetter){
      lcd.setCursor(i+2,1);
      lcd.print(nameCurrent[i]);
    }
    else
      blinkLetter();
  }

  changeLetter();
}

void resetEEPROM(){
  for (int i = 4; i < 7; i++)
  {
    EEPROM.update(i, -1);
  }
  for(int i = 7; i < 10; i++)
  {
    EEPROM.update(i,255);
  }
  
}

void welcome()
{
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");

  
  lcd.setCursor(0, 1);
  lcd.print("BomberMan!");
}

//create custom character 
void createCustomCharacter(){
  for(int i = 0; i < 3; i++)
    lcd.createChar(i, principalMenuIcon[i]);
  lcd.createChar(5, scroll);
  lcd.createChar(6, time);
  lcd.createChar(7, walls);
}

void secondaryMenu(){ 
  if(firstChoice == aboutIndex){
    for (int i = 0; i < aboutText.length(); i++) {
      int joystickPressed = digitalRead(joystickSW); // if is pressed => 0 => go back to principal menu
      if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){//scrolling the text
        firstChoice = -1;
        lastJoystickPressed = millis();
        lcd.clear();
        return;
      }

      if(millis() - lastText > second/2 && lastText){ 
        lastText = millis();
        lcd.clear();
        continue;
      }

      lcd.setCursor(0,0);
      lcd.print("<ABOUT>");
      lcd.setCursor(0, 1);
      lcd.print(aboutText.substring(i, i + 16));
      
      if(!lastText) 
        lastText = millis();

      i--;
    }
  }
  else
    if(firstChoice == howToPlayIndex){
      for (int i = 0; i < howToPlay.length(); i++) {
        int joystickPressed = digitalRead(joystickSW); // if is pressed => 0 => go back to principal menu
        if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){//scrolling the text
          firstChoice = -1;
          lastJoystickPressed = millis();
          lcd.clear();
          return;
        }

        if(millis() - lastText > second/2 && lastText){ 
          lastText = millis();
          lcd.clear();
          continue;
        }

        lcd.setCursor(0,0);
        lcd.print("<HOW TO PLAY>");
        lcd.setCursor(0, 1);
        lcd.print(howToPlay.substring(i, i + 16));
        
        if(!lastText) 
          lastText = millis();

        i--;
      }
    }
  else //go to secondary menu 
    if(firstChoice == settingsIndex){
      settingsMenu();
    }
  else
    if(firstChoice == highscoreIndex){
      showHighscore();
    }
  else
    if(firstChoice == nameIndex)
    {
      setName();
    }
  else
    if(firstChoice == resetIndex)
    {
      resetEEPROM();
      firstChoice = -1;
      lcd.clear();
    }
}

void principalMenu()
{
  //Serial.println(firstChoice);
  if(firstChoice != -1){ // pass principal menu if you are in a submenu
    secondaryMenu();
    return;
  }

  brightness[0] = brightLCD;
  brightness[1]=brightMatrix;

  lcd.setCursor(0,0);
  lcd.print("<MENU>");
  lcd.setCursor(15,0);
  lcd.write(5);

  //show current item on menu
  lcd.setCursor(0,1);
  lcd.write(currentItem);
  lcd.print(principalMenuItems[currentItem]);

  if(!startButtonPressed)
    moveMenu();
  
  selectItem();
}



