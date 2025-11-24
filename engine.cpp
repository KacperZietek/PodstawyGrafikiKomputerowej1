#include "Engine.h"
#include <iostream>

Engine::Engine()
    : display(nullptr), event_queue(nullptr), timer(nullptr), font(nullptr), buffer(nullptr),
      screen_width(800), screen_height(600), display_mode(WINDOWED),
      fps(60), running(false), mouse_installed(false), keyboard_installed(false),
      double_buffering(true) {

    log_file.open("engine_log.txt", std::ios::out | std::ios::app);
    log_message("Engine constructor called");
}

Engine::~Engine() {
    cleanup();
    if (log_file.is_open()) {
        log_message("Engine destructor called");
        log_file.close();
    }
}

bool Engine::initialize(int width, int height, DisplayMode mode, int target_fps, bool use_double_buffering) {
    screen_width = width;
    screen_height = height;
    display_mode = mode;
    fps = target_fps;
    double_buffering = use_double_buffering;

    log_message("Initializing engine...");

    if (!al_init()) {
        show_error("Failed to initialize Allegro!");
        return false;
    }

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    if (!initialize_graphics()) {
        return false;
    }

    if (!initialize_input()) {
        return false;
    }

    timer = al_create_timer(1.0 / fps);
    if (!timer) {
        show_error("Failed to create timer!");
        return false;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        show_error("Failed to create event queue!");
        return false;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    if (keyboard_installed) {
        al_register_event_source(event_queue, al_get_keyboard_event_source());
    }

    if (mouse_installed) {
        al_register_event_source(event_queue, al_get_mouse_event_source());
    }

    // Tworzenie bufora po�redniego je�li podw�jne buforowanie jest w��czone
    if (double_buffering) {
        buffer = al_create_bitmap(screen_width, screen_height);
        if (!buffer) {
            log_message("Warning: Could not create intermediate buffer");
            double_buffering = false;
        } else {
            log_message("Double buffering enabled");
        }
    }

    font = al_create_builtin_font();
    if (!font) {
        log_message("Warning: Could not create built-in font");
    }

    log_message("Engine initialized successfully");
    return true;
}

bool Engine::initialize_graphics() {
    int flags = 0;

    if (display_mode == FULLSCREEN) {
        flags |= ALLEGRO_FULLSCREEN_WINDOW;
    }

    al_set_new_display_flags(flags);
    display = al_create_display(screen_width, screen_height);

    if (!display) {
        show_error("Failed to create display!");
        return false;
    }

    al_set_window_title(display, "2D Game Engine");
    log_message("Graphics initialized");
    return true;
}

bool Engine::initialize_input() {
    // Inicjalizacja klawiatury
    keyboard_installed = al_install_keyboard();
    if (keyboard_installed) {
        log_message("Keyboard installed successfully");
    } else {
        log_message("Warning: Keyboard installation failed");
    }

    // Inicjalizacja myszy
    mouse_installed = al_install_mouse();
    if (mouse_installed) {
        log_message("Mouse installed successfully");
    } else {
        log_message("Warning: Mouse installation failed");
    }

    return true;
}

void Engine::run() {
    if (!display || !timer || !event_queue) {
        show_error("Engine not properly initialized!");
        return;
    }

    running = true;
    al_start_timer(timer);

    log_message("Starting main game loop");

    float last_time = al_get_time();
    float current_time, delta_time;

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        current_time = al_get_time();
        delta_time = current_time - last_time;
        last_time = current_time;

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;

            case ALLEGRO_EVENT_TIMER:
                begin_frame(); // Rozpocznij ramke
                handle_input();
                update(delta_time);
                render();
                end_frame(); // Zakoncz ramke
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                on_key_press(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                on_key_release(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                on_mouse_click(event.mouse.button, event.mouse.x, event.mouse.y);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                on_mouse_move(event.mouse.x, event.mouse.y);
                break;
        }
    }

    log_message("Game loop ended");
}

void Engine::begin_frame() {
    if (double_buffering && buffer) {
        al_set_target_bitmap(buffer);
    } else {
        al_set_target_backbuffer(display);
    }
}

void Engine::end_frame() {
    if (double_buffering && buffer) {
        al_set_target_backbuffer(display);
        al_draw_bitmap(buffer, 0, 0, 0);
    }
    al_flip_display();
}

void Engine::stop() {
    running = false;
    log_message("Engine stop requested");
}

void Engine::clear_screen(ALLEGRO_COLOR color) {
    al_clear_to_color(color);
}

void Engine::clear_bitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color) {
    if (bitmap) {
        al_set_target_bitmap(bitmap);
        al_clear_to_color(color);
        al_set_target_backbuffer(display);
    }
}

void Engine::log_message(const std::string& message) {
    std::string log_msg = "[LOG] " + message;
    std::cout << log_msg << std::endl;

    if (log_file.is_open()) {
        log_file << log_msg << std::endl;
    }
}

void Engine::show_error(const std::string& error) {
    std::string error_msg = "[ERROR] " + error;
    std::cerr << error_msg << std::endl;

    if (log_file.is_open()) {
        log_file << error_msg << std::endl;
    }
}

void Engine::cleanup() {
    log_message("Cleaning up resources...");

    if (buffer) {
        al_destroy_bitmap(buffer);
        buffer = nullptr;
    }

    if (font) {
        al_destroy_font(font);
        font = nullptr;
    }

    if (timer) {
        al_destroy_timer(timer);
        timer = nullptr;
    }

    if (event_queue) {
        al_destroy_event_queue(event_queue);
        event_queue = nullptr;
    }

    if (display) {
        al_destroy_display(display);
        display = nullptr;
    }
}
