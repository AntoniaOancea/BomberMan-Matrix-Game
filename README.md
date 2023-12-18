# BomberMan Matrix Game

## Backstory
  ![](https://github.com/AntoniaOancea/BomberMan-Matrix-Game/blob/main/BomberMan/originalgame.png)
  
  I chose this game because I used to play the original game (Bomberman)  quite often when I was little.
  When I played it, I had the impression that it was a simple game , a man who walks between some walls and places bombs.
  In the end it was not that easy, but I liked working on it.

## Components
  * LCD
  * Joystick
  * Buzzer
  * Led Matrix
  * Matrix Driver
  * Button
  * Potentiometer
  * Wires, Resistors, Capacitors as needed
  * Arduino Uno


## Menu 
  Use the joystick to walk through the menu!
  * Start Game
  * Your name
    - AAA (change it using the joystick)
  * Settings (set intensity)
    - LCD Bright
    - Matrix Bright
  * About
  * How To Play
  * Highscore
     - 1 name score
     - 2 name score
     - 3 name score
  * Reset Highscore

## Game Scope
  The player should destroy all the walls on the map by placing bombs in less than 240 seconds and without being hit by a bomb.

## How to play
  The player can start the game in 2 ways: pressing start game from the menu or pressing the button next to the buzzer.
  
  Once the game starts, the player can place bombs by pressing the joystick and move around the map according to the movement of the joystick.
  
  I chose to create the map so that you can walk between 4 rooms placed vertically.
  
  The bomb will explode depending on the room you are in, for exemple if you are in room 1 it will explode in 5 seconds, in room 2 will explode in 4 seconds and so on.
  
  You cannot change the room while a bomb is active.
  
  If during the game you want to drink water or think about a strategy for the game, you can press the button next to the buzzer and will pause the game.

                                                          +---------+
                                                          
                                                          | Room 1  |
                                                          
                                                          +---------+
                                                          
                                                          | Room 2  |
                                                          
                                                          +---------+
                                                          
                                                          | Room 3  |
                                                          
                                                          +---------+
                                                          
                                                          | Room 4  |
                                                          
                                                          +---------+

            
## [Demo](https://youtu.be/WYBJjbET9w8)

## [Code](https://github.com/AntoniaOancea/BomberMan-Matrix-Game/blob/main/BomberMan/main.ino)

## Setup
![Setup](https://github.com/AntoniaOancea/BomberMan-Matrix-Game/blob/main/BomberMan/project-setup.jpg)


  
  
  

    
