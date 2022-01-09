#include <gb/gb.h>
#include "Graphics/FlappyBirdBackground.h"

// A counter for scrolling the background
// We'll move the background every time this reaches 7, so it's really slow
UINT8 backgroundScrollCounter=0,topBackgroundScroll,midBackgroundScroll,lowBackgroundScroll,highScore=0,floorBackgroundScroll=0;

// Used for keeping track  of the current and previous states of the gameboy joypad
UINT8 joypadCurrent;
UINT8 joypadPrevious;

INT16 birdY=80;
UINT8 birdX=24;
UINT8 score=0;

// Are we currently between two pipes (one above, and one below)
// Later: Whenever this value changes from 1 to 0, we will increase score 
UINT8 betweenTwoPipes=0;

// How fast is the bird moving up or down
// This needs to be a signed integer so we can have negative (rising) values
INT8 birdVelocityY=0;

// Used to help spawn pipes
UINT8 distance=0;

// Is the bird alive (1) or dead (0)
UINT8 alive=0;

UINT8 ScrollSpritesForPipes(UINT8 speed){
    
    UINT8 numberOfInUseSprites=2;

    for(UINT8 i=2;i<40;i++){
        
        OAM_item_t * itm = &shadow_OAM[i];
        
        if(itm->x<=176){

            numberOfInUseSprites++;

            scroll_sprite(i,-speed,0);

            if(alive){

                // Check if we horizontally align with the player
                if(itm->x<birdX+2)continue;
                if(itm->x>birdX+21)continue;

                // If our x coordinates align with the player, 
                // They are 'betweenTwoPipes'
                betweenTwoPipes=1;

                // Check if we vertically align with the player
                if(itm->y<birdY)continue;
                if(itm->y>birdY+32)continue;

                alive=0;
                
            }

        }

    }

    return numberOfInUseSprites;
}



void UpdateScoreTextAt(UINT8 x, UINT8 y,UINT16 showscore,UINT8 useWindow){

    unsigned char scorePlane1[] = {0x03,0x03,0x03};
    unsigned char scoreText[] = {0x77,0x77,0x77};
    set_bkg_tiles(x,y,3,1,scorePlane1);

    scoreText[0]=(showscore/100)%10+FlappyBirdBackground_TILE_COUNT;
    scoreText[1]=(showscore/10)%10+FlappyBirdBackground_TILE_COUNT;
    scoreText[2]=showscore%10+FlappyBirdBackground_TILE_COUNT;

    VBK_REG = 1;
    if(useWindow)set_win_tiles(x,y,3,1,scorePlane1);
    else set_bkg_tiles(x,y,3,1,scorePlane1);

    VBK_REG = 0;
    if(useWindow)set_win_tiles(x,y,3,1,scoreText);
    else set_bkg_tiles(x,y,3,1,scoreText);
}