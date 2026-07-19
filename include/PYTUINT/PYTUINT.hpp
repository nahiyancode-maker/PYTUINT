#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <regex>
#include <optional>
#include <variant>
#include <any>
#include <type_traits>
#include <numeric>
#include <random>
#include <cmath>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#endif

namespace py = pybind11;

namespace pytuint {

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 1: COLOR SYSTEM
// ═══════════════════════════════════════════════════════════════════════════════

struct Color {
    int r, g, b;
    float a = 1.0f;
    
    Color() : r(255), g(255), b(255), a(1.0f) {}
    Color(int r, int g, int b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    std::string fg() const {
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    
    std::string bg() const {
        return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    
    Color blend(const Color& other, float t) const {
        return Color(
            (int)(r * (1-t) + other.r * t),
            (int)(g * (1-t) + other.g * t),
            (int)(b * (1-t) + other.b * t),
            a * (1-t) + other.a * t
        );
    }
    
    Color darken(float factor) const {
        return Color((int)(r * factor), (int)(g * factor), (int)(b * factor), a);
    }
    
    Color lighten(float factor) const {
        return Color(
            std::min(255, (int)(r + (255 - r) * factor)),
            std::min(255, (int)(g + (255 - g) * factor)),
            std::min(255, (int)(b + (255 - b) * factor)),
            a
        );
    }
    
    static Color from_hex(const std::string& hex) {
        if (hex.empty() || hex[0] != '#') return Color::WHITE;
        int r, g, b;
        if (hex.length() == 7) {
            sscanf(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
        } else if (hex.length() == 4) {
            r = (hex[1] - '0') * 17;
            g = (hex[2] - '0') * 17;
            b = (hex[3] - '0') * 17;
        } else {
            return Color::WHITE;
        }
        return Color(r, g, b);
    }
    
    static Color from_hsl(float h, float s, float l) {
        float c = (1 - std::abs(2 * l - 1)) * s;
        float x = c * (1 - std::abs(fmod(h / 60.0f, 2) - 1));
        float m = l - c / 2;
        float r, g, b;
        if (h < 60) { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }
        return Color((int)((r + m) * 255), (int)((g + m) * 255), (int)((b + m) * 255));
    }
    
    static const Color WHITE;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color CYAN;
    static const Color MAGENTA;
    static const Color GRAY;
    static const Color DARK_GRAY;
    static const Color ORANGE;
    static const Color PURPLE;
    static const Color PINK;
    static const Color BROWN;
    static const Color LIME;
    static const Color NAVY;
    static const Color TEAL;
    static const Color OLIVE;
    static const Color MAROON;
    static const Color AQUA;
    static const Color SILVER;
    static const Color GOLD;
    static const Color CORAL;
    static const Color SALMON;
    static const Color KHAKI;
    static const Color PLUM;
    static const Color INDIGO;
    static const Color TURQUOISE;
    static const Color VIOLET;
    static const Color BEIGE;
    static const Color IVORY;
    static const Color LINEN;
    static const Color AZURE;
    static const Color LAVENDER;
    static const Color MINT;
    static const Color PEACH;
    static const Color APRICOT;
    static const Color MAUVE;
    static const Color LILAC;
    static const Color TAN;
    static const Color RUST;
    static const Color MUSTARD;
    static const Color BURGUNDY;
    static const Color CRIMSON;
    static const Color SCARLET;
    static const Color CERULEAN;
    static const Color COBALT;
    static const Color SAPPHIRE;
    static const Color AMBER;
    static const Color BRONZE;
    static const Color COPPER;
    static const Color CHARCOAL;
    static const Color SLATE;
    static const Color MIDNIGHT;
    static const Color EBONY;
    static const Color JET;
    static const Color ONYX;
    static const Color PEARL;
    static const Color OPAL;
    static const Color JADE;
    static const Color EMERALD;
    static const Color MALACHITE;
    static const Color AQUAMARINE;
    static const Color TOPAZ;
    static const Color GARNET;
    static const Color RUBY;
    static const Color AMETHYST;
    static const Color CITRINE;
    static const Color PERIDOT;
    static const Color TANZANITE;
    static const Color MOONSTONE;
    static const Color SUNSTONE;
    static const Color LABRADORITE;
    static const Color OBSIDIAN;
    static const Color TOURMALINE;
    static const Color SPINEL;
    static const Color ZIRCON;
    static const Color ALEXANDRITE;
    static const Color CHRYSOBERYL;
    static const Color CATSEYE;
    static const Color TIGERSEYE;
    static const Color CHALCEDONY;
    static const Color AGATE;
    static const Color JASPER;
    static const Color ONYXITE;
    static const Color MARBLE;
    static const Color GRANITE;
    static const Color QUARTZ;
    static const Color BASALT;
    static const Color OBSIDIA;
    static const Color PUMICE;
    static const Color SCORIA;
    static const Color TUFF;
    static const Color RHYOLITE;
    static const Color ANDESITE;
    static const Color DACITE;
    static const Color TRACHYTE;
    static const Color PHONOLITE;
    static const Color LIMESTONE;
    static const Color SANDSTONE;
    static const Color SHALE;
    static const Color MUDSTONE;
    static const Color SILTSTONE;
    static const Color CONGLOMERATE;
    static const Color BRECCIA;
    static const Color CHALK;
    static const Color FLINT;
    static const Color CHERT;
    static const Color JASPERITE;
    static const Color HEMATITE;
    static const Color MAGNETITE;
    static const Color PYRITE;
    static const Color GALENA;
    static const Color SPHALERITE;
    static const Color CHALCOPYRITE;
    static const Color BORNITE;
    static const Color COVELLITE;
    static const Color CHALCOCITE;
    static const Color TENNANTITE;
    static const Color TETRAHEDRITE;
    static const Color ENARGITE;
    static const Color ARGENTITE;
    static const Color PYRARGYRITE;
    static const Color PROUSTITE;
    static const Color STROMEYERITE;
    static const Color ARSENOPYRITE;
    static const Color LOELLINGITE;
    static const Color GLAUCODOT;
    static const Color SAFFLORITE;
    static const Color GEOCRONITE;
    static const Color CLINOBISVANITE;
    static const Color SCHULZITE;
    static const Color TRIPHYLITE;
    static const Color LITHIOPHILITE;
    static const Color PHOSPHOPHYLLITE;
    static const Color PURPURITE;
    static const Color HETEROSITE;
    static const Color FERRISICKLERITE;
    static const Color SICKLERITE;
    static const Color VARULITE;
    static const Color ARROJADITE;
    static const Color ALLUAUDITE;
    static const Color HUREAULITE;
    static const Color BRANDTITE;
    static const Color KOLBECKITE;
    static const Color STRENGITE;
    static const Color PHOSPHOSIDERITE;
    static const Color SCORODITE;
    static const Color MANDELNITE;
    static const Color PARASCORODITE;
    static const Color YELLOW_ARSENIC;
    static const Color REALGAR;
    static const Color ORPIMENT;
    static const Color STIBNITE;
    static const Color BISMUTHINITE;
    static const Color GUANAJUATITE;
    static const Color FAMATINITE;
    static const Color GEARKSUTITE;
    static const Color CHALCOMENITE;
    static const Color TENORITE;
    static const Color MELACONITE;
    static const Color CUPRITE;
    static const Color TENORIT;
    static const Color DELAFOSSITE;
    static const Color PSEUDOMALACHITE;
    static const Color CHRYSOCOLLA;
    static const Color DIOPTASITE;
    static const Color SPANGOLITE;
    static const Color AURICHALCITE;
    static const Color HYDROCCHRYSOCOLLA;
    static const Color LIROCONITE;
    static const Color POSNJAKITE;
    static const Color KIPUSHITE;
    static const Color GERHARDITE;
    static const Color ZINCOCOPPERITE;
    static const Color ROSASITE;
    static const Color AURICHALCIUM;
    static const Color HYDROCHRYSOCOLLA;
    static const Color PSEUDOMALACHIUM;
    static const Color CHRYSOCOLLUM;
    static const Color DIOPTASIUM;
    static const Color SPANGOLIUM;
    static const Color AURICHALCUM;
    static const Color LIROCONUM;
    static const Color POSNJAKUM;
    static const Color KIPUSHUM;
    static const Color GERHARDUM;
    static const Color ZINCOCOPPERUM;
    static const Color ROSASUM;
    static const Color AURICHALCUS;
    static const Color HYDROCHRYSOCOLLUS;
    static const Color PSEUDOMALACHUS;
    static const Color CHRYSOCOLLUS;
    static const Color DIOPTASUS;
    static const Color SPANGOLUS;
    static const Color AURICHALCUS2;
    static const Color LIROCONUS;
    static const Color POSNJAKUS;
    static const Color KIPUSHUS;
    static const Color GERHARDUS;
    static const Color ZINCOCOPPERUS;
    static const Color ROSASUS;
};

const Color Color::WHITE(255, 255, 255);
const Color Color::BLACK(0, 0, 0);
const Color Color::RED(255, 0, 0);
const Color Color::GREEN(0, 255, 0);
const Color Color::BLUE(0, 0, 255);
const Color Color::YELLOW(255, 255, 0);
const Color Color::CYAN(0, 255, 255);
const Color Color::MAGENTA(255, 0, 255);
const Color Color::GRAY(128, 128, 128);
const Color Color::DARK_GRAY(64, 64, 64);
const Color Color::ORANGE(255, 165, 0);
const Color Color::PURPLE(128, 0, 128);
const Color Color::PINK(255, 192, 203);
const Color Color::BROWN(139, 69, 19);
const Color Color::LIME(50, 205, 50);
const Color Color::NAVY(0, 0, 128);
const Color Color::TEAL(0, 128, 128);
const Color Color::OLIVE(128, 128, 0);
const Color Color::MAROON(128, 0, 0);
const Color Color::AQUA(0, 255, 255);
const Color Color::SILVER(192, 192, 192);
const Color Color::GOLD(255, 215, 0);
const Color Color::CORAL(255, 127, 80);
const Color Color::SALMON(250, 128, 114);
const Color Color::KHAKI(240, 230, 140);
const Color Color::PLUM(221, 160, 221);
const Color Color::INDIGO(75, 0, 130);
const Color Color::TURQUOISE(64, 224, 208);
const Color Color::VIOLET(238, 130, 238);
const Color Color::BEIGE(245, 245, 220);
const Color Color::IVORY(255, 255, 240);
const Color Color::LINEN(250, 240, 230);
const Color Color::AZURE(240, 255, 255);
const Color Color::LAVENDER(230, 230, 250);
const Color Color::MINT(189, 252, 201);
const Color Color::PEACH(255, 218, 185);
const Color Color::APRICOT(251, 206, 177);
const Color Color::MAUVE(224, 176, 255);
const Color Color::LILAC(200, 162, 200);
const Color Color::TAN(210, 180, 140);
const Color Color::RUST(183, 65, 14);
const Color Color::MUSTARD(255, 219, 88);
const Color Color::BURGUNDY(128, 0, 32);
const Color Color::CRIMSON(220, 20, 60);
const Color Color::SCARLET(255, 36, 0);
const Color Color::CERULEAN(0, 123, 167);
const Color Color::COBALT(0, 71, 171);
const Color Color::SAPPHIRE(15, 82, 186);
const Color Color::AMBER(255, 191, 0);
const Color Color::BRONZE(205, 127, 50);
const Color Color::COPPER(184, 115, 51);
const Color Color::CHARCOAL(54, 69, 79);
const Color Color::SLATE(112, 128, 144);
const Color Color::MIDNIGHT(25, 25, 112);
const Color Color::EBONY(85, 93, 80);
const Color Color::JET(52, 52, 52);
const Color Color::ONYX(53, 56, 57);
const Color Color::PEARL(234, 224, 200);
const Color Color::OPAL(168, 195, 188);
const Color Color::JADE(0, 168, 107);
const Color Color::EMERALD(80, 200, 120);
const Color Color::MALACHITE(11, 218, 81);
const Color Color::AQUAMARINE(127, 255, 212);
const Color Color::TOPAZ(255, 200, 124);
const Color Color::GARNET(115, 28, 28);
const Color Color::RUBY(224, 17, 95);
const Color Color::AMETHYST(153, 102, 204);
const Color Color::CITRINE(228, 208, 10);
const Color Color::PERIDOT(175, 211, 60);
const Color Color::TANZANITE(75, 0, 130);
const Color Color::MOONSTONE(58, 168, 193);
const Color Color::SUNSTONE(201, 148, 118);
const Color Color::LABRADORITE(110, 124, 139);
const Color Color::OBSIDIAN(55, 55, 55);
const Color Color::TOURMALINE(134, 197, 216);
const Color Color::SPINEL(255, 80, 80);
const Color Color::ZIRCON(0, 128, 128);
const Color Color::ALEXANDRITE(99, 154, 170);
const Color Color::CHRYSOBERYL(156, 224, 140);
const Color Color::CATSEYE(175, 211, 60);
const Color Color::TIGERSEYE(224, 178, 82);
const Color Color::CHALCEDONY(184, 184, 184);
const Color Color::AGATE(178, 178, 178);
const Color Color::JASPER(215, 58, 64);
const Color Color::ONYXITE(53, 56, 57);
const Color Color::MARBLE(252, 252, 252);
const Color Color::GRANITE(102, 102, 102);
const Color Color::QUARTZ(217, 217, 217);
const Color Color::BASALT(72, 72, 72);
const Color Color::OBSIDIA(55, 55, 55);
const Color Color::PUMICE(190, 190, 190);
const Color Color::SCORIA(139, 69, 19);
const Color Color::TUFF(204, 204, 204);
const Color Color::RHYOLITE(204, 204, 204);
const Color Color::ANDESITE(155, 155, 155);
const Color Color::DACITE(204, 204, 204);
const Color Color::TRACHYTE(204, 204, 204);
const Color Color::PHONOLITE(204, 204, 204);
const Color Color::LIMESTONE(220, 220, 220);
const Color Color::SANDSTONE(194, 178, 128);
const Color Color::SHALE(128, 128, 128);
const Color Color::MUDSTONE(128, 128, 128);
const Color Color::SILTSTONE(194, 178, 128);
const Color Color::CONGLOMERATE(194, 178, 128);
const Color Color::BRECCIA(194, 178, 128);
const Color Color::CHALK(255, 255, 255);
const Color Color::FLINT(111, 109, 106);
const Color Color::CHERT(111, 109, 106);
const Color Color::JASPERITE(215, 58, 64);
const Color Color::HEMATITE(132, 56, 56);
const Color Color::MAGNETITE(64, 64, 64);
const Color Color::PYRITE(240, 216, 120);
const Color Color::GALENA(108, 108, 108);
const Color Color::SPHALERITE(108, 108, 108);
const Color Color::CHALCOPYRITE(191, 144, 64);
const Color Color::BORNITE(152, 102, 51);
const Color Color::COVELLITE(56, 84, 144);
const Color Color::CHALCOCITE(56, 56, 56);
const Color Color::TENNANTITE(108, 108, 108);
const Color Color::TETRAHEDRITE(108, 108, 108);
const Color Color::ENARGITE(108, 108, 108);
const Color Color::ARGENTITE(108, 108, 108);
const Color Color::PYRARGYRITE(108, 108, 108);
const Color Color::PROUSTITE(108, 108, 108);
const Color Color::STROMEYERITE(152, 102, 51);
const Color Color::ARSENOPYRITE(152, 102, 51);
const Color Color::LOELLINGITE(152, 102, 51);
const Color Color::GLAUCODOT(152, 102, 51);
const Color Color::SAFFLORITE(152, 102, 51);
const Color Color::GEOCRONITE(152, 102, 51);
const Color Color::CLINOBISVANITE(152, 102, 51);
const Color Color::SCHULZITE(152, 102, 51);
const Color Color::TRIPHYLITE(152, 102, 51);
const Color Color::LITHIOPHILITE(152, 102, 51);
const Color Color::PHOSPHOPHYLLITE(152, 102, 51);
const Color Color::PURPURITE(152, 102, 51);
const Color Color::HETEROSITE(152, 102, 51);
const Color Color::FERRISICKLERITE(152, 102, 51);
const Color Color::SICKLERITE(152, 102, 51);
const Color Color::VARULITE(152, 102, 51);
const Color Color::ARROJADITE(152, 102, 51);
const Color Color::ALLUAUDITE(152, 102, 51);
const Color Color::HUREAULITE(152, 102, 51);
const Color Color::BRANDTITE(152, 102, 51);
const Color Color::KOLBECKITE(152, 102, 51);
const Color Color::STRENGITE(152, 102, 51);
const Color Color::PHOSPHOSIDERITE(152, 102, 51);
const Color Color::SCORODITE(152, 102, 51);
const Color Color::MANDELNITE(152, 102, 51);
const Color Color::PARASCORODITE(152, 102, 51);
const Color Color::YELLOW_ARSENIC(255, 255, 0);
const Color Color::REALGAR(255, 102, 0);
const Color Color::ORPIMENT(255, 204, 0);
const Color Color::STIBNITE(68, 68, 68);
const Color Color::BISMUTHINITE(68, 68, 68);
const Color Color::GUANAJUATITE(68, 68, 68);
const Color Color::FAMATINITE(68, 68, 68);
const Color Color::GEARKSUTITE(68, 68, 68);
const Color Color::CHALCOMENITE(68, 68, 68);
const Color Color::TENORITE(68, 68, 68);
const Color Color::MELACONITE(68, 68, 68);
const Color Color::CUPRITE(184, 115, 51);
const Color Color::TENORIT(184, 115, 51);
const Color Color::DELAFOSSITE(184, 115, 51);
const Color Color::PSEUDOMALACHITE(184, 115, 51);
const Color Color::CHRYSOCOLLA(184, 115, 51);
const Color Color::DIOPTASITE(184, 115, 51);
const Color Color::SPANGOLITE(184, 115, 51);
const Color Color::AURICHALCITE(184, 115, 51);
const Color Color::HYDROCHRYSOCOLLA(184, 115, 51);
const Color Color::LIROCONITE(184, 115, 51);
const Color Color::POSNJAKITE(184, 115, 51);
const Color Color::KIPUSHITE(184, 115, 51);
const Color Color::GERHARDITE(184, 115, 51);
const Color Color::ZINCOCOPPERITE(184, 115, 51);
const Color Color::ROSASITE(184, 115, 51);
const Color Color::AURICHALCIUM(184, 115, 51);
const Color Color::HYDROCHRYSOCOLLUM(184, 115, 51);
const Color Color::PSEUDOMALACHIUM(184, 115, 51);
const Color Color::CHRYSOCOLLUM(184, 115, 51);
const Color Color::DIOPTASIUM(184, 115, 51);
const Color Color::SPANGOLIUM(184, 115, 51);
const Color Color::AURICHALCUM(184, 115, 51);
const Color Color::LIROCONUM(184, 115, 51);
const Color Color::POSNJAKUM(184, 115, 51);
const Color Color::KIPUSHUM(184, 115, 51);
const Color Color::GERHARDUM(184, 115, 51);
const Color Color::ZINCOCOPPERUM(184, 115, 51);
const Color Color::ROSASUM(184, 115, 51);
const Color Color::AURICHALCUS(184, 115, 51);
const Color Color::HYDROCHRYSOCOLLUS(184, 115, 51);
const Color Color::PSEUDOMALACHUS(184, 115, 51);
const Color Color::CHRYSOCOLLUS(184, 115, 51);
const Color Color::DIOPTASUS(184, 115, 51);
const Color Color::SPANGOLUS(184, 115, 51);
const Color Color::AURICHALCUS2(184, 115, 51);
const Color Color::LIROCONUS(184, 115, 51);
const Color Color::POSNJAKUS(184, 115, 51);
const Color Color::KIPUSHUS(184, 115, 51);
const Color Color::GERHARDUS(184, 115, 51);
const Color Color::ZINCOCOPPERUS(184, 115, 51);
const Color Color::ROSASUS(184, 115, 51);

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 2: STYLE SYSTEM
// ═══════════════════════════════════════════════════════════════════════════════

struct BorderStyle {
    enum Type { NONE, SINGLE, DOUBLE, ROUNDED, THICK, DASHED, DOTTED, ASCII, BLOCK, HEAVY };
    Type type = SINGLE;
    Color color = Color::WHITE;
    
    std::string top_left() const {
        switch(type) {
            case SINGLE: return "┌";
            case DOUBLE: return "╔";
            case ROUNDED: return "╭";
            case THICK: return "┏";
            case DASHED: return "┌";
            case DOTTED: return "·";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▛";
            default: return " ";
        }
    }
    
    std::string top_right() const {
        switch(type) {
            case SINGLE: return "┐";
            case DOUBLE: return "╗";
            case ROUNDED: return "╮";
            case THICK: return "┓";
            case DASHED: return "┐";
            case DOTTED: return "·";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▜";
            default: return " ";
        }
    }
    
    std::string bottom_left() const {
        switch(type) {
            case SINGLE: return "└";
            case DOUBLE: return "╚";
            case ROUNDED: return "╰";
            case THICK: return "┗";
            case DASHED: return "└";
            case DOTTED: return "·";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▙";
            default: return " ";
        }
    }
    
    std::string bottom_right() const {
        switch(type) {
            case SINGLE: return "┘";
            case DOUBLE: return "╝";
            case ROUNDED: return "╯";
            case THICK: return "┛";
            case DASHED: return "┘";
            case DOTTED: return "·";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▟";
            default: return " ";
        }
    }
    
    std::string horizontal() const {
        switch(type) {
            case SINGLE: return "─";
            case DOUBLE: return "═";
            case ROUNDED: return "─";
            case THICK: return "━";
            case DASHED: return "╌";
            case DOTTED: return "┈";
            case ASCII: return "-";
            case BLOCK: return "█";
            case HEAVY: return "▀";
            default: return " ";
        }
    }
    
    std::string vertical() const {
        switch(type) {
            case SINGLE: return "│";
            case DOUBLE: return "║";
            case ROUNDED: return "│";
            case THICK: return "┃";
            case DASHED: return "╎";
            case DOTTED: return "┊";
            case ASCII: return "|";
            case BLOCK: return "█";
            case HEAVY: return "▌";
            default: return " ";
        }
    }
    
    std::string cross() const {
        switch(type) {
            case SINGLE: return "┼";
            case DOUBLE: return "╬";
            case ROUNDED: return "┼";
            case THICK: return "╋";
            case DASHED: return "┼";
            case DOTTED: return "┼";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▚";
            default: return " ";
        }
    }
    
    std::string t_down() const {
        switch(type) {
            case SINGLE: return "┬";
            case DOUBLE: return "╦";
            case ROUNDED: return "┬";
            case THICK: return "┳";
            case DASHED: return "┬";
            case DOTTED: return "┬";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▜";
            default: return " ";
        }
    }
    
    std::string t_up() const {
        switch(type) {
            case SINGLE: return "┴";
            case DOUBLE: return "╩";
            case ROUNDED: return "┴";
            case THICK: return "┻";
            case DASHED: return "┴";
            case DOTTED: return "┴";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▙";
            default: return " ";
        }
    }
    
    std::string t_right() const {
        switch(type) {
            case SINGLE: return "├";
            case DOUBLE: return "╠";
            case ROUNDED: return "├";
            case THICK: return "┣";
            case DASHED: return "├";
            case DOTTED: return "├";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▌";
            default: return " ";
        }
    }
    
    std::string t_left() const {
        switch(type) {
            case SINGLE: return "┤";
            case DOUBLE: return "╣";
            case ROUNDED: return "┤";
            case THICK: return "┫";
            case DASHED: return "┤";
            case DOTTED: return "┤";
            case ASCII: return "+";
            case BLOCK: return "█";
            case HEAVY: return "▐";
            default: return " ";
        }
    }
};

struct Style {
    Color fg_color = Color::WHITE;
    Color bg_color = Color::BLACK;
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool strikethrough = false;
    bool dim = false;
    bool blink = false;
    bool reverse = false;
    bool hidden = false;
    int padding_top = 0;
    int padding_bottom = 0;
    int padding_left = 0;
    int padding_right = 0;
    int margin_top = 0;
    int margin_bottom = 0;
    int margin_left = 0;
    int margin_right = 0;
    int width = -1;
    int height = -1;
    int min_width = -1;
    int min_height = -1;
    int max_width = -1;
    int max_height = -1;
    BorderStyle border;
    std::string text_align = "left";
    std::string vertical_align = "top";
    bool overflow = false;
    bool word_wrap = true;
    float opacity = 1.0f;
    int z_index = 0;
    
    Style& set_fg(const Color& c) { fg_color = c; return *this; }
    Style& set_bg(const Color& c) { bg_color = c; return *this; }
    Style& set_bold(bool b) { bold = b; return *this; }
    Style& set_italic(bool b) { italic = b; return *this; }
    Style& set_underline(bool b) { underline = b; return *this; }
    Style& set_strikethrough(bool b) { strikethrough = b; return *this; }
    Style& set_dim(bool b) { dim = b; return *this; }
    Style& set_blink(bool b) { blink = b; return *this; }
    Style& set_reverse(bool b) { reverse = b; return *this; }
    Style& set_hidden(bool b) { hidden = b; return *this; }
    Style& set_padding(int p) { padding_top = padding_bottom = padding_left = padding_right = p; return *this; }
    Style& set_padding(int v, int h) { padding_top = padding_bottom = v; padding_left = padding_right = h; return *this; }
    Style& set_padding(int t, int r, int b, int l) { padding_top = t; padding_right = r; padding_bottom = b; padding_left = l; return *this; }
    Style& set_padding_top(int p) { padding_top = p; return *this; }
    Style& set_padding_bottom(int p) { padding_bottom = p; return *this; }
    Style& set_padding_left(int p) { padding_left = p; return *this; }
    Style& set_padding_right(int p) { padding_right = p; return *this; }
    Style& set_margin(int m) { margin_top = margin_bottom = margin_left = margin_right = m; return *this; }
    Style& set_margin(int v, int h) { margin_top = margin_bottom = v; margin_left = margin_right = h; return *this; }
    Style& set_margin(int t, int r, int b, int l) { margin_top = t; margin_right = r; margin_bottom = b; margin_left = l; return *this; }
    Style& set_margin_top(int m) { margin_top = m; return *this; }
    Style& set_margin_bottom(int m) { margin_bottom = m; return *this; }
    Style& set_margin_left(int m) { margin_left = m; return *this; }
    Style& set_margin_right(int m) { margin_right = m; return *this; }
    Style& set_width(int w) { width = w; return *this; }
    Style& set_height(int h) { height = h; return *this; }
    Style& set_min_width(int w) { min_width = w; return *this; }
    Style& set_min_height(int h) { min_height = h; return *this; }
    Style& set_max_width(int w) { max_width = w; return *this; }
    Style& set_max_height(int h) { max_height = h; return *this; }
    Style& set_border(const BorderStyle& b) { border = b; return *this; }
    Style& set_text_align(const std::string& a) { text_align = a; return *this; }
    Style& set_vertical_align(const std::string& a) { vertical_align = a; return *this; }
    Style& set_overflow(bool o) { overflow = o; return *this; }
    Style& set_word_wrap(bool w) { word_wrap = w; return *this; }
    Style& set_opacity(float o) { opacity = o; return *this; }
    Style& set_z_index(int z) { z_index = z; return *this; }
    
    int total_padding_h() const { return padding_left + padding_right; }
    int total_padding_v() const { return padding_top + padding_bottom; }
    int total_margin_h() const { return margin_left + margin_right; }
    int total_margin_v() const { return margin_top + margin_bottom; }
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 3: TERMINAL MANAGER
// ═══════════════════════════════════════════════════════════════════════════════

class Terminal {
public:
    static Terminal& instance() {
        static Terminal t;
        return t;
    }
    
    void init() {
        if (initialized) return;
        
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwMode |= ENABLE_PROCESSED_OUTPUT;
        dwMode |= ENABLE_WRAP_AT_EOL_OUTPUT;
        SetConsoleMode(hOut, dwMode);
        
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        DWORD dwInMode = 0;
        GetConsoleMode(hIn, &dwInMode);
        dwInMode &= ~ENABLE_LINE_INPUT;
        dwInMode &= ~ENABLE_ECHO_INPUT;
        dwInMode |= ENABLE_WINDOW_INPUT;
        dwInMode |= ENABLE_MOUSE_INPUT;
        SetConsoleMode(hIn, dwInMode);
#else
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
        std::cout << "\033[?1049h";
        std::cout << "\033[?25l";
        std::cout << "\033[2J";
        std::cout << "\033[H";
        std::cout << "\033[?1002h";
        std::cout << "\033[?1006h";
        std::cout << "\033[?1003h";
        std::cout << "\033[?1015h";
        std::cout << "\033[?1004h";
        initialized = true;
    }
    
    void cleanup() {
        if (!initialized) return;
        std::cout << "\033[?25h";
        std::cout << "\033[?1049l";
        std::cout << "\033[0m";
        std::cout << "\033[?1002l";
        std::cout << "\033[?1006l";
        std::cout << "\033[?1003l";
        std::cout << "\033[?1015l";
        std::cout << "\033[?1004l";
        std::cout.flush();
        
#ifndef _WIN32
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);
        raw.c_lflag |= (ECHO | ICANON | IEXTEN | ISIG);
        raw.c_iflag |= (BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag |= (OPOST);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
        initialized = false;
    }
    
    void write(const std::string& s) {
        buffer += s;
    }
    
    void flush() {
        if (!buffer.empty()) {
            std::cout << buffer << std::flush;
            buffer.clear();
        }
    }
    
    void clear() {
        buffer += "\033[2J\033[H";
    }
    
    void moveTo(int row, int col) {
        buffer += "\033[" + std::to_string(row + 1) + ";" + std::to_string(col + 1) + "H";
    }
    
    void set_color_fg(const Color& c) {
        buffer += c.fg();
    }
    
    void set_color_bg(const Color& c) {
        buffer += c.bg();
    }
    
    void reset_color() {
        buffer += "\033[0m";
    }
    
    void set_bold(bool on) {
        buffer += on ? "\033[1m" : "\033[22m";
    }
    
    void set_italic(bool on) {
        buffer += on ? "\033[3m" : "\033[23m";
    }
    
    void set_underline(bool on) {
        buffer += on ? "\033[4m" : "\033[24m";
    }
    
    void set_strikethrough(bool on) {
        buffer += on ? "\033[9m" : "\033[29m";
    }
    
    void set_dim(bool on) {
        buffer += on ? "\033[2m" : "\033[22m";
    }
    
    void set_blink(bool on) {
        buffer += on ? "\033[5m" : "\033[25m";
    }
    
    void set_reverse(bool on) {
        buffer += on ? "\033[7m" : "\033[27m";
    }
    
    void set_hidden(bool on) {
        buffer += on ? "\033[8m" : "\033[28m";
    }
    
    void hide_cursor() {
        buffer += "\033[?25l";
    }
    
    void show_cursor() {
        buffer += "\033[?25h";
    }
    
    int width() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
#endif
    }
    
    int height() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_row;
#endif
    }
    
    int get_key() {
#ifdef _WIN32
        if (_kbhit()) {
            return _getch();
        }
        return -1;
#else
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            return (unsigned char)c;
        }
        return -1;
#endif
    }
    
    bool has_input() {
#ifdef _WIN32
        return _kbhit();
#else
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        struct timeval tv = {0, 0};
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
    }
    
private:
    Terminal() : initialized(false) {}
    ~Terminal() { cleanup(); }
    
    bool initialized;
    std::string buffer;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 4: EVENT SYSTEM
// ═══════════════════════════════════════════════════════════════════════════════

enum class EventType {
    NONE,
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_CLICK,
    MOUSE_RELEASE,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    RESIZE,
    FOCUS,
    BLUR,
    TEXT_INPUT,
    QUIT
};

struct Event {
    EventType type = EventType::NONE;
    int key = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    int mouse_button = 0;
    int scroll_delta = 0;
    char text[32] = {0};
    int width = 0;
    int height = 0;
    
    bool is_key(int k) const { return type == EventType::KEY_PRESS && key == k; }
    bool is_mouse_click(int btn = 0) const { return type == EventType::MOUSE_CLICK && (btn == 0 || mouse_button == btn); }
    bool is_mouse_move() const { return type == EventType::MOUSE_MOVE; }
    bool is_mouse_scroll() const { return type == EventType::MOUSE_SCROLL; }
    bool is_resize() const { return type == EventType::RESIZE; }
    bool is_quit() const { return type == EventType::QUIT; }
    bool is_focus() const { return type == EventType::FOCUS; }
    bool is_blur() const { return type == EventType::BLUR; }
    bool is_text_input() const { return type == EventType::TEXT_INPUT; }
};

class EventQueue {
public:
    void push(const Event& e) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(e);
    }
    
    bool pop(Event& e) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        e = queue_.front();
        queue_.pop();
        return true;
    }
    
    bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
    
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!queue_.empty()) queue_.pop();
    }
    
private:
    std::queue<Event> queue_;
    std::mutex mutex_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 5: ANIMATION SYSTEM
// ═══════════════════════════════════════════════════════════════════════════════

enum class Easing {
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT,
    BOUNCE,
    ELASTIC,
    BACK_IN,
    BACK_OUT,
    BACK_IN_OUT,
    CIRC_IN,
    CIRC_OUT,
    CIRC_IN_OUT,
    EXPO_IN,
    EXPO_OUT,
    EXPO_IN_OUT,
    QUAD_IN,
    QUAD_OUT,
    QUAD_IN_OUT,
    CUBIC_IN,
    CUBIC_OUT,
    CUBIC_IN_OUT,
    QUART_IN,
    QUART_OUT,
    QUART_IN_OUT,
    QUINT_IN,
    QUINT_OUT,
    QUINT_IN_OUT,
    SINE_IN,
    SINE_OUT,
    SINE_IN_OUT
};

class Animation {
public:
    Animation(float duration = 1.0f, Easing easing = Easing::LINEAR)
        : duration_(duration), easing_(easing), progress_(0.0f), running_(false) {}
    
    void start() {
        running_ = true;
        start_time_ = std::chrono::steady_clock::now();
        progress_ = 0.0f;
    }
    
    void stop() {
        running_ = false;
        progress_ = 0.0f;
    }
    
    void reset() {
        running_ = false;
        progress_ = 0.0f;
    }
    
    bool update() {
        if (!running_) return false;
        
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - start_time_).count();
        progress_ = std::min(1.0f, elapsed / duration_);
        
        if (progress_ >= 1.0f) {
            running_ = false;
            return true;
        }
        return false;
    }
    
    float get_progress() const { return apply_easing(progress_); }
    float get_raw_progress() const { return progress_; }
    bool is_running() const { return running_; }
    bool is_finished() const { return !running_ && progress_ >= 1.0f; }
    
    void set_duration(float d) { duration_ = d; }
    void set_easing(Easing e) { easing_ = e; }
    
private:
    float apply_easing(float t) const {
        switch (easing_) {
            case Easing::LINEAR: return t;
            case Easing::EASE_IN: return t * t;
            case Easing::EASE_OUT: return 1 - (1 - t) * (1 - t);
            case Easing::EASE_IN_OUT: return t < 0.5 ? 2 * t * t : 1 - (-2 * t + 2) * (-2 * t + 2) / 2;
            case Easing::BOUNCE: return bounce_ease_out(t);
            case Easing::ELASTIC: return elastic_ease_out(t);
            case Easing::BACK_IN: return back_ease_in(t);
            case Easing::BACK_OUT: return back_ease_out(t);
            case Easing::BACK_IN_OUT: return back_ease_in_out(t);
            case Easing::CIRC_IN: return 1 - std::sqrt(1 - t * t);
            case Easing::CIRC_OUT: return std::sqrt(1 - (t - 1) * (t - 1));
            case Easing::CIRC_IN_OUT: return t < 0.5 ? (1 - std::sqrt(1 - 4 * t * t)) / 2 : (std::sqrt(1 - (-2 * t + 2) * (-2 * t + 2)) + 1) / 2;
            case Easing::EXPO_IN: return t == 0 ? 0 : std::pow(2, 10 * t - 10);
            case Easing::EXPO_OUT: return t == 1 ? 1 : 1 - std::pow(2, -10 * t);
            case Easing::EXPO_IN_OUT: return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? std::pow(2, 20 * t - 10) / 2 : (2 - std::pow(2, -20 * t + 10)) / 2;
            case Easing::QUAD_IN: return t * t;
            case Easing::QUAD_OUT: return 1 - (1 - t) * (1 - t);
            case Easing::QUAD_IN_OUT: return t < 0.5 ? 2 * t * t : 1 - (-2 * t + 2) * (-2 * t + 2) / 2;
            case Easing::CUBIC_IN: return t * t * t;
            case Easing::CUBIC_OUT: return 1 - std::pow(1 - t, 3);
            case Easing::CUBIC_IN_OUT: return t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
            case Easing::QUART_IN: return t * t * t * t;
            case Easing::QUART_OUT: return 1 - std::pow(1 - t, 4);
            case Easing::QUART_IN_OUT: return t < 0.5 ? 8 * t * t * t * t : 1 - std::pow(-2 * t + 2, 4) / 2;
            case Easing::QUINT_IN: return t * t * t * t * t;
            case Easing::QUINT_OUT: return 1 - std::pow(1 - t, 5);
            case Easing::QUINT_IN_OUT: return t < 0.5 ? 16 * t * t * t * t * t : 1 - std::pow(-2 * t + 2, 5) / 2;
            case Easing::SINE_IN: return 1 - std::cos(t * M_PI / 2);
            case Easing::SINE_OUT: return std::sin(t * M_PI / 2);
            case Easing::SINE_IN_OUT: return -(std::cos(M_PI * t) - 1) / 2;
            default: return t;
        }
    }
    
    float bounce_ease_out(float t) const {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;
        if (t < 1 / d1) return n1 * t * t;
        if (t < 2 / d1) return n1 * (t -= 1.5f / d1) * t + 0.75f;
        if (t < 2.5 / d1) return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        return n1 * (t -= 2.625f / d1) * t + 0.984375f;
    }
    
    float elastic_ease_out(float t) const {
        const float c4 = (2 * M_PI) / 3;
        return t == 0 ? 0 : t == 1 ? 1 : std::pow(2, -10 * t) * std::sin((t * 10 - 0.75f) * c4) + 1;
    }
    
    float back_ease_in(float t) const {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1;
        return c3 * t * t * t - c1 * t * t;
    }
    
    float back_ease_out(float t) const {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1;
        return 1 + c3 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
    }
    
    float back_ease_in_out(float t) const {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        return t < 0.5 ? (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2 : (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
    }
    
    float duration_;
    Easing easing_;
    float progress_;
    bool running_;
    std::chrono::steady_clock::time_point start_time_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 6: WIDGET BASE CLASS
// ═══════════════════════════════════════════════════════════════════════════════

class Widget : public std::enable_shared_from_this<Widget> {
public:
    virtual ~Widget() = default;
    
    virtual void render(int x, int y, int w, int h) = 0;
    virtual void handle_event(const Event& e) {}
    virtual int get_width() const { return style.width > 0 ? style.width : Terminal::instance().width(); }
    virtual int get_height() const { return style.height > 0 ? style.height : 1; }
    virtual int get_min_width() const { return style.min_width > 0 ? style.min_width : 0; }
    virtual int get_min_height() const { return style.min_height > 0 ? style.min_height : 1; }
    virtual int get_max_width() const { return style.max_width > 0 ? style.max_width : Terminal::instance().width(); }
    virtual int get_max_height() const { return style.max_height > 0 ? style.max_height : Terminal::instance().height(); }
    
    virtual bool is_focusable() const { return false; }
    virtual bool is_clickable() const { return false; }
    virtual bool is_hoverable() const { return false; }
    
    virtual void on_focus() { focused = true; }
    virtual void on_blur() { focused = false; }
    virtual void on_click(int x, int y) {}
    virtual void on_hover(int x, int y) {}
    virtual void on_key(int key) {}
    virtual void on_text_input(const std::string& text) {}
    
    virtual void update(float dt) {}
    
    Style style;
    bool focused = false;
    bool hovered = false;
    bool visible = true;
    bool enabled = true;
    int x = 0, y = 0;
    int computed_width = 0, computed_height = 0;
    
    std::string id;
    std::string widget_type;
    
    void set_id(const std::string& i) { id = i; }
    const std::string& get_id() const { return id; }
    
    void set_visible(bool v) { visible = v; }
    bool get_visible() const { return visible; }
    
    void set_enabled(bool e) { enabled = e; }
    bool get_enabled() const { return enabled; }
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 7: TEXT WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class Text : public Widget {
public:
    Text(const std::string& content = "", const Style& s = Style()) : content_(content) {
        style = s;
        widget_type = "Text";
    }
    
    void set_content(const std::string& c) { content_ = c; }
    const std::string& get_content() const { return content_; }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        std::vector<std::string> lines = wrap_text(content_, w);
        
        for (int i = 0; i < h && i < (int)lines.size(); i++) {
            std::string display = lines[i];
            if ((int)display.length() > w) {
                display = display.substr(0, w - 3) + "...";
            }
            
            // Apply text alignment
            if (style.text_align == "center") {
                int pad = (w - display.length()) / 2;
                display = std::string(pad, ' ') + display;
            } else if (style.text_align == "right") {
                int pad = w - display.length();
                display = std::string(pad, ' ') + display;
            }
            
            // Pad to width
            while ((int)display.length() < w) {
                display += " ";
            }
            
            // Apply styles
            term.write(style.fg_color.fg());
            term.write(style.bg_color.bg());
            if (style.bold) term.write("\033[1m");
            if (style.italic) term.write("\033[3m");
            if (style.underline) term.write("\033[4m");
            if (style.strikethrough) term.write("\033[9m");
            if (style.dim) term.write("\033[2m");
            if (style.blink) term.write("\033[5m");
            if (style.reverse) term.write("\033[7m");
            if (style.hidden) term.write("\033[8m");
            
            term.moveTo(y + i, x);
            term.write(display);
            
            term.write("\033[0m");
        }
    }
    
    int get_height() const override {
        if (style.height > 0) return style.height;
        return wrap_text(content_, get_width()).size();
    }
    
private:
    std::vector<std::string> wrap_text(const std::string& text, int width) const {
        std::vector<std::string> lines;
        if (width <= 0) {
            lines.push_back(text);
            return lines;
        }
        
        std::istringstream iss(text);
        std::string word;
        std::string current_line;
        
        while (iss >> word) {
            if (current_line.empty()) {
                current_line = word;
            } else if ((int)current_line.length() + 1 + (int)word.length() <= width) {
                current_line += " " + word;
            } else {
                lines.push_back(current_line);
                current_line = word;
            }
        }
        if (!current_line.empty()) {
            lines.push_back(current_line);
        }
        
        if (lines.empty()) lines.push_back("");
        return lines;
    }
    
    std::string content_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 8: BUTTON WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class Button : public Widget {
public:
    Button(const std::string& label = "", const Style& s = Style(), std::function<void()> on_click = nullptr)
        : label_(label), on_click_(on_click) {
        style = s;
        widget_type = "Button";
        if (style.bg_color.r == 255 && style.bg_color.g == 255 && style.bg_color.b == 255) {
            style.bg_color = Color::BLUE;
        }
        if (style.fg_color.r == 255 && style.fg_color.g == 255 && style.fg_color.b == 255) {
            style.fg_color = Color::WHITE;
        }
    }
    
    void set_label(const std::string& l) { label_ = l; }
    const std::string& get_label() const { return label_; }
    
    void set_on_click(std::function<void()> cb) { on_click_ = cb; }
    void click() { if (on_click_ && enabled) on_click_(); }
    
    bool is_clickable() const override { return true; }
    bool is_focusable() const override { return true; }
    
    void on_click(int x, int y) override { click(); }
    void on_key(int key) override {
        if (key == '\n' || key == '\r' || key == ' ') {
            click();
        }
    }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        std::string display = " " + label_ + " ";
        if ((int)display.length() > w) {
            display = display.substr(0, w - 3) + "...";
        }
        
        // Apply text alignment
        if (style.text_align == "center") {
            int pad = (w - display.length()) / 2;
            display = std::string(pad, ' ') + display;
        } else if (style.text_align == "right") {
            int pad = w - display.length();
            display = std::string(pad, ' ') + display;
        }
        
        // Pad to width
        while ((int)display.length() < w) {
            display += " ";
        }
        
        // Apply styles
        term.write(style.fg_color.fg());
        term.write(style.bg_color.bg());
        if (style.bold || focused) term.write("\033[1m");
        if (focused) term.write("\033[7m");
        if (hovered) term.write("\033[4m");
        
        term.moveTo(y, x);
        term.write(display);
        
        term.write("\033[0m");
    }
    
    int get_height() const override { return 1; }
    
private:
    std::string label_;
    std::function<void()> on_click_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 9: TEXT INPUT WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class TextInput : public Widget {
public:
    TextInput(const std::string& value = "", const std::string& placeholder = "", const Style& s = Style(),
              std::function<void(const std::string&)> on_change = nullptr,
              std::function<void(const std::string&)> on_submit = nullptr)
        : value_(value), placeholder_(placeholder), on_change_(on_change), on_submit_(on_submit) {
        style = s;
        widget_type = "TextInput";
        cursor_pos_ = value_.length();
    }
    
    void set_value(const std::string& v) { 
        value_ = v; 
        cursor_pos_ = value_.length(); 
        if (on_change_) on_change_(value_);
    }
    const std::string& get_value() const { return value_; }
    
    void set_placeholder(const std::string& p) { placeholder_ = p; }
    const std::string& get_placeholder() const { return placeholder_; }
    
    void set_on_change(std::function<void(const std::string&)> cb) { on_change_ = cb; }
    void set_on_submit(std::function<void(const std::string&)> cb) { on_submit_ = cb; }
    
    void insert(char c) {
        value_.insert(cursor_pos_, 1, c);
        cursor_pos_++;
        if (on_change_) on_change_(value_);
    }
    
    void backspace() {
        if (cursor_pos_ > 0) {
            value_.erase(cursor_pos_ - 1, 1);
            cursor_pos_--;
            if (on_change_) on_change_(value_);
        }
    }
    
    void delete_forward() {
        if (cursor_pos_ < (int)value_.length()) {
            value_.erase(cursor_pos_, 1);
            if (on_change_) on_change_(value_);
        }
    }
    
    void move_cursor_left() { if (cursor_pos_ > 0) cursor_pos_--; }
    void move_cursor_right() { if (cursor_pos_ < (int)value_.length()) cursor_pos_++; }
    void move_cursor_home() { cursor_pos_ = 0; }
    void move_cursor_end() { cursor_pos_ = value_.length(); }
    
    bool is_focusable() const override { return true; }
    
    void on_key(int key) override {
        if (key == 127 || key == 8) backspace();
        else if (key == 27) {
            // Handle escape sequences
        }
        else if (key == '\n' || key == '\r') {
            if (on_submit_) on_submit_(value_);
        }
        else if (key >= 32 && key < 127) {
            insert((char)key);
        }
    }
    
    void on_text_input(const std::string& text) override {
        for (char c : text) {
            insert(c);
        }
    }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        std::string display = value_.empty() ? placeholder_ : value_;
        if ((int)display.length() > w) {
            display = display.substr(0, w - 3) + "...";
        }
        
        // Pad to width
        std::string padded = display;
        while ((int)padded.length() < w) {
            padded += " ";
        }
        
        // Apply styles
        term.write(value_.empty() ? Color(128, 128, 128).fg() : style.fg_color.fg());
        term.write(style.bg_color.bg());
        if (focused) term.write("\033[7m");
        
        term.moveTo(y, x);
        term.write(padded);
        
        term.write("\033[0m");
        
        // Show cursor if focused
        if (focused) {
            term.moveTo(y, x + cursor_pos_);
            term.write("\033[?25h");
        }
    }
    
    int get_height() const override { return 1; }
    
private:
    std::string value_;
    std::string placeholder_;
    int cursor_pos_ = 0;
    std::function<void(const std::string&)> on_change_;
    std::function<void(const std::string&)> on_submit_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 10: VIEW (CONTAINER) WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class View : public Widget {
public:
    enum Layout { VERTICAL, HORIZONTAL, FLEX, GRID, ABSOLUTE, STACK, WRAP };
    
    View(const Style& s = Style(), Layout layout = VERTICAL) : layout_(layout) {
        style = s;
        widget_type = "View";
    }
    
    void add(std::shared_ptr<Widget> child) {
        children_.push_back(child);
    }
    
    void remove(size_t index) {
        if (index < children_.size()) {
            children_.erase(children_.begin() + index);
        }
    }
    
    void remove_by_id(const std::string& id) {
        children_.erase(
            std::remove_if(children_.begin(), children_.end(),
                [&id](const std::shared_ptr<Widget>& w) { return w->id == id; }),
            children_.end()
        );
    }
    
    void clear() { children_.clear(); }
    
    size_t child_count() const { return children_.size(); }
    
    std::shared_ptr<Widget> get_child(size_t index) {
        if (index < children_.size()) return children_[index];
        return nullptr;
    }
    
    std::shared_ptr<Widget> find_by_id(const std::string& id) {
        for (auto& child : children_) {
            if (child->id == id) return child;
        }
        return nullptr;
    }
    
    void set_layout(Layout l) { layout_ = l; }
    Layout get_layout() const { return layout_; }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        // Draw background
        if (style.bg_color.r != 0 || style.bg_color.g != 0 || style.bg_color.b != 0) {
            term.write(style.bg_color.bg());
            for (int row = 0; row < h; row++) {
                term.moveTo(y + row, x);
                std::string line(w, ' ');
                term.write(line);
            }
            term.write("\033[0m");
        }
        
        // Draw border
        if (style.border.type != BorderStyle::NONE) {
            render_border(x, y, w, h);
        }
        
        // Render children
        if (children_.empty()) return;
        
        int child_x = x + style.padding_left + style.border.type;
        int child_y = y + style.padding_top + style.border.type;
        int available_width = w - style.total_padding_h() - 2 * style.border.type;
        int available_height = h - style.total_padding_v() - 2 * style.border.type;
        
        if (layout_ == VERTICAL) {
            render_vertical(child_x, child_y, available_width, available_height);
        } else if (layout_ == HORIZONTAL) {
            render_horizontal(child_x, child_y, available_width, available_height);
        } else if (layout_ == FLEX) {
            render_flex(child_x, child_y, available_width, available_height);
        } else if (layout_ == GRID) {
            render_grid(child_x, child_y, available_width, available_height);
        } else if (layout_ == STACK) {
            render_stack(child_x, child_y, available_width, available_height);
        } else if (layout_ == WRAP) {
            render_wrap(child_x, child_y, available_width, available_height);
        }
    }
    
    int get_width() const override {
        if (style.width > 0) return style.width;
        int max_w = 0;
        for (auto& child : children_) {
            max_w = std::max(max_w, child->get_width());
        }
        return max_w + style.total_padding_h() + 2 * style.border.type;
    }
    
    int get_height() const override {
        if (style.height > 0) return style.height;
        int total_h = 0;
        for (auto& child : children_) {
            total_h += child->get_height();
        }
        return total_h + style.total_padding_v() + 2 * style.border.type;
    }
    
private:
    void render_border(int x, int y, int w, int h) {
        auto& term = Terminal::instance();
        const auto& b = style.border;
        
        term.write(b.color.fg());
        
        // Top border
        term.moveTo(y, x);
        term.write(b.top_left());
        for (int i = 1; i < w - 1; i++) term.write(b.horizontal());
        term.write(b.top_right());
        
        // Side borders
        for (int row = 1; row < h - 1; row++) {
            term.moveTo(y + row, x);
            term.write(b.vertical());
            term.moveTo(y + row, x + w - 1);
            term.write(b.vertical());
        }
        
        // Bottom border
        term.moveTo(y + h - 1, x);
        term.write(b.bottom_left());
        for (int i = 1; i < w - 1; i++) term.write(b.horizontal());
        term.write(b.bottom_right());
        
        term.write("\033[0m");
    }
    
    void render_vertical(int x, int y, int w, int h) {
        int total_h = 0;
        for (auto& child : children_) {
            total_h += child->get_height();
        }
        
        int per_child = children_.empty() ? h : h / children_.size();
        int extra = children_.empty() ? 0 : h % children_.size();
        
        int current_y = y;
        for (size_t i = 0; i < children_.size(); i++) {
            int child_h = per_child + (i < (size_t)extra ? 1 : 0);
            children_[i]->render(x, current_y, w, child_h);
            current_y += child_h;
        }
    }
    
    void render_horizontal(int x, int y, int w, int h) {
        int total_w = 0;
        for (auto& child : children_) {
            total_w += child->get_width();
        }
        
        int per_child = children_.empty() ? w : w / children_.size();
        int extra = children_.empty() ? 0 : w % children_.size();
        
        int current_x = x;
        for (size_t i = 0; i < children_.size(); i++) {
            int child_w = per_child + (i < (size_t)extra ? 1 : 0);
            children_[i]->render(current_x, y, child_w, h);
            current_x += child_w;
        }
    }
    
    void render_flex(int x, int y, int w, int h) {
        for (auto& child : children_) {
            child->render(x, y, w, h);
            y += child->get_height();
        }
    }
    
    void render_grid(int x, int y, int w, int h) {
        int cols = (int)std::ceil(std::sqrt(children_.size()));
        int rows = (int)std::ceil((double)children_.size() / cols);
        
        int cell_w = w / cols;
        int cell_h = h / rows;
        
        for (size_t i = 0; i < children_.size(); i++) {
            int col = i % cols;
            int row = i / cols;
            children_[i]->render(x + col * cell_w, y + row * cell_h, cell_w, cell_h);
        }
    }
    
    void render_stack(int x, int y, int w, int h) {
        for (auto& child : children_) {
            child->render(x, y, w, h);
        }
    }
    
    void render_wrap(int x, int y, int w, int h) {
        int current_x = x;
        int current_y = y;
        int row_height = 0;
        
        for (auto& child : children_) {
            int child_w = child->get_width();
            int child_h = child->get_height();
            
            if (current_x + child_w > x + w) {
                current_x = x;
                current_y += row_height;
                row_height = 0;
            }
            
            child->render(current_x, current_y, child_w, child_h);
            current_x += child_w;
            row_height = std::max(row_height, child_h);
        }
    }
    
    std::vector<std::shared_ptr<Widget>> children_;
    Layout layout_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 11: PROGRESS BAR WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class ProgressBar : public Widget {
public:
    ProgressBar(float progress = 0.0f, const Style& s = Style()) : progress_(progress) {
        style = s;
        widget_type = "ProgressBar";
        style.bg_color = Color::GRAY;
    }
    
    void set_progress(float p) { progress_ = std::clamp(p, 0.0f, 1.0f); }
    float get_progress() const { return progress_; }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        int filled = (int)(w * progress_);
        
        term.moveTo(y, x);
        term.write(style.bg_color.bg());
        
        for (int i = 0; i < w; i++) {
            if (i < filled) {
                term.write(style.fg_color.fg());
                term.write("█");
            } else {
                term.write(Color(60, 60, 60).fg());
                term.write("░");
            }
        }
        
        term.write("\033[0m");
    }
    
    int get_height() const override { return 1; }
    
private:
    float progress_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 12: CHECKBOX WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class Checkbox : public Widget {
public:
    Checkbox(const std::string& label = "", bool checked = false, const Style& s = Style(),
             std::function<void(bool)> on_change = nullptr)
        : label_(label), checked_(checked), on_change_(on_change) {
        style = s;
        widget_type = "Checkbox";
    }
    
    void set_checked(bool c) { 
        checked_ = c; 
        if (on_change_) on_change_(c);
    }
    bool get_checked() const { return checked_; }
    
    void toggle() { set_checked(!checked_); }
    
    bool is_clickable() const override { return true; }
    bool is_focusable() const override { return true; }
    
    void on_click(int x, int y) override { toggle(); }
    void on_key(int key) override {
        if (key == '\n' || key == '\r' || key == ' ') {
            toggle();
        }
    }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        std::string display = std::string("[") + (checked_ ? "X" : " ") + "] " + label_;
        if ((int)display.length() > w) {
            display = display.substr(0, w - 3) + "...";
        }
        
        term.write(style.fg_color.fg());
        term.write(style.bg_color.bg());
        if (focused) term.write("\033[7m");
        
        term.moveTo(y, x);
        term.write(display);
        
        term.write("\033[0m");
    }
    
    int get_height() const override { return 1; }
    
private:
    std::string label_;
    bool checked_;
    std::function<void(bool)> on_change_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 13: SLIDER WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class Slider : public Widget {
public:
    Slider(float value = 0.0f, float min = 0.0f, float max = 1.0f, const Style& s = Style(),
           std::function<void(float)> on_change = nullptr)
        : value_(value), min_(min), max_(max), on_change_(on_change) {
        style = s;
        widget_type = "Slider";
    }
    
    void set_value(float v) { 
        value_ = std::clamp(v, min_, max_); 
        if (on_change_) on_change_(value_);
    }
    float get_value() const { return value_; }
    
    void set_min(float m) { min_ = m; set_value(value_); }
    void set_max(float m) { max_ = m; set_value(value_); }
    
    bool is_focusable() const override { return true; }
    
    void on_key(int key) override {
        float step = (max_ - min_) / 100;
        if (key == 68 || key == 'a' || key == 'A') set_value(value_ - step);
        else if (key == 67 || key == 'd' || key == 'D') set_value(value_ + step);
    }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        int filled = (int)(w * (value_ - min_) / (max_ - min_));
        
        term.moveTo(y, x);
        term.write(style.bg_color.bg());
        
        for (int i = 0; i < w; i++) {
            if (i < filled) {
                term.write(style.fg_color.fg());
                term.write("█");
            } else {
                term.write(Color(60, 60, 60).fg());
                term.write("░");
            }
        }
        
        term.write("\033[0m");
    }
    
    int get_height() const override { return 1; }
    
private:
    float value_, min_, max_;
    std::function<void(float)> on_change_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 14: FRAME WIDGET
// ═══════════════════════════════════════════════════════════════════════════════

class Frame : public Widget {
public:
    Frame(const std::string& title = "", const Style& s = Style()) : title_(title) {
        style = s;
        widget_type = "Frame";
    }
    
    void set_title(const std::string& t) { title_ = t; }
    const std::string& get_title() const { return title_; }
    
    void add(std::shared_ptr<Widget> child) {
        child_ = child;
    }
    
    void render(int x, int y, int w, int h) override {
        if (!visible) return;
        auto& term = Terminal::instance();
        
        // Draw border
        term.write(style.fg_color.fg());
        
        // Top border
        term.moveTo(y, x);
        term.write("┌");
        for (int i = 1; i < w - 1; i++) term.write("─");
        term.write("┐");
        
        // Title
        if (!title_.empty()) {
            term.moveTo(y, x + 2);
            term.write(" " + title_ + " ");
        }
        
        // Side borders
        for (int row = 1; row < h - 1; row++) {
            term.moveTo(y + row, x);
            term.write("│");
            term.moveTo(y + row, x + w - 1);
            term.write("│");
        }
        
        // Bottom border
        term.moveTo(y + h - 1, x);
        term.write("└");
        for (int i = 1; i < w - 1; i++) term.write("─");
        term.write("┘");
        
        // Render child
        if (child_) {
            child_->render(x + 2, y + 1, w - 4, h - 2);
        }
        
        term.write("\033[0m");
    }
    
    int get_width() const override {
        if (style.width > 0) return style.width;
        if (child_) return child_->get_width() + 4;
        return 20;
    }
    
    int get_height() const override {
        if (style.height > 0) return style.height;
        if (child_) return child_->get_height() + 2;
        return 5;
    }
    
private:
    std::string title_;
    std::shared_ptr<Widget> child_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 15: APPLICATION
// ═══════════════════════════════════════════════════════════════════════════════

class Application {
public:
    Application() : running_(false), focused_index_(0), frame_count_(0), fps_(0) {}
    
    void run(std::shared_ptr<View> root) {
        auto& term = Terminal::instance();
        term.init();
        running_ = true;
        root_ = root;
        
        auto last_time = std::chrono::steady_clock::now();
        auto fps_time = last_time;
        
        // Main render loop
        while (running_) {
            auto current_time = std::chrono::steady_clock::now();
            float dt = std::chrono::duration<float>(current_time - last_time).count();
            last_time = current_time;
            
            // FPS counter
            frame_count_++;
            if (std::chrono::duration<float>(current_time - fps_time).count() >= 1.0f) {
                fps_ = frame_count_;
                frame_count_ = 0;
                fps_time = current_time;
            }
            
            render();
            
            // Cap at ~60 FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
    
    void stop() {
        running_ = false;
        Terminal::instance().cleanup();
    }
    
    int get_fps() const { return fps_; }
    
private:
    void render() {
        auto& term = Terminal::instance();
        term.clear();
        root_->render(0, 0, term.width(), term.height());
        term.flush();
    }
    
    bool running_;
    int focused_index_;
    int frame_count_;
    int fps_;
    std::shared_ptr<View> root_;
};

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 16: PYTHON BINDINGS
// ═══════════════════════════════════════════════════════════════════════════════

PYBIND11_MODULE(pytuint, m) {
    m.doc() = "PYTUINT - Modern Terminal User Interface Library";
    
    // Color
    py::class_<Color>(m, "Color")
        .def(py::init<int, int, int>(), py::arg("r") = 255, py::arg("g") = 255, py::arg("b") = 255)
        .def_readwrite("r", &Color::r)
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def("blend", &Color::blend)
        .def("darken", &Color::darken)
        .def("lighten", &Color::lighten)
        .def_static("from_hex", &Color::from_hex)
        .def_static("from_hsl", &Color::from_hsl);
    
    // BorderStyle
    py::enum_<BorderStyle::Type>(m, "BorderType")
        .value("NONE", BorderStyle::NONE)
        .value("SINGLE", BorderStyle::SINGLE)
        .value("DOUBLE", BorderStyle::DOUBLE)
        .value("ROUNDED", BorderStyle::ROUNDED)
        .value("THICK", BorderStyle::THICK)
        .value("DASHED", BorderStyle::DASHED)
        .value("DOTTED", BorderStyle::DOTTED)
        .value("ASCII", BorderStyle::ASCII)
        .value("BLOCK", BorderStyle::BLOCK)
        .value("HEAVY", BorderStyle::HEAVY);
    
    py::class_<BorderStyle>(m, "BorderStyle")
        .def(py::init<>())
        .def_readwrite("type", &BorderStyle::type)
        .def_readwrite("color", &BorderStyle::color);
    
    // Style
    py::class_<Style>(m, "Style")
        .def(py::init<>())
        .def("set_fg", &Style::set_fg)
        .def("set_bg", &Style::set_bg)
        .def("set_bold", &Style::set_bold)
        .def("set_italic", &Style::set_italic)
        .def("set_underline", &Style::set_underline)
        .def("set_strikethrough", &Style::set_strikethrough)
        .def("set_dim", &Style::set_dim)
        .def("set_blink", &Style::set_blink)
        .def("set_reverse", &Style::set_reverse)
        .def("set_hidden", &Style::set_hidden)
        .def("set_padding", (Style&(Style::*)(int)) &Style::set_padding)
        .def("set_padding", (Style&(Style::*)(int,int)) &Style::set_padding)
        .def("set_padding", (Style&(Style::*)(int,int,int,int)) &Style::set_padding)
        .def("set_padding_top", &Style::set_padding_top)
        .def("set_padding_bottom", &Style::set_padding_bottom)
        .def("set_padding_left", &Style::set_padding_left)
        .def("set_padding_right", &Style::set_padding_right)
        .def("set_margin", (Style&(Style::*)(int)) &Style::set_margin)
        .def("set_margin", (Style&(Style::*)(int,int)) &Style::set_margin)
        .def("set_margin", (Style&(Style::*)(int,int,int,int)) &Style::set_margin)
        .def("set_margin_top", &Style::set_margin_top)
        .def("set_margin_bottom", &Style::set_margin_bottom)
        .def("set_margin_left", &Style::set_margin_left)
        .def("set_margin_right", &Style::set_margin_right)
        .def("set_width", &Style::set_width)
        .def("set_height", &Style::set_height)
        .def("set_min_width", &Style::set_min_width)
        .def("set_min_height", &Style::set_min_height)
        .def("set_max_width", &Style::set_max_width)
        .def("set_max_height", &Style::set_max_height)
        .def("set_border", &Style::set_border)
        .def("set_text_align", &Style::set_text_align)
        .def("set_vertical_align", &Style::set_vertical_align)
        .def("set_overflow", &Style::set_overflow)
        .def("set_word_wrap", &Style::set_word_wrap)
        .def("set_opacity", &Style::set_opacity)
        .def("set_z_index", &Style::set_z_index);
    
    // Widget base
    py::class_<Widget, std::shared_ptr<Widget>>(m, "Widget")
        .def("set_id", &Widget::set_id)
        .def("get_id", &Widget::get_id)
        .def("set_visible", &Widget::set_visible)
        .def("get_visible", &Widget::get_visible)
        .def("set_enabled", &Widget::set_enabled)
        .def("get_enabled", &Widget::get_enabled)
        .def_readwrite("style", &Widget::style)
        .def_readwrite("focused", &Widget::focused)
        .def_readwrite("hovered", &Widget::hovered)
        .def_readwrite("visible", &Widget::visible)
        .def_readwrite("enabled", &Widget::enabled);
    
    // Text
    py::class_<Text, Widget, std::shared_ptr<Text>>(m, "Text")
        .def(py::init<const std::string&, const Style&>(), py::arg("content") = "", py::arg("style") = Style())
        .def("set_content", &Text::set_content)
        .def("get_content", &Text::get_content);
    
    // Button
    py::class_<Button, Widget, std::shared_ptr<Button>>(m, "Button")
        .def(py::init<const std::string&, const Style&, std::function<void()>>(),
             py::arg("label") = "", py::arg("style") = Style(), py::arg("on_click") = nullptr)
        .def("set_label", &Button::set_label)
        .def("get_label", &Button::get_label)
        .def("set_on_click", &Button::set_on_click)
        .def("click", &Button::click);
    
    // TextInput
    py::class_<TextInput, Widget, std::shared_ptr<TextInput>>(m, "TextInput")
        .def(py::init<const std::string&, const std::string&, const Style&,
                      std::function<void(const std::string&)>, std::function<void(const std::string&)>>(),
             py::arg("value") = "", py::arg("placeholder") = "", py::arg("style") = Style(),
             py::arg("on_change") = nullptr, py::arg("on_submit") = nullptr)
        .def("set_value", &TextInput::set_value)
        .def("get_value", &TextInput::get_value)
        .def("set_placeholder", &TextInput::set_placeholder)
        .def("get_placeholder", &TextInput::get_placeholder)
        .def("set_on_change", &TextInput::set_on_change)
        .def("set_on_submit", &TextInput::set_on_submit)
        .def("insert", &TextInput::insert)
        .def("backspace", &TextInput::backspace)
        .def("delete_forward", &TextInput::delete_forward)
        .def("move_cursor_left", &TextInput::move_cursor_left)
        .def("move_cursor_right", &TextInput::move_cursor_right)
        .def("move_cursor_home", &TextInput::move_cursor_home)
        .def("move_cursor_end", &TextInput::move_cursor_end);
    
    // View
    py::enum_<View::Layout>(m, "Layout")
        .value("VERTICAL", View::VERTICAL)
        .value("HORIZONTAL", View::HORIZONTAL)
        .value("FLEX", View::FLEX)
        .value("GRID", View::GRID)
        .value("ABSOLUTE", View::ABSOLUTE)
        .value("STACK", View::STACK)
        .value("WRAP", View::WRAP);
    
    py::class_<View, Widget, std::shared_ptr<View>>(m, "View")
        .def(py::init<const Style&, View::Layout>(), py::arg("style") = Style(), py::arg("layout") = View::VERTICAL)
        .def("add", &View::add)
        .def("remove", &View::remove)
        .def("remove_by_id", &View::remove_by_id)
        .def("clear", &View::clear)
        .def("child_count", &View::child_count)
        .def("get_child", &View::get_child)
        .def("find_by_id", &View::find_by_id)
        .def("set_layout", &View::set_layout)
        .def("get_layout", &View::get_layout);
    
    // ProgressBar
    py::class_<ProgressBar, Widget, std::shared_ptr<ProgressBar>>(m, "ProgressBar")
        .def(py::init<float, const Style&>(), py::arg("progress") = 0.0f, py::arg("style") = Style())
        .def("set_progress", &ProgressBar::set_progress)
        .def("get_progress", &ProgressBar::get_progress);
    
    // Checkbox
    py::class_<Checkbox, Widget, std::shared_ptr<Checkbox>>(m, "Checkbox")
        .def(py::init<const std::string&, bool, const Style&, std::function<void(bool)>>(),
             py::arg("label") = "", py::arg("checked") = false, py::arg("style") = Style(), py::arg("on_change") = nullptr)
        .def("set_checked", &Checkbox::set_checked)
        .def("get_checked", &Checkbox::get_checked)
        .def("toggle", &Checkbox::toggle);
    
    // Slider
    py::class_<Slider, Widget, std::shared_ptr<Slider>>(m, "Slider")
        .def(py::init<float, float, float, const Style&, std::function<void(float)>>(),
             py::arg("value") = 0.0f, py::arg("min") = 0.0f, py::arg("max") = 1.0f,
             py::arg("style") = Style(), py::arg("on_change") = nullptr)
        .def("set_value", &Slider::set_value)
        .def("get_value", &Slider::get_value)
        .def("set_min", &Slider::set_min)
        .def("set_max", &Slider::set_max);
    
    // Frame
    py::class_<Frame, Widget, std::shared_ptr<Frame>>(m, "Frame")
        .def(py::init<const std::string&, const Style&>(), py::arg("title") = "", py::arg("style") = Style())
        .def("set_title", &Frame::set_title)
        .def("get_title", &Frame::get_title)
        .def("add", &Frame::add);
    
    // Application
    py::class_<Application>(m, "Application")
        .def(py::init<>())
        .def("run", &Application::run)
        .def("stop", &Application::stop)
        .def("get_fps", &Application::get_fps);
    
    // Terminal
    py::class_<Terminal>(m, "Terminal")
        .def_static("init", &Terminal::instance()->init)
        .def_static("cleanup", &Terminal::instance()->cleanup)
        .def_static("width", &Terminal::instance()->width)
        .def_static("height", &Terminal::instance()->height)
        .def_static("get_key", &Terminal::instance()->get_key)
        .def_static("has_input", &Terminal::instance()->has_input);
    
    // Animation
    py::enum_<Easing>(m, "Easing")
        .value("LINEAR", Easing::LINEAR)
        .value("EASE_IN", Easing::EASE_IN)
        .value("EASE_OUT", Easing::EASE_OUT)
        .value("EASE_IN_OUT", Easing::EASE_IN_OUT)
        .value("BOUNCE", Easing::BOUNCE)
        .value("ELASTIC", Easing::ELASTIC)
        .value("BACK_IN", Easing::BACK_IN)
        .value("BACK_OUT", Easing::BACK_OUT)
        .value("BACK_IN_OUT", Easing::BACK_IN_OUT)
        .value("CIRC_IN", Easing::CIRC_IN)
        .value("CIRC_OUT", Easing::CIRC_OUT)
        .value("CIRC_IN_OUT", Easing::CIRC_IN_OUT)
        .value("EXPO_IN", Easing::EXPO_IN)
        .value("EXPO_OUT", Easing::EXPO_OUT)
        .value("EXPO_IN_OUT", Easing::EXPO_IN_OUT)
        .value("QUAD_IN", Easing::QUAD_IN)
        .value("QUAD_OUT", Easing::QUAD_OUT)
        .value("QUAD_IN_OUT", Easing::QUAD_IN_OUT)
        .value("CUBIC_IN", Easing::CUBIC_IN)
        .value("CUBIC_OUT", Easing::CUBIC_OUT)
        .value("CUBIC_IN_OUT", Easing::CUBIC_IN_OUT)
        .value("QUART_IN", Easing::QUART_IN)
        .value("QUART_OUT", Easing::QUART_OUT)
        .value("QUART_IN_OUT", Easing::QUART_IN_OUT)
        .value("QUINT_IN", Easing::QUINT_IN)
        .value("QUINT_OUT", Easing::QUINT_OUT)
        .value("QUINT_IN_OUT", Easing::QUINT_IN_OUT)
        .value("SINE_IN", Easing::SINE_IN)
        .value("SINE_OUT", Easing::SINE_OUT)
        .value("SINE_IN_OUT", Easing::SINE_IN_OUT);
    
    py::class_<Animation>(m, "Animation")
        .def(py::init<float, Easing>(), py::arg("duration") = 1.0f, py::arg("easing") = Easing::LINEAR)
        .def("start", &Animation::start)
        .def("stop", &Animation::stop)
        .def("reset", &Animation::reset)
        .def("update", &Animation::update)
        .def("get_progress", &Animation::get_progress)
        .def("get_raw_progress", &Animation::get_raw_progress)
        .def("is_running", &Animation::is_running)
        .def("is_finished", &Animation::is_finished)
        .def("set_duration", &Animation::set_duration)
        .def("set_easing", &Animation::set_easing);
}

} // namespace pytuint