#include <gb/gb.h>
#include "Common.h"
#include "Utilities.h"


void UpdateScoreText(){

    unsigned char scorePlane1[] = {0x03,0x03,0x03};
    unsigned char scoreText[] = {0x77,0x77,0x77};

    scoreText[0]=(score/100)%10+0x77;
    scoreText[1]=(score/10)%10+0x77;
    scoreText[2]=score%10+0x77;

    VBK_REG = 1;
    set_win_tiles(8,1,3,1,scorePlane1);

    VBK_REG = 0;
    set_win_tiles(8,1,3,1,scoreText);
}

void SpawnPipes(){
        
    // The size of the top pipe, how much space in-between, and the size of the bottom pipe
    UINT8 top = RandomNumber(1,4);
    UINT8 space = RandomNumber(3,4);

    // Randomly move the top upwards and the bottom downwards some.
    UINT8 rdrop1 = RandomNumber(0,16);
    UINT8 rdrop2 = RandomNumber(0,16);
    
    // The top pipe
    // Each iteration of the for loop represents one 16x16 portion of a sprite ( 2 sprites)
    for(UINT8  j=0;j<top;j++){

        UINT8 spr = GetAvailableSprite();


        // We checked that we had enough sprites
        // However, if for any reason this returns 0: stop everything
        if(spr==0)return;

        // Set the sprite's tile, it's color palette, and it's position
        set_sprite_tile(spr,j==(top-1)?16:10);
        set_sprite_prop(spr,1);
        move_sprite(spr, 168,j*16+16-rdrop1);
        
        
        spr = GetAvailableSprite();


        // We checked that we had enough sprites
        // However, if for any reason this returns 0: stop everything
        if(spr==0)return;

        // Set the sprite's tile, it's color palette, and it's position
        set_sprite_tile(spr,j==(top-1)?20:12);
        set_sprite_prop(spr,1);
        move_sprite(spr, 176,j*16+16-rdrop1);

    }

    // The bottom pipe
    // Each iteration of the for loop represents one 16x16 portion of a sprite ( 2 sprites)
    for(UINT8 j=top+space;j<8;j++){

        UINT8 spr = GetAvailableSprite();


        // We checked that we had enough sprites
        // However, if for any reason this returns 0: stop everything
        if(spr==0)return;

        // Set the sprite's tile, it's color palette, and it's position
        set_sprite_tile(spr,j==(top+space)?18:10);
        set_sprite_prop(spr,1);
        move_sprite(spr, 168,j*16+16+rdrop2);

        
        spr = GetAvailableSprite();

        // We checked that we had enough sprites
        // However, if for any reason this returns 0: stop everything
        if(spr==0)return;

        // Set the sprite's tile, it's color palette, and it's position
        set_sprite_tile(spr,j==(top+space)?14:12);
        set_sprite_prop(spr,1);
        move_sprite(spr, 176,j*16+16+rdrop2);

    }
    
}


void CoreGameLoopSetup(){


    // Update the bird's palette to be it's normal one.
    set_sprite_prop(0,0);
    set_sprite_prop(1,0);

    alive=1;
    score=0;
    distance=0;
    betweenTwoPipes=0;
    birdVelocityY=-14;

    UpdateScoreText();

}

UINT8 CoreGameLoopUpdate(){

    // If we jsut pressed the A button
    if((joypadCurrent & J_A) && !(joypadPrevious & J_A)){

        // Set our y velocity to negative, to rise up
        birdVelocityY=-14;

        // PLay a sound
        NR10_REG=0x1C;
        NR11_REG=0x44;
        NR12_REG=0x42;
        NR13_REG=0x43;
        NR14_REG=0x86;

    }


    birdY+=birdVelocityY/5;

    if(birdY>106){
        alive=0;
    }
    
    move_sprite(0,birdX+8,birdY+16);
    move_sprite(1,birdX+8+8,birdY+16);

    // Save whether or not we WERE previously between tiles
    UINT8 wasBetweenTiles=betweenTwoPipes;

    // Set us as NOT inbetween tiles by default
    betweenTwoPipes=0;
    
    UINT8 numberOfInUseSprites = ScrollSpritesForPipes(1);    

    // Wait until a random amount of distance as passed
    if(distance!=0)distance--;

    // make sure we have enough sprites NOT in use
    // The  top/bottom pipe combinations take around 10 sprites maximum
    // So make sure we don't have more than 30 in use (there are 40 sprites max for gameboy games)
    else if(numberOfInUseSprites<30){

        SpawnPipes();

        // Wait a random amount of distance until we spawn another pipe
        distance=RandomNumber(36,76);
    }

    // If we were between tiles, but now we are not
    // We want to increase our score and play a sound
    if(wasBetweenTiles==1&&betweenTwoPipes==0){

        NR21_REG=0x81;
        NR22_REG=0x44;
        NR23_REG=0xD7;
        NR24_REG=0x86;

        score++;
        
        UpdateScoreText();
    }

    if(alive==0)return GAMEPLAYEND;
    else return COREGAMELOOP;
}


