#include <gb/gb.h>
#include "Graphics/FlappyBirdBackground.h"
#include "Graphics/BirdAndPipes.h"
#include "Graphics/GroundTiles.h"
#include "Common.h"


void HideSpritesForWindow(){

    // If the gameboy is drawing line 119 (where our ground/score starts)
    if(LYC_REG==119){

        // Hide sprites from here on
        HIDE_SPRITES;

        // The interrupt should next trigger at line 0
        LYC_REG=0;

    // if thegameboy is drawing line 0 (the top of the screen)
    }else if(LYC_REG==0){

        // Show sprites
        SHOW_SPRITES;

        // The interrupt should next trigger at line 119
        LYC_REG=119;
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
    SHOW_WIN;
    SPRITES_8x16;

    // We're gonna use interrupts to hide sprites where the window shows
    // Set the LYC register at 119, where we will start hiding sprites
    // From there we will wait until LYC_REG=0 (top of screen) to show sprites
    STAT_REG|=0x40; //enable LYC=LY interrupt
    LYC_REG=119;
    disable_interrupts();
    add_LCD(HideSpritesForWindow);
    set_interrupts(LCD_IFLAG|VBL_IFLAG);
    enable_interrupts();


    /////////////////////
    // Setting up VRAM //
    /////////////////////

    const UWORD BirdPalette[] = {BirdAndPipesCGBPal0c0,BirdAndPipesCGBPal0c1,BirdAndPipesCGBPal0c2,BirdAndPipesCGBPal0c3};
    const UWORD PipesPalette[] = {BirdAndPipesCGBPal1c0,BirdAndPipesCGBPal1c1,BirdAndPipesCGBPal1c2,BirdAndPipesCGBPal1c3};
    const UWORD GroundPalette[] ={ 
        GroundTilesCGBPal0c0,GroundTilesCGBPal0c1,GroundTilesCGBPal0c2,GroundTilesCGBPal0c3,
        GroundTilesCGBPal1c0,GroundTilesCGBPal1c1,GroundTilesCGBPal1c2,GroundTilesCGBPal1c3,
        GroundTilesCGBPal2c0,GroundTilesCGBPal2c1,GroundTilesCGBPal2c2,GroundTilesCGBPal2c3
        } ;
    const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};

    set_bkg_data(0,FlappyBirdBackground_tileCount,FlappyBirdBackground_data);   
    set_bkg_data(FlappyBirdBackground_tileCount,12,GroundTiles);
    set_sprite_data(0,22,BirdAndPipes);

    set_bkg_palette(0,1,&FlappyBirdBackground_pallette[0][0]); 
    set_bkg_palette(1,1,&FlappyBirdBackground_pallette[1][0]);
    set_bkg_palette(2,3,GroundPalette);

    set_sprite_palette(0,2,BirdPalette);
    set_sprite_palette(1,1,PipesPalette);
    set_sprite_palette(2,1,BlackAndWhitePalette);

    // Set our player's tiles
    set_sprite_tile(0,2);
    set_sprite_tile(1,4);

    /////////////////////////
    // Draw our background //
    /////////////////////////

    VBK_REG = 1;    
    set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_plane1);     
    VBK_REG = 0;     
    set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_plane0); 

    ///////////////////////////////////
    // Draw our ground on the window //
    ///////////////////////////////////

    move_win(7,120);

    // Paint the 'ground' on the top of the window
    for(UINT8 i=0;i<32;i++){

        unsigned char groundOneTwo[] = {0x75,0x76,0x76};
        unsigned char groundOneTwoPlane1[] = {0x02,0x03,0x03};

        VBK_REG = 1;
        set_win_tiles(i,0,1,3,groundOneTwoPlane1);

        VBK_REG = 0;
        set_win_tiles(i,0,1,3,groundOneTwo);
    }

}