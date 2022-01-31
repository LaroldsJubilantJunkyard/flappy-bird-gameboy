#include <gb/gb.h>
#include <gb/hardware.h>
#include "Common.h"
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/FlappyBirdTitle.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/ScoreTiles.h"
#include "Graphics/TapToStartSprites.h"

void ClearTopSky(){
    VBK_REG = 1;    
    set_bkg_tiles(0,0,20,8,FlappyBirdBackground_map_attributes);  
    VBK_REG = 0;     
    set_bkg_tiles(0,0,20,8,FlappyBirdBackground_map); 
}

// Show the flappy bird title/logo at 4,1
// The assets assume their corresponding tiles are at the start of VRAM
// They are not, because of this, we need to use a helper array to offset their values
// Once we have offset their values we can display their tiles on the background layer
void ShowFlappyBirdLogo(){

    uint8_t i;

    // Put our title into VRAM after our score and the background tiles
    set_bkg_data(FlappyBirdBackground_TILE_COUNT+ScoreTilesLen,FlappyBirdTitle_TILE_COUNT,FlappyBirdTitle_tiles);
    
    // Add an additional background palette for the title
    set_bkg_palette(5,3,FlappyBirdTitle_palettes);

    // A helper array or passing offset plane 0 and 1 into vram
    unsigned char FlappyBirdTitle_map_offset[48];

    for(i=0;i<48;i++){

        // Offset by 5
        // Our FlappyBirdTitle thinks it's color palettes start at 0, when they really start at 5
        FlappyBirdTitle_map_offset[i]=FlappyBirdTitle_map_attributes[i]+5;
    }

    // Plane 1 for setting attributes
    VBK_REG = 1;    

    // Our title is 12 x 4 tiles in size
    set_bkg_tiles(4,1,12,4,FlappyBirdTitle_map_offset);  

    for(i=0;i<48;i++){
        
        // Offset by our background and score tiles
        // Our FlappyBirdTitle thinks it's tiles are at the start of VRAM, when really they are ater the background and score itles
        FlappyBirdTitle_map_offset[i]=FlappyBirdTitle_map[i]+FlappyBirdBackground_TILE_COUNT+ScoreTilesLen;
    }

    // Plane 0 for setting tile mappings
    VBK_REG = 0;    

    // Our title is 12 x 4 tiles in size
    set_bkg_tiles(4,1,12,4,FlappyBirdTitle_map_offset); 
}

void GameplayStartSetup(){

    // Clear up sprites for usage
    // Move all non-player sprites to large number
    // So our logic later tracks them as 'unused' (x > 176)
    // Also set their palette
    for(uint8_t i=2;i<40;i++){
        move_sprite(i,250,0);
        set_sprite_prop(i,1);
    }

    const UWORD TapToStartPalette[] = {TapToStartSpritesCGBPal0c0,TapToStartSpritesCGBPal0c1,TapToStartSpritesCGBPal0c2,TapToStartSpritesCGBPal0c3};
    const UWORD TapSpritePalette[] = {TapToStartSpritesCGBPal2c0,TapToStartSpritesCGBPal2c1,TapToStartSpritesCGBPal2c2,TapToStartSpritesCGBPal2c3};
    const UWORD BirdPalette[] = {BirdAndPipesCGBPal0c0,BirdAndPipesCGBPal0c1,BirdAndPipesCGBPal0c2,BirdAndPipesCGBPal0c3};
    const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};
    const UWORD PipesPalette[] = {BirdAndPipesCGBPal1c0,BirdAndPipesCGBPal1c1,BirdAndPipesCGBPal1c2,BirdAndPipesCGBPal1c3};    

    // Put our bird, the pipes, and the menu sprites (a button, and 'tap' sign) into VRAM
    set_sprite_data(0,BirdAndPipesLen,BirdAndPipes);
    set_sprite_data(BirdAndPipesLen,TapToStartSpritesLen,TapToStartSprites);

    // Set the palettes for our birds, pipes, and the menu sprites
    set_sprite_palette(0,2,BirdPalette);
    set_sprite_palette(1,1,PipesPalette);
    set_sprite_palette(2,1,BlackAndWhitePalette);
    set_sprite_palette(3,1,TapToStartPalette);
    set_sprite_palette(4,1,TapSpritePalette);

    // Clear the top sky so when we come from GameplayEnd the Board is painted over.
    ClearTopSky();

    // Show logo
    ShowFlappyBirdLogo();

    // put the bird off screen
    birdX=240;
    birdY=80;

    // Set our player's tiles
    set_sprite_tile(0,2);
    set_sprite_tile(1,4);

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

uint8_t GameplayStartUpdate(){  

    // Move tothe right, while our birdX is less than 24
    // Our bird starts at 240 (off screen tothe right)
    // We also check to make sure it's larger than 160 (the width of the screen in pixels)
    if(birdX<24||birdX>160)birdX++;

    move_sprite(0,birdX+8,birdY+16);
    move_sprite(1,birdX+16,birdY+16);

    // If the bird isn't in it's ready spot
    if(birdX<24||birdX>=160){

        // Drawing the Tap hand sprite at 0,0
        // sprites at 0,0 are not visible
        move_sprite(2,0,0);
        move_sprite(3,0,0);
    }else {

        // Drawing the Tap hand sprite Below the bird
        move_sprite(2,birdX+8,birdY+32);
        move_sprite(3,birdX+16,birdY+32);
    }

    // Sprites 4 and 5 are the "tap" sign
    // We want to show it on and off every 3 frames or so.
    // We'll use the floorBackgroundScroll since it's always moving
    if((floorBackgroundScroll/3)%2==0&&birdX>=24&&birdX<160){

        //Drawing the tap sign sprite to the right of the bird

        move_sprite(4,birdX+32,birdY+24);
        move_sprite(5,birdX+40,birdY+24);

    }else {

        // Drawing the tap sign sprite at 0,0
        // sprites at 0,0 are not visible

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
        for(uint8_t i=2;i<40;i++){
            move_sprite(i,250,0);
            set_sprite_prop(i,1);
        }

        ClearTopSky();

        return COREGAMELOOP;
    }

    return GAMEPLAYSTART;
}

