#include "Engine.h"
#include <iostream>

Engine::Engine()
    : display(nullptr), event_queue(nullptr), timer(nullptr), font(nullptr),
      screen_width(800), screen_height(600), display_mode(WINDOWED),
      fps(60), running(false), mouse_installed(false), keyboard_installed(false){

    log_file.open("engine_log.txt", std::ios::out | std::ios::app);
    log_message("Engine constructor called");
    log_file.close();
}

Engine::~Engine(){
    cleanup();
    if (log_file.is_open()){
        log_message("Engine destructor called");
        log_file.close()
    }
}

bool Engine::initialize(int width, int height, DisplayMode mode, int target_fps){
    creen_width=width;
    screen_height=height;
    display_mode=mode;
    fps=target_fps;

    log_message("initialize engine...");

    if (!al_init()){
        show_error("Failed initialize Allegro");
        return false;
    }

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    if(!initialize_graphics()){
        return false;
    }

        if(!initialize_input()){
        return false;
    }

    timer=al_create_timer(1.0 / fps);
        if(!event_queue){
            show_error("Failed to create timer");
            return false;
    }

    event_queue=al_create_event_queue();
        if(!event_queue){
            show_error("failed to create event queue");
            return false;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    if(keyboard_installed){
        al_register_event_source(event_queue, al_get_keyboard_event_source());
    }

    if(mouse_installed){
        al_register_event_source(event_queue, al_get_mouse_event_source());
    }

    font=al_create_builtin_font();
    if (!font){
        log_message("Warning: Could not create built-in font");
    }

    log_message("Engine initalizied succesfully");
    return true;
}

bool Engine::initialize_input(){}
