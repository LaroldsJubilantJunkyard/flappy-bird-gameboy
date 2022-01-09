#include <gb/gb.h>
#include <gb/hardware.h>
#include "Common.h"
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/FlappyBirdTitle.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/ScoreTiles.h"
#include "Graphics/MenuSprites.h"

void ClearTopSky(){
    VBK_REG = 1;    
    set_bkg_tiles(0,0,20,8,FlappyBirdBackground_map_attributes);  
    VBK_REG = 0;     
    set_bkg_tiles(0,0,20,8,FlappyBirdBackground_map); 
}

void ShowFlappyBirdLogo(){

    UINT8 i;

    unsigned char FlappyBirdTitle_map_offset2[48];
    unsigned char FlappyBirdTitle_map_offset[48];

    for(i=0;i<48;i++){
        FlappyBirdTitle_map_offset2[i]=FlappyBirdTitle_map_attributes[i]+5;
    }

    VBK_REG = 1;    
    set_bkg_tiles(4,1,12,4,FlappyBirdTitle_map_offset2);  

    for(i=0;i<48;i++){
        FlappyBirdTitle_map_offset[i]=FlappyBirdTitle_map[i]+FlappyBirdBackground_TILE_COUNT+ScoreTilesLen;
    }

    VBK_REG = 0;     
    set_bkg_tiles(4,1,12,4,FlappyBirdTitle_map_offset); 
}

void GameplayStartSetup(){

    UINT8 i;

    // Clear up sprites for usage
    // Move all non-player sprites to large number
    // So our logic later tracks them as 'unused' (x > 176)
    // Also set their palette
    for(i=2;i<40;i++){
        move_sprite(i,250,0);
        set_sprite_prop(i,1);
    }

    const UWORD TapToStartPalette[] = {MenuSpritesCGBPal0c0,MenuSpritesCGBPal0c1,MenuSpritesCGBPal0c2,MenuSpritesCGBPal0c3};
    const UWORD TapSpritePalette[] = {MenuSpritesCGBPal2c0,MenuSpritesCGBPal2c1,MenuSpritesCGBPal2c2,MenuSpritesCGBPal2c3};
    const UWORD BirdPalette[] = {BirdAndPipesCGBPal0c0,BirdAndPipesCGBPal0c1,BirdAndPipesCGBPal0c2,BirdAndPipesCGBPal0c3};
    const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};
    const UWORD PipesPalette[] = {BirdAndPipesCGBPal1c0,BirdAndPipesCGBPal1c1,BirdAndPipesCGBPal1c2,BirdAndPipesCGBPal1c3};
    

    set_sprite_data(0,BirdAndPipesLen,BirdAndPipes);
    set_sprite_data(BirdAndPipesLen,MenuSpritesLen,MenuSprites);

    set_sprite_palette(0,2,BirdPalette);
    set_sprite_palette(1,1,PipesPalette);
    set_sprite_palette(2,1,BlackAndWhitePalette);
    set_sprite_palette(3,1,TapToStartPalette);
    set_sprite_palette(4,1,TapSpritePalette);

    // Set our player's tiles
    set_sprite_tile(0,2);
    set_sprite_tile(1,4);
    
    set_bkg_data(FlappyBirdBackground_TILE_COUNT+ScoreTilesLen,FlappyBirdTitle_TILE_COUNT,FlappyBirdTitle_tiles);
    set_bkg_palette(5,3,FlappyBirdTitle_palettes);

    // Clear the top sky so when we come from GameplayEnd the Board is painted over.
    ClearTopSky();

    // Show logo
    ShowFlappyBirdLogo();

    // put the bird off screen
    birdX=240;
    birdY=80;

    // Set our player's tile palette to be the black and white palette
    set_sprite_prop(0,2);
    set_sprite_prop(1,2);

    // Set our "tap hand" sprites tiles
    set_sprite_tile(2,BirdAndPipesLen);
    set_sprite_tile(3,BirdAndPipesLen+2);
    set_sprite_prop(2,3);
    set_sprite_prop(3,3);
    
    // Set our "tap" text sprites tiles
    set_sprite_tile(4,BirdAndPipesLen+4);
    set_sprite_tile(5,BirdAndPipesLen+6);
    set_sprite_prop(4,4);
    set_sprite_prop(5,4);

}

UINT8 GameplayStartUpdate(){  

    // Move tothe right, while our birdX is less than 24
    // Our bird starts at 240 (off screen tothe right)
    // We also check to make sure it's larger than 160 (the width of the screen in pixels)
    if(birdX<24||birdX>160)birdX++;

    move_sprite(0,birdX+8,birdY+16);
    move_sprite(1,birdX+16,birdY+16);

    if(birdX>=24&&birdX<160){


        // Drawing the Tap hand sprite

        move_sprite(2,birdX+8,birdY+32);
        move_sprite(3,birdX+16,birdY+32);
    }else{

        // Drawing the Tap hand sprite

        move_sprite(2,0,0);
        move_sprite(3,0,0);

    }

    if((SCX_REG/3)%2==0&&birdX>=24&&birdX<160){

        //Drawing the tap text sprite

        move_sprite(4,birdX+32,birdY+24);
        move_sprite(5,birdX+40,birdY+24);

    }else {

        //Drawing the tap text sprite

        move_sprite(4,0,0);
        move_sprite(5,0,0);

    }

    // If the player presses A and the bird is larger than or equal to 24
    // The bird starts off screen to the right, we make sureit's X is less than 160. So were sure it's on screen
    if(birdX>=24&&birdX<160&&(joypad() & J_A)){

        // Clear up sprites for usage
        // Move all non-player sprites to large number
        // So our logic later tracks them as 'unused' (x > 176)
        // Also set their palette
        for(UINT8 i=2;i<40;i++){
            move_sprite(i,250,0);
            set_sprite_prop(i,1);
        }

        ClearTopSky();

        return COREGAMELOOP;
    }

    return GAMEPLAYSTART;
}

