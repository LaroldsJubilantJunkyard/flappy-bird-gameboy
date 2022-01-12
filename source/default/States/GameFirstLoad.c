#include <gb/gb.h>
#include <gb/cgb.h>
#include "Graphics/FlappyBirdTitle.h"
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/ScoreTiles.h"
#include "Graphics/TapToStartSprites.h"
#include "Common.h"


void HandleBackgroundScrolling(){

    // If the gameboy is drawing line 0 (the top of the screen)
    if(LYC_REG==0){

        // The interrupt should next trigger at line 63
        LYC_REG=63;

        // Move everything below on the background (until our next interrupt at 63) back to the starting position
        move_bkg(0,0);

        // Re-show sprites
        SHOW_SPRITES;

    // if the gameboy is drawing line 63
    }else if(LYC_REG==63){

        // The interrupt should next trigger at line 79
        LYC_REG=79;

        // Move everything below on the background (until our next interrupt at 79) according to this variable
        move_bkg(topBackgroundScroll,0);

    // if the gameboy is drawing line 79
    }else if(LYC_REG==79){

        // The interrupt should next trigger at line 95
        LYC_REG=95;

        // Move everything below on the background (until our next interrupt at 95) according to this variable
        move_bkg(midBackgroundScroll,0);

    // if the gameboy is drawing line 95
    }else if(LYC_REG==95){

        // The interrupt should next trigger at line 119
        LYC_REG=119;

        // Move everything below on the background (until our next interrupt at 119) according to this variable
        move_bkg(lowBackgroundScroll,0);

    // if the gameboy is drawing line 119
    }else if(LYC_REG==119){

        // The interrupt should next trigger at line 125
        LYC_REG=125;

        // Move everything below on the background (until our next interrupt at 125) according to this variable
        move_bkg(floorBackgroundScroll,0);

        // Prevent pipes from overlapping
        HIDE_SPRITES;

    // if the gameboy is drawing line 125
    }else if(LYC_REG==125){

        // The interrupt should next trigger at line 0
        LYC_REG=0;

        // Move everything below on the background (until our next interrupt at 0) back to the starting position
        move_bkg(0,0);
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

    // We're gonna use interrupts to achieve parallax scrolling
    // Set the LYC register at 0, where we will start the scrolling logic
    // From there we will move diferent chunks of the background different amounts
    STAT_REG|=0x40; //enable LYC=LY interrupt
    LYC_REG=0;
    disable_interrupts();
    add_LCD(HandleBackgroundScrolling);
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
    set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_attributes);    
    VBK_REG = 0;     
    set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map); 

}