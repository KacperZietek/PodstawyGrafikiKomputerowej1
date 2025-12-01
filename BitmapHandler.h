#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <map>

class BitmapHandler {
private:
    // Cache: przechowuje za³adowane bitmapy, ¿eby nie dublowaæ ich w pamiêci
    std::map<std::string, ALLEGRO_BITMAP*> bitmaps;

public:
    BitmapHandler();
    ~BitmapHandler();

    // Wczytuje bitmapê z pliku (lub zwraca ju¿ wczytan¹)
    ALLEGRO_BITMAP* loadBitmap(const std::string& filename);

    // Tworzy pust¹ bitmapê
    ALLEGRO_BITMAP* createBitmap(int width, int height);

    // Usuwa konkretn¹ bitmapê
    void destroyBitmap(const std::string& filename);

    // Czyœci wszystko (przy zamykaniu gry)
    void cleanup();
};

#endif
