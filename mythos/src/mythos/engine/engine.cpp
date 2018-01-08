#include <iostream>
#include <string>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/engine/graphics.h"


namespace engine {


int FRAME_RATE;
int FRAMES_BEFORE_DROP;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

int get_screen_width() {
    return SCREEN_WIDTH;
}

int get_screen_height() {
    return SCREEN_HEIGHT;
}


ALLEGRO_DISPLAY*        al_display		= nullptr;
ALLEGRO_EVENT_QUEUE**   al_queue		= nullptr;
ALLEGRO_EVENT_QUEUE*    al_timer_queue	= nullptr;
ALLEGRO_TIMER*          al_timer		= nullptr;


State*                  state_curr;
State*                  state_next;

State* get_state() {
    return state_curr;
}

void set_state(State* s) {
    if (state_curr)
        state_next = s;
    else
        state_curr = s;
}

void swap_state() {
    delete state_curr;
    state_curr = state_next;
    state_next = nullptr;
}


void init_allegro() {
	// initialize Allegro
    if (!al_init())
        throw "Allegro library failed to initialize.";
    if (!al_install_mouse())
        throw "Allegro mouse addon failed to initialize.";
    if (!al_install_keyboard())
        throw "Allegro keyboard addon failed to initialize.";
    if (!al_init_image_addon())
        throw "Allegro image addon failed to initialize.";
    if (!al_init_font_addon())
        throw "Allegro font addon failed to initialize.";
    if (!al_init_ttf_addon())
        throw "Allegro ttf addon failed to initialize.";
    if (!al_init_primitives_addon())
        throw "Allegro primitives addon failed to initialize.";

    state_curr = nullptr;
    state_next = nullptr;
}

void init_timer() {
	if (al_timer)
		al_destroy_timer(al_timer);

	al_timer = nullptr;
	al_timer = al_create_timer(1.0 / FRAME_RATE);
	if (!al_timer)
		throw "Allegro timer failed to initialize.";
}

void init_display() {
	if (al_display)
		al_destroy_display(al_display);

	al_display = nullptr;
	al_display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!al_display)
		throw "Allegro display failed to initialize.";
}

void init_event_queue() {
	if (al_queue) {
		for (int i = FRAMES_BEFORE_DROP - 1; i >= 0; --i)
			al_destroy_event_queue(al_queue[i]);
		delete[] al_queue;
	}
	if (al_timer_queue)
		al_destroy_event_queue(al_timer_queue);

	al_queue = new ALLEGRO_EVENT_QUEUE*[FRAMES_BEFORE_DROP];
    for (int i = FRAMES_BEFORE_DROP - 1; i >= 0; --i) {
        al_queue[i] = nullptr;
        al_queue[i] = al_create_event_queue();
        if (!al_queue[i])
            throw "Allegro event queue failed to initialize.";
        al_register_event_source(al_queue[i], al_get_display_event_source(al_display));
        al_register_event_source(al_queue[i], al_get_mouse_event_source());
        al_register_event_source(al_queue[i], al_get_keyboard_event_source());
        al_register_event_source(al_queue[i], al_get_timer_event_source(al_timer));
    }

	al_timer_queue = nullptr;
    al_timer_queue = al_create_event_queue();
    if (!al_timer_queue)
        throw "Allegro event queue failed to initialize.";
    al_register_event_source(al_timer_queue, al_get_timer_event_source(al_timer));
}

void init(char const* config_file) {
    // load configurations
    ALLEGRO_CONFIG* config = al_load_config_file(config_file);

    FRAME_RATE = atoi(al_get_config_value(config, NULL, "frame rate"));
    FRAMES_BEFORE_DROP = FRAME_RATE / 15;
    SCREEN_WIDTH = atoi(al_get_config_value(config, NULL, "display width"));
    SCREEN_HEIGHT = atoi(al_get_config_value(config, NULL, "display height"));

    // finish Allegro initialization
    init_timer();
    init_display();
    init_event_queue();
}


void run() {
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

    ALLEGRO_EVENT e;
    al_start_timer(al_timer);

    int frame_receive = 0;
    int frame_inspect = 0;

    while (true) {
        if (al_is_event_queue_empty(al_timer_queue)) {
			al_wait_for_event(al_queue[frame_receive], &e);
			al_pause_event_queue(al_queue[frame_receive], true);
			frame_receive = (frame_receive + 1) % FRAMES_BEFORE_DROP;
			al_flush_event_queue(al_queue[frame_receive]);
			al_pause_event_queue(al_queue[frame_receive], false);
			if (frame_inspect == frame_receive)
				frame_inspect = (frame_inspect + 1) % FRAMES_BEFORE_DROP;
		} else {
			while (!al_is_event_queue_empty(al_timer_queue)) {
				al_get_next_event(al_timer_queue, &e);
				al_pause_event_queue(al_queue[frame_receive], true);
				frame_receive = (frame_receive + 1) % FRAMES_BEFORE_DROP;
				al_flush_event_queue(al_queue[frame_receive]);
				al_pause_event_queue(al_queue[frame_receive], false);
				if (frame_inspect == frame_receive)
					frame_inspect = (frame_inspect + 1) % FRAMES_BEFORE_DROP;
			}
		}

		while (al_is_event_queue_empty(al_queue[frame_inspect])) {
			frame_inspect = (frame_inspect + 1) % FRAMES_BEFORE_DROP;
			if (frame_inspect == frame_receive)
				break;
		}
		if (!al_is_event_queue_empty(al_queue[frame_inspect]))
			al_get_next_event(al_queue[frame_inspect], &e);

        if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        else if (e.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(black);
            state_curr->update_frame();
            al_flip_display();
        } else
            state_curr->update_event(&e);

        if (state_next)
            swap_state();
    }
}

void destroy_allegro() {
	if (al_timer_queue)
		al_destroy_event_queue(al_timer_queue);

	if (al_queue) {
		for (int f = FRAMES_BEFORE_DROP - 1; f >= 0; --f) {
			if (al_queue[f])
				al_destroy_event_queue(al_queue[f]);
		}
	}

	if (al_display)
		al_destroy_display(al_display);

	if (al_timer)
		al_destroy_timer(al_timer);
}


}
