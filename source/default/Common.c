#include <gb/gb.h>
#include "Graphics/FlappyBirdBackground.h"

// A counter for scrolling the background
// We'll move different parts of the background at different rates
// Top background = clouds - scroll 1px every 10 frames
// mid background = city - scroll 1px every 5 frames
// low background = bushes - scroll 1px every 2 frames
// floor background = ground - scroll 1px every frame
UINT8 backgroundScrollCounter=0,topBackgroundScroll=0,midBackgroundScroll=0,lowBackgroundScroll=0,floorBackgroundScroll=0;

// Used for keeping track  of the current and previous states of the gameboy joypad
UINT8 joypadCurrent;
UINT8 joypadPrevious;

INT16 birdY=80;
UINT8 birdX=24;
UINT8 score=0,highScore=0;

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

// Move the bird vertically based on birdVelocityY
// Set the bird as not alive if too low
// Choose the proper sprite for our bird
// Gameboy doesn't support rotation of sprites
// Thus we have extra sprites for different rotation states
void MoveAndUpdateFlappyBird(){    

    // our bird's default state is tile 2
    UINT8 tile=2;

    // Apply velocity
    birdY+=birdVelocityY/5;

    // Avoid going TOO low
    if(birdY>106){
        alive=0;
    }

    // use a different tile based on our y velocity
    if(birdVelocityY>10)tile=14;
    if(birdVelocityY>18)tile=18;
    if(birdVelocityY<-5)tile=6;

    // Set the proper tile for our bird
    set_sprite_tile(0,tile);
    set_sprite_tile(1,tile+2);
    
    move_sprite(0,birdX+8,birdY+16);
    move_sprite(1,birdX+8+8,birdY+16);
}

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



void UpdateScoreTextAt(UINT8 x, UINT8 y,UINT16 scoreToDisplay){

    unsigned char scorePlane1[] = {0x03,0x03,0x03};
    unsigned char scoreText[] = {0x77,0x77,0x77};

    // The score tiles start immediately after the background tiles
    scoreText[0]=(scoreToDisplay/100)%10+FlappyBirdBackground_TILE_COUNT;
    scoreText[1]=(scoreToDisplay/10)%10+FlappyBirdBackground_TILE_COUNT;
    scoreText[2]=scoreToDisplay%10+FlappyBirdBackground_TILE_COUNT;

    VBK_REG = 1;
    set_bkg_tiles(x,y,3,1,scorePlane1);

    VBK_REG = 0;
    set_bkg_tiles(x,y,3,1,scoreText);
}