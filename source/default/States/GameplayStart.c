#include <gb/gb.h>
#include "Common.h"



void GameplayStartSetup(){

    // Clear up sprites for usage
    // Move all non-player sprites to large number
    // So our logic later tracks them as 'unused' (x > 176)
    // Also set their palette
    for(UINT8 i=2;i<40;i++){
        move_sprite(i,250,0);
        set_sprite_prop(i,1);
    }

    // put the bird off screen
    birdX=240;
    birdY=80;

    // Set our player's tile palette to be the black and white palette
    set_sprite_prop(0,2);
    set_sprite_prop(1,2);


}

UINT8 GameplayStartUpdate(){  

    // Move tothe right, while our birdX is less than 24
    // Our bird starts at 240 (off screen tothe right)
    // We also check to make sure it's larger than 160 (the width of the screen in pixels)
    if(birdX<24||birdX>160)birdX++;

    move_sprite(0,birdX+8,birdY+16);
    move_sprite(1,birdX+8+8,birdY+16);

    // If the player presses A and the bird is larger than or equal to 24
    // The bird starts off screen to the right, we make sureit's X is less than 160. So were sure it's on screen
    if(birdX>=24&&birdX<160&&(joypad() & J_A)){
        return COREGAMELOOP;
    }

    return GAMEPLAYSTART;
}

