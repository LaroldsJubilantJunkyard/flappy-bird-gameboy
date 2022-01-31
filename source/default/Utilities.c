#include <gb/gb.h>





uint8_t GetAvailableSprite(){
    for(uint8_t i=2;i<40;i++){
        OAM_item_t * itm = &shadow_OAM[i];
        if(itm->x>=200)return i;
    }
    return 0;
}

uint8_t RandomNumber(uint8_t min, uint8_t max){

    unsigned char *ptr_div_reg = 0xFF04; 

    return min+(*(ptr_div_reg) % (max-min));    // get value at memory address
}

