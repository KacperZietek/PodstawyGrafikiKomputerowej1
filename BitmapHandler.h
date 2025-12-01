#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <map>

class BitmapHandler {
private:
    //przechowywanie bitmap
    std::map<std::string, ALLEGRO_BITMAP*> bitmaps;

public:
    BitmapHandler();
    ~BitmapHandler();

    //wczytuje bitmape pliku
    ALLEGRO_BITMAP* loadBitmap(const std::string& filename);

    //tworzy pusta bitmape
    ALLEGRO_BITMAP* createBitmap(int width, int height);

    //usuwa konkretna bitmape
    void destroyBitmap(const std::string& filename);

    void cleanup();
};

#endif
