#include <gb/gb.h>
#include "Common.h"
#include "States/GameFirstLoad.h"
#include "States/GameplayStart.h"
#include "States/CoreGameLoop.h"
#include "States/GameplayEnd.h"

void ScrollTheBackgroundAndApplyGravity(){
    
    backgroundScrollCounter++;

    floorBackgroundScroll++;

    // We only scroll (by one) every 7 frames, so it scrolls slower than pipes move
    if(backgroundScrollCounter>=10){
        topBackgroundScroll++;

        backgroundScrollCounter=0;
    }
    if(backgroundScrollCounter%5==0){
        midBackgroundScroll++;
    }

    if(backgroundScrollCounter%2==0){
        lowBackgroundScroll++;
    }

    // Increase velocity, but limit to avoid high falling speeds
    if(birdVelocityY<21){
        birdVelocityY++;
        
    }
}

void main(void){

    GameFirstLoad();
    
    UINT8 currentGameState=GAMEFIRSTLOAD;
    UINT8 nextGameState=GAMEPLAYSTART;

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