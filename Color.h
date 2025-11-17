#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

class Color {
public:
    float r, g, b, a; //wartosci od 0.0f do 1.0f

    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    
    Color(float red, float green, float blue, float alpha = 1.0f) 
        : r(red), g(green), b(blue), a(alpha) {}
    
    //wartosci 0-255
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red / 255.0f), g(green / 255.0f), b(blue / 255.0f), a(alpha / 255.0f) {}
    //predef rgb
    static Color White() { return Color(1.0f, 1.0f, 1.0f); }
    static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
    static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
    static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
    static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }
    static Color Yellow() { return Color(1.0f, 1.0f, 0.0f); }
    static Color Cyan() { return Color(0.0f, 1.0f, 1.0f); }
    static Color Magenta() { return Color(1.0f, 0.0f, 1.0f); }
    static Color Orange() { return Color(1.0f, 0.65f, 0.0f); }
    static Color Purple() { return Color(0.5f, 0.0f, 0.5f); }
    static Color Gray() { return Color(0.5f, 0.5f, 0.5f); }
    static Color Pink() { return Color(1.0f, 0.75f, 0.8f); }

    Color operator*(float scalar) const {
        return Color(r * scalar, g * scalar, b * scalar, a);
    }

    Color lerp(const Color& other, float t) const {
        return Color(
            r + (other.r - r) * t,
            g + (other.g - g) * t,
            b + (other.b - b) * t,
            a + (other.a - a) * t
        );
    }
    //hex
    uint32_t toHex() const {
        uint8_t ri = static_cast<uint8_t>(r * 255);
        uint8_t gi = static_cast<uint8_t>(g * 255);
        uint8_t bi = static_cast<uint8_t>(b * 255);
        uint8_t ai = static_cast<uint8_t>(a * 255);
        return (ai << 24) | (ri << 16) | (gi << 8) | bi;
    }

    //tworzenie koloru
    static Color fromHex(uint32_t hex) {
        return Color(
            ((hex >> 16) & 0xFF) / 255.0f,
            ((hex >> 8) & 0xFF) / 255.0f,
            (hex & 0xFF) / 255.0f,
            ((hex >> 24) & 0xFF) / 255.0f
        );
    }
};

#endif // COLOR_H
