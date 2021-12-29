#include <gb/gb.h>
extern UWORD FlappyBirdBackground_pallette[5][4];
extern UINT16 FlappyBirdBackground_tileCount;
extern unsigned char FlappyBirdBackground_data[];
extern unsigned char FlappyBirdBackground_map_plane1[];
extern unsigned char FlappyBirdBackground_map_plane0[];
#define DRAW_FLAPPYBIRDBACKGROUND_NO_PALETTE set_bkg_data(0,117,FlappyBirdBackground_data);VBK_REG = 1; set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_plane1); VBK_REG = 0; set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_plane0);
 #define DRAW_FLAPPYBIRDBACKGROUND set_bkg_palette(0,1,&FlappyBirdBackground_pallette[0][0]); set_bkg_palette(1,1,&FlappyBirdBackground_pallette[1][0]);set_bkg_data(0,117,FlappyBirdBackground_data);VBK_REG = 1; set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_plane1); VBK_REG = 0; set_bkg_tiles(0,0,32,18,FlappyBirdBackground_map_plane0); 