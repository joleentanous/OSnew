#include "os.h"
#define offset 12
#include <stdio.h>


void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn){
    printf("we are at page_table_update\n");
    //uint64_t res = page_table_query(pt, vpn);

    //printf("The cur_pt is: %p\n", cur_pt);
    //printf("The res is: %ld\n", res);

    //if (res == NO_MAPPING && ppn == NO_MAPPING){
    //    return ;
    //}
    int indx[5];
    int i, prefix ;
    for (i=0; i<=4; i++){
        indx[4-i] = ((vpn >> 9*i) & 0x1FF);
        printf("The symbol is: %d\n", indx[4-i]);
    }

    uint64_t *cur_pt = phys_to_virt(pt << offset);
    for (i=0; i<4; i++){
        prefix = cur_pt[indx[i]];
        printf("The prefix is: %d\n", prefix);
        if ((prefix & 1) == 0) {
            if (ppn == NO_MAPPING){
                return ;
            }
            uint64_t new_page = (alloc_page_frame() << 12) | 1;
            cur_pt[indx[i]] = new_page;
        }
        cur_pt = phys_to_virt(cur_pt[indx[i]]);
    }
    if (ppn == NO_MAPPING){
        cur_pt[indx[4]] = 0;
        return;
    }
    cur_pt[indx[4]] = ppn << offset;
    }




uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    int indx[5];
    int i, prefix ;
    printf("The vpn in query is: %lu\n", vpn);
    printf("The pt in query is: %lu\n", pt);
    for (i=0; i<=4; i++){
        indx[4-i] = ((vpn >> 9*i) & 0x1FF);
        printf("The symbol of query is: %d\n", indx[4-i]);
    }

    
    uint64_t *cur_pt = phys_to_virt(pt << offset);
    printf("The cur_pt of query is: %p\n", cur_pt);
    printf("any entry: %ld\n", cur_pt[20]);
    for (i=0; i<4; i++){
        prefix = cur_pt[indx[i]];
        printf("The prefix of query is: %d\n", prefix);
        printf("indx0 is: %d\n", indx[0]);
        printf("The prefixes of query are: %lu\n", cur_pt[indx[0]]);
        printf("The prefixes of query are: %lu\n", cur_pt[indx[1]]);
        printf("The prefixes of query are: %lu\n", cur_pt[indx[2]]);
        printf("The prefixes of query are: %lu\n", cur_pt[indx[3]]);
        printf("The prefixes of query are: %lu\n", cur_pt[indx[4]]);
    
        if ((prefix & 1) == 0) {
            return NO_MAPPING;
        }
        cur_pt = phys_to_virt(prefix << offset);
    }
    if ((cur_pt[indx[4]] & 1) == 0) {
        return NO_MAPPING;
    }
    return cur_pt[indx[4]] >> offset;

}


