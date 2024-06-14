#include "os.h"
#define OFFSET 12
#include <stdio.h>


void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn){
    int indx[5];
    for (int i=0; i<=4; i++){
        indx[4-i] = (vpn >> (9*i)) & 0x1FF;
    }

    uint64_t* cur_pt;
    uint64_t shifted = pt << OFFSET;
    for (int i=0; i<4; i++){
        cur_pt = (uint64_t*)phys_to_virt(shifted);
        uint64_t prefix = cur_pt[indx[i]];
        if ((prefix & 1) == 0) {
            if (ppn == NO_MAPPING){
                return ;
            }
            uint64_t new_page = (alloc_page_frame() << OFFSET) | 1;
            cur_pt[indx[i]] = new_page;
        }
        shifted = (cur_pt[indx[i]] >> OFFSET) << OFFSET; 
    }
    cur_pt = (uint64_t*)phys_to_virt(shifted);
    if (ppn == NO_MAPPING){
        cur_pt[indx[4]] = 0;
        return;
    }
    cur_pt[indx[4]] = (ppn << OFFSET) | 1 ;
}




uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    int indx[5];
    for (int i=0; i<=4; i++){
        indx[4-i] = (vpn >> (9*i)) & 0x1FF;
    }
    
    uint64_t* cur_pt;
    uint64_t shifted = pt << OFFSET;
    for (int i=0; i<4; i++){
        cur_pt = (uint64_t*)phys_to_virt(shifted);
        uint64_t prefix = cur_pt[indx[i]];
    
        if ((prefix & 1) == 0) {
            return NO_MAPPING;
        }
        shifted = (cur_pt[indx[i]] >> OFFSET) << OFFSET; 
    }
    cur_pt = (uint64_t*)phys_to_virt(shifted);
    if ((cur_pt[indx[4]] & 1) == 0) {
        return NO_MAPPING;
    }
    return cur_pt[indx[4]] >> OFFSET;

}


