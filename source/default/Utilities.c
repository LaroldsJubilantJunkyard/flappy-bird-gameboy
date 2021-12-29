#include <gb/gb.h>





UINT8 GetAvailableSprite(){
    for(UINT8 i=2;i<40;i++){
        OAM_item_t * itm = &shadow_OAM[i];
        if(itm->x>=200)return i;
    }
    return 0;
}

UINT8 RandomNumber(UINT8 min, UINT8 max){

    unsigned char *ptr_div_reg = 0xFF04; 

    return min+(*(ptr_div_reg) % (max-min));    // get value at memory address
}

