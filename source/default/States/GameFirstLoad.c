#include <gb/gb.h>
#include <gb/cgb.h>
#include "Graphics/FlappyBirdTitle.h"
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/ScoreTiles.h"
#include "Graphics/MenuSprites.h"
#include "Common.h"


void HideSpritesForWindow(){

    // If the gameboy is drawing line 119 (where our ground/score starts)
    if(LYC_REG==0){

        move_bkg(0,0);

        // Hide sprites from here on
        SHOW_SPRITES;

        // The interrupt should next trigger at line 0
        LYC_REG=63;

    // if thegameboy is drawing line 0 (the top of the screen)
    }else if(LYC_REG==63){

        // The interrupt should next trigger at line 119
        LYC_REG=79;

        move_bkg(topBackgroundScroll,0);

    // if thegameboy is drawing line 0 (the top of the screen)
    }else if(LYC_REG==79){

        // The interrupt should next trigger at line 119
        LYC_REG=95;

        move_bkg(midBackgroundScroll,0);

    }else if(LYC_REG==95){

        // The interrupt should next trigger at line 119
        LYC_REG=119;

        move_bkg(lowBackgroundScroll,0);

    }else if(LYC_REG==119){

        move_bkg(floorBackgroundScroll,0);

        // Show sprites
        HIDE_SPRITES;

        // The 0 should next trigger at line 119
        LYC_REG=124;
    }else if(LYC_REG==124){

        move_bkg(0,0);

        // The 0 should next trigger at line 119
        LYC_REG=0;
    }

}

void GameFirstLoad(){

    // This enables Sound
    // these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x16;

    // We're gonna use interrupts to hide sprites where the window shows
    // Set the LYC register at 119, where we will start hiding sprites
    // From there we will wait until LYC_REG=0 (top of screen) to show sprites
    STAT_REG|=0x40; //enable LYC=LY interrupt
    LYC_REG=0;
    disable_interrupts();
    add_LCD(HideSpritesForWindow);
    set_interrupts(LCD_IFLAG|VBL_IFLAG);
    enable_interrupts();


    /////////////////////
    // Setting up VRAM //
    /////////////////////
    const UWORD ScoreTilesPalette[] ={ ScoreTilesCGBPal1c0,ScoreTilesCGBPal1c1,ScoreTilesCGBPal1c2,ScoreTilesCGBPal1c3} ;

    set_bkg_data(0,FlappyBirdBackground_TILE_COUNT,FlappyBirdBackground_tiles);   
    set_bkg_data(FlappyBirdBackground_TILE_COUNT,ScoreTilesLen,ScoreTiles);

    set_bkg_palette(0,3,FlappyBirdBackground_palettes); 
    set_bkg_palette(3,1,ScoreTilesPalette); 

    /////////////////////////
    // Draw our background //
    /////////////////////////

    VBK_REG = 1;    
    set_bkg_tiles(0,0,20,18,FlappyBirdBackground_map_attributes);  
    set_bkg_tiles(20,0,20,18,FlappyBirdBackground_map_attributes);   
    VBK_REG = 0;     
    set_bkg_tiles(0,0,20,18,FlappyBirdBackground_map); 
    set_bkg_tiles(20,0,20,18,FlappyBirdBackground_map); 
    

    topBackgroundScroll=0;
    midBackgroundScroll=0;
    lowBackgroundScroll=0;
    floorBackgroundScroll=0;

}