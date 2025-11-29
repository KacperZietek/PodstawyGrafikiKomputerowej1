#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <fstream>

enum DisplayMode {
    WINDOWED,
    FULLSCREEN
};

class Engine {
private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* buffer; // Bufor posredni

    int screen_width;
    int screen_height;
    DisplayMode display_mode;
    int fps;
    bool running;
    bool mouse_installed;
    bool keyboard_installed;
    bool double_buffering;

    std::ofstream log_file;

    bool initialize_graphics();
    bool initialize_input();
    void cleanup();

public:
    Engine();
    virtual ~Engine();

    bool initialize(int width = 800, int height = 600,
                   DisplayMode mode = WINDOWED, int target_fps = 60,
                   bool use_double_buffering = true);
    void run();
    void stop();

    void begin_frame();
    void end_frame();

    void clear_screen(ALLEGRO_COLOR color);
    void clear_bitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color);

    void log_message(const std::string& message);
    void show_error(const std::string& error);

    // NEW: Fullscreen methods
    void toggle_fullscreen();
    bool set_display_mode(DisplayMode mode);

    // Gettery
    int get_screen_width() const { return screen_width; }
    int get_screen_height() const { return screen_height; }
    DisplayMode get_display_mode() const { return display_mode; }
    bool is_running() const { return running; }
    ALLEGRO_FONT* get_font() const { return font; }
    ALLEGRO_BITMAP* get_buffer() const { return buffer; }

    virtual void update(float delta_time) {}
    virtual void render() {}
    virtual void handle_input() {}
    virtual void on_key_press(int key) {}
    virtual void on_key_release(int key) {}
    virtual void on_mouse_click(int button, int x, int y) {}
    virtual void on_mouse_move(int x, int y) {}
};

#endif
