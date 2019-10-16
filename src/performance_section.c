#include "performance_section.h"
#include "piano.h"
#include "helper.h"
#include "modulation_pad.h"
#include "app_defs.h"

Piano piano = {
    0,
    0, 
    0,
    {0, 11, 1, 12, 2, 3, 14, 4, 15, 5, 16, 6, 7}
};

ModulationPad test_pad = {};

void piano_section_controller(u8 index, u8 value) {
    u8 size = sizeof piano.pads_intervals / sizeof *piano.pads_intervals;
    u8 midi_note = in_array(index - piano.c, piano.pads_intervals, size);

    if (value > 0) {
        midi_send(DINMIDI, NOTEON, piano.midi_channel, midi_note + (12 * piano.octave), value);
    } else {
        // midi_send(DINMIDI, NOTEOFF, piano.midi_channel, midi_note + (12 * piano.octave), 0); // TODO
    }
}

void performance_section_draw() {
    display_draw_piano(0);
    modulation_pad_draw(&test_pad);
}

void performance_section_change_page(u8 index) {
}

void performance_section_init() {
    piano.octave = 3;
    piano.c = 71;
    piano.midi_channel = 8;

    test_pad.color = 0x345723;
    test_pad.midi_channel = 8;
    test_pad.cc = 16; // Pitch
    test_pad.index = 11;
}

void performance_section_handler(u8 index, u8 value) {
    /*if (index >= piano.c && index <= piano.c + 7 ||
        index >= piano.c + 10 && index <= piano.c + 17) {
        piano_section_controller(index, value);
    }*/

    piano_section_controller(index, value);

    modulation_pad_handler(&test_pad, index, value);
}
