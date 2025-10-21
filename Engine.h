#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_primitives.h>
#include<string>
#include<fstream>

enum DisplayMode{
    WINDOWED,
    FULLSCREEN,
    FULLSCREEN_WINDOWED
};

class Engine{
private:

    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_FONT* font;

    int screen_width;
    int sreen_height;
    DisplayMode display_mode;
    int fps;
    bool running;
    bool mouse_installed;
    bool keyboard_installed;

    std::ofstream lof_file;

    bool initialize_graphics();
    bool initialize_input();
    void cleanup();

public:
    Engine();
    ~Engine();

    bool initialize(int width = 800, int height=600,
                     DisplayMode mode=WINDOWED, int target_fps = 60);
    void run();
    void stop();

    void clear_screen(ALLEGRO_COLOR color);
    void clear_bitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color);

    void log_message(const std::string& message);
    void show_error(const std::string& error);

    int get_screen_width() const {return screen_width;}
    int get_screen_height() const {return screen_height;}
    virtual void update(float delta_time){}
    virtual void render(){}
    virtual void handle_input(){}
    virtual void on_key_press(int key){}
    virtual void on_key_release(int key){}
    virtual void on_mouse_click(int button, int x, int y){}
    virtual void on_mouse_move(int x, int y){}

};

#endif // ENGINE_H_INCLUDED
