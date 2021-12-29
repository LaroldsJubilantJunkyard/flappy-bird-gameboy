#include <gb/gb.h>
#include "Common.h"


void GameplayEndSetup(){

    // Play a noise
    NR10_REG=0x7C;
    NR11_REG=0x41;
    NR12_REG=0x73;
    NR13_REG=0x73;
    NR14_REG=0x86;
}

UINT8 GameplayEndUpdate(){


    // While the player's y isn't really large
    // We're going to make the player fall off screen
    // Eventually the player will physically reach below (greater than) 176 and the loop will stop
    if(birdY<176){
        

        birdY+=birdVelocityY/5;

        // Reposition the bird's sprites
        move_sprite(0,birdX+8,birdY+16);
        move_sprite(1,birdX+8+8,birdY+16);

    // If the bird is far off screen, let's scroll in use sprites until none are on screen.
    }else{
        
        UINT8 numberOfInUseSprites=ScrollSpritesForPipes(4);

        if(numberOfInUseSprites==2){
            return GAMEPLAYSTART;
        }

    }

    return GAMEPLAYEND;
}
