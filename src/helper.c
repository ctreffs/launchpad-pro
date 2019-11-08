#include "helper.h"

u8 is_pad(u8 index) {
    u8 r = index % 10;
    u8 q = index / 10;
    return (r > 0 && r < 9 && q < 9 && q > 0);
}

u8 is_section_button(u8 index) {
    return (index > 0 && index < 9);
}

u8 is_page_button(u8 index) {
    return (index % 10 == 9);
}

u8 in_array(u8 value, u8 arr[], u8 size) {
    for (u8 i = 0; i < size; ++i) {
        if (arr[i] == value) {
            //debug2(30);
            return i;
        }
    }

    display_plot_led(30, 0);

    return 0;
}

//void debug2(u8 index) {
//    display_plot_led(index, rand()/100);
//}