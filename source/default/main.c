#include <gb/gb.h>
#include "Common.h"
#include "States/GameFirstLoad.h"
#include "States/GameplayStart.h"
#include "States/CoreGameLoop.h"
#include "States/GameplayEnd.h"

void ScrollTheBackgroundAndApplyGravity(){
    
    backgroundScrollCounter++;

    // We only scroll the background clouds (by one) every 10frames, so it scrolls slower than pipes move
    if(backgroundScrollCounter>=10){
        topBackgroundScroll++;

        backgroundScrollCounter=0;
    }

    // We only scroll the city buildings every 5 frames
    if(backgroundScrollCounter%5==0){
        midBackgroundScroll++;
    }

    // We only scroll the background bushes every 2 frames
    if(backgroundScrollCounter%2==0){
        lowBackgroundScroll++;
    }

    // The scroll of the ground floor
    // We scroll this every frame
    floorBackgroundScroll++;

    // Increase velocity, but limit to avoid high falling speeds
    if(birdVelocityY<21){
        birdVelocityY++;
        
    }
}

void main(void){

    GameFirstLoad();
    
    uint8_t currentGameState=GAMEFIRSTLOAD;
    uint8_t nextGameState=GAMEPLAYSTART;

    while(1){
        
        // Save the last joypad state
        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();

        ScrollTheBackgroundAndApplyGravity();

        // If we are changing game state
        if(nextGameState!=currentGameState){

            // Update our current game state
            currentGameState=nextGameState;

            // Call our game state's setup function
            if(currentGameState==GAMEPLAYSTART)GameplayStartSetup();
            else if(currentGameState==COREGAMELOOP)CoreGameLoopSetup();
            else if(currentGameState==GAMEPLAYEND)GameplayEndSetup();

        }

        // Call our game state's update function
        if(currentGameState==GAMEPLAYSTART)nextGameState=GameplayStartUpdate();
        else if(currentGameState==COREGAMELOOP)nextGameState=CoreGameLoopUpdate();
        else if(currentGameState==GAMEPLAYEND)nextGameState=GameplayEndUpdate();

        // Wait until the vertical blank phase has completed
        wait_vbl_done();
    }
    

}