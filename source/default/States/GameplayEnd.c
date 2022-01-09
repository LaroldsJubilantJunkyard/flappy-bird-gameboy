#include <gb/gb.h>
#include <gb/metasprites.h>
#include "Common.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/ScoreTiles.h"
#include "Graphics/FlappyBirdEnd.h"
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/FlappyBirdMedals.h"


UINT8 created=0;

void ShowGameplayEndBoard(){

    unsigned char coverScoreTiles[3]={0,0,0};
    VBK_REG = 1;   
    get_bkg_tiles(0,17,3,1,coverScoreTiles);
    set_bkg_tiles(8,16,3,1,coverScoreTiles);
    VBK_REG = 0;     
    get_bkg_tiles(0,17,3,1,coverScoreTiles);
    set_bkg_tiles(8,16,3,1,coverScoreTiles);

    // Show logo

    unsigned char FlappyBirdEnd_map_offset2[98];
    unsigned char FlappyBirdEnd_map_offset[98];


    for(UINT8 i=0;i<98;i++){
        FlappyBirdEnd_map_offset2[i]=FlappyBirdEnd_map_attributes[i]+5;
        FlappyBirdEnd_map_offset[i]=FlappyBirdEnd_map[i]+FlappyBirdBackground_TILE_COUNT+ScoreTilesLen;
    }
        
    set_bkg_data(FlappyBirdBackground_TILE_COUNT+ScoreTilesLen,FlappyBirdEnd_TILE_COUNT,FlappyBirdEnd_tiles);
    set_bkg_palette(5,2,FlappyBirdEnd_palettes);

    VBK_REG = 1;    
    set_bkg_tiles(3,1,14,7,FlappyBirdEnd_map_offset2);
    VBK_REG = 0;     
    set_bkg_tiles(3,1,14,7,FlappyBirdEnd_map_offset); 

    UpdateScoreTextAt(13,3,score,0);
    UpdateScoreTextAt(13,6,highScore,0);

    if(score>10){

        set_sprite_data(24,64,FlappyBirdMedals_tiles);
        set_sprite_palette(0,3,FlappyBirdMedals_palettes);

        UINT8 medal = 0;

        if(score>20)medal=1;
        if(score>30)medal=2;
        if(score>40)medal=3;

        // inline uint8_t move_metasprite(const metasprite_t * metasprite, uint8_t base_tile, uint8_t base_sprite, uint8_t x, uint8_t y) {
        move_metasprite(FlappyBirdMedals_metasprites[medal],24,2,60,61);

    }
}

void GameplayEndSetup(){

    // Play a noise
    NR10_REG=0x7C;
    NR11_REG=0x41;
    NR12_REG=0x73;
    NR13_REG=0x73;
    NR14_REG=0x86;

    created=0;
}

UINT8 GameplayEndUpdate(){

        
    UINT8 numberOfInUseSprites=2;
    
    if(created==0){
        if(birdY>=176)numberOfInUseSprites=ScrollSpritesForPipes(4);
        else numberOfInUseSprites=ScrollSpritesForPipes(1);
    }

    birdX-=1;

    // While the player's y isn't really large
    // We're going to make the player fall off screen
    // Eventually the player will physically reach below (greater than) 176 and the loop will stop
    if(birdY<176){

        birdY+=birdVelocityY/5;

        UINT8 tile=2;

        if(birdVelocityY>10)tile=14;
        if(birdVelocityY>18)tile=18;
        if(birdVelocityY<-5)tile=6;

        set_sprite_tile(0,tile);
        set_sprite_tile(1,tile+2);

        // Reposition the bird's sprites
        move_sprite(0,birdX+8,birdY+16);
        move_sprite(1,birdX+8+8,birdY+16);

    // If the bird is far off screen, let's scroll in use sprites until none are on screen.
    }else if(created==0 && numberOfInUseSprites==2){

        created=1;

        ShowGameplayEndBoard();

    }else{

        if((joypadCurrent & J_A) && !(joypadPrevious & J_A)){
            return GAMEPLAYSTART;
        }
    }

    return GAMEPLAYEND;
}
