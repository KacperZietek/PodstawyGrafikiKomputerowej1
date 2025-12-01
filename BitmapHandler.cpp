#include "BitmapHandler.h"
#include <iostream>

BitmapHandler::BitmapHandler() {}

BitmapHandler::~BitmapHandler() {
    cleanup();
}

ALLEGRO_BITMAP* BitmapHandler::loadBitmap(const std::string& filename) {
    // 1. SprawdŸ, czy ju¿ mamy tê bitmapê w cache
    auto it = bitmaps.find(filename);
    if (it != bitmaps.end()) {
        return it->second;
    }

    // 2. Jeœli nie, spróbuj za³adowaæ z dysku
    ALLEGRO_BITMAP* bmp = al_load_bitmap(filename.c_str());
    if (!bmp) {
        std::cerr << "B³¹d: Nie uda³o siê za³adowaæ bitmapy: " << filename << std::endl;
        return nullptr;
    }

    // 3. Zapisz w cache i zwróæ
    bitmaps[filename] = bmp;
    std::cout << "Za³adowano bitmapê: " << filename << std::endl;
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
