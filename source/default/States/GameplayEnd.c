#include <gb/gb.h>
#include <gb/metasprites.h>
#include "Common.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/ScoreTiles.h"
#include "Graphics/FlappyBirdEnd.h"
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/FlappyBirdMedals.h"

// This function will cover showing our high score board.
// Here's everything it will do:
// - Cover the score on the bottom of the background
// - Show the high score board on the background
// - Score the score and high score
// - Conditionally show the medal earned.
void ShowGameplayEndBoard(){

    ////////////////////////////////////////////////////////
    // Cover up the score at the bottom of the background //
    ////////////////////////////////////////////////////////

    unsigned char coverScoreTiles[3]={0,0,0};

    VBK_REG = 1;   
    get_bkg_tiles(0,17,3,1,coverScoreTiles);
    set_bkg_tiles(8,16,3,1,coverScoreTiles);
    VBK_REG = 0;     
    get_bkg_tiles(0,17,3,1,coverScoreTiles);
    set_bkg_tiles(8,16,3,1,coverScoreTiles);

    ///////////////////////////////
    // Show our high score board //
    ///////////////////////////////

    unsigned char FlappyBirdEnd_map_offset2[98];
    unsigned char FlappyBirdEnd_map_offset[98];

    // Our gameplay end board expects to have it's tiles & palettes at the start of VRAM
    // Because this is not the case, we need to use 2 helper arrays to offset these values
    // The color palettes should start at 5
    // The tiles should be after the background & scores tiles.
    for(uint8_t i=0;i<98;i++){
        FlappyBirdEnd_map_offset2[i]=FlappyBirdEnd_map_attributes[i]+5;
        FlappyBirdEnd_map_offset[i]=FlappyBirdEnd_map[i]+FlappyBirdBackground_TILE_COUNT+ScoreTilesLen;
    }
        
    set_bkg_data(FlappyBirdBackground_TILE_COUNT+ScoreTilesLen,FlappyBirdEnd_TILE_COUNT,FlappyBirdEnd_tiles);
    set_bkg_palette(5,2,FlappyBirdEnd_palettes);

    VBK_REG = 1;    
    set_bkg_tiles(3,1,14,7,FlappyBirdEnd_map_offset2);
    VBK_REG = 0;     
    set_bkg_tiles(3,1,14,7,FlappyBirdEnd_map_offset); 

    /////////////////////////////////////////////
    // Draw our scores on the high score board //
    /////////////////////////////////////////////

    UpdateScoreTextAt(13,3,score);
    UpdateScoreTextAt(13,6,highScore);

    ///////////////////////////////////////////////////
    // Show our medal if we have more than 10 points //
    ///////////////////////////////////////////////////

    if(score>10){

        set_sprite_data(24,64,FlappyBirdMedals_tiles);
        set_sprite_palette(0,3,FlappyBirdMedals_palettes);

        uint8_t medal = 0;

        if(score>20)medal=1;
        if(score>30)medal=2;
        if(score>40)medal=3;

        // inline uint8_t move_metasprite(const metasprite_t * metasprite, uint8_t base_tile, uint8_t base_sprite, uint8_t x, uint8_t y) {
        move_metasprite(FlappyBirdMedals_metasprites[medal],24,2,60,61);

    }
}

uint8_t areShowingGameplayEndBoard=0;

void GameplayEndSetup(){

    // Play a noise
    NR10_REG=0x7C;
    NR11_REG=0x41;
    NR12_REG=0x73;
    NR13_REG=0x73;
    NR14_REG=0x86;

    areShowingGameplayEndBoard=0;
}

uint8_t GameplayEndUpdate(){

        
    uint8_t numberOfInUseSprites=2;
    
    if(areShowingGameplayEndBoard==0){

        // Scroll the pipes faster when the bird is fully offscreen
        if(birdY>=176)numberOfInUseSprites=ScrollSpritesForPipes(4);
        else numberOfInUseSprites=ScrollSpritesForPipes(1);
    }

    // Move our bird to the left along with our scrolling pipes
    birdX-=1;

    // While the player's y isn't really large
    // We're going to make the player fall off screen
    // Eventually the player will physically reach below (greater than) 176 and the loop will stop
    if(birdY<176){

        MoveAndUpdateFlappyBird();

    // If the bird is far off screen, let's scroll in use sprites until none are on screen.
    }else if(areShowingGameplayEndBoard==0 && numberOfInUseSprites==2){

        areShowingGameplayEndBoard=1;

        ShowGameplayEndBoard();

    }else{

        if((joypadCurrent & J_A) && !(joypadPrevious & J_A)){
            return GAMEPLAYSTART;
        }
    }

    return GAMEPLAYEND;
}
