#include "BitmapHandler.h"
#include <iostream>

BitmapHandler::BitmapHandler() {}

BitmapHandler::~BitmapHandler() {
    cleanup();
}

ALLEGRO_BITMAP* BitmapHandler::loadBitmap(const std::string& filename) {
    // sprawdzamy bitmape w cache
    auto it = bitmaps.find(filename);
    if (it != bitmaps.end()) {
        return it->second;
    }

    // jesli nie zaladuj z dysku
    ALLEGRO_BITMAP* bmp = al_load_bitmap(filename.c_str());
    if (!bmp) {
        std::cerr << "Blad: Nie udalo sie zaladowac bitmapy: " << filename << std::endl;
        return nullptr;
    }

    // Zapisanie w cache
    bitmaps[filename] = bmp;
    std::cout << "Zaladowano bitmape: " << filename << std::endl;
    return bmp;
}

ALLEGRO_BITMAP* BitmapHandler::createBitmap(int width, int height) {
    return al_create_bitmap(width, height);
}

void BitmapHandler::destroyBitmap(const std::string& filename) {
    auto it = bitmaps.find(filename);
    if (it != bitmaps.end()) {
        al_destroy_bitmap(it->second);
        bitmaps.erase(it);
    }
}

void BitmapHandler::cleanup() {
    for (auto& pair : bitmaps) {
        al_destroy_bitmap(pair.second);
    }
    bitmaps.clear();
}
