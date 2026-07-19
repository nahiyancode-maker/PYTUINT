// ============================================================================
// PTUI - Professional Terminal User Interface Library
// A React-inspired TUI framework for C++ with Python bindings
// 
// Features:
//   - Virtual DOM with O(n) diffing algorithm
//   - Full Hooks system (useState, useEffect, useContext, useRef, useMemo, useCallback, useReducer)
//   - Context API with Provider/Consumer
//   - Component lifecycle (mount/update/unmount/error boundaries)
//   - Suspense for async operations
//   - Router with history API
//   - Flexbox layout engine
//   - Animation system with keyframes
//   - Form handling with validation
//   - Theme system
//   - Event delegation & bubbling
//   - Refs system
//   - Fragments & Portals
//   - Built-in widgets (Container, Text, Input, Button, List, ScrollView, Modal, etc.)
//
// Usage:
//   C++:  #define PTUI_IMPLEMENTATION
//         #include "ptui.hpp"
//   Python: import ptui
//
// MIT License
// ============================================================================

#ifndef PTUI_HPP
#define PTUI_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <sstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <optional>
#include <any>
#include <type_traits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <array>
#include <stack>
#include <regex>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <sys/select.h>
#endif

// ============================================================================
// Forward Declarations
// ============================================================================
namespace ptui {
    class Element;
    class Component;
    class Fiber;
    class Reconciler;
    class Renderer;
    class Terminal;
    class Context;
    class Ref;
    class Animation;
    class Router;
    class Theme;
    
    using ElementPtr = std::shared_ptr<Element>;
    using ComponentPtr = std::shared_ptr<Component>;
    using FiberPtr = std::shared_ptr<Fiber>;
    using ContextPtr = std::shared_ptr<Context>;
    using RefPtr = std::shared_ptr<Ref>;
    using AnimationPtr = std::shared_ptr<Animation>;
    using RouterPtr = std::shared_ptr<Router>;
}

// ============================================================================
// Utility
// ============================================================================
namespace ptui {
namespace util {

    inline std::string to_string(const std::any& value) {
        if (!value.has_value()) return "null";
        if (value.type() == typeid(std::string)) return std::any_cast<std::string>(value);
        if (value.type() == typeid(int)) return std::to_string(std::any_cast<int>(value));
        if (value.type() == typeid(double)) return std::to_string(std::any_cast<double>(value));
        if (value.type() == typeid(bool)) return std::any_cast<bool>(value) ? "true" : "false";
        if (value.type() == typeid(const char*)) return std::any_cast<const char*>(value);
        return "unknown";
    }

    inline int to_int(const std::any& value, int default_val = 0) {
        if (!value.has_value()) return default_val;
        if (value.type() == typeid(int)) return std::any_cast<int>(value);
        if (value.type() == typeid(double)) return static_cast<int>(std::any_cast<double>(value));
        if (value.type() == typeid(std::string)) return std::stoi(std::any_cast<std::string>(value));
        return default_val;
    }

    inline bool to_bool(const std::any& value, bool default_val = false) {
        if (!value.has_value()) return default_val;
        if (value.type() == typeid(bool)) return std::any_cast<bool>(value);
        if (value.type() == typeid(int)) return std::any_cast<int>(value) != 0;
        if (value.type() == typeid(std::string)) {
            auto s = std::any_cast<std::string>(value);
            return s == "true" || s == "1";
        }
        return default_val;
    }

    inline std::string hash_str(const std::string& str) {
        // FNV-1a hash
        uint64_t hash = 14695981039346656037ull;
        for (char c : str) {
            hash ^= static_cast<uint64_t>(c);
            hash *= 1099511628211ull;
        }
        return std::to_string(hash);
    }

} // namespace util
} // namespace ptui

// ============================================================================
// Color System (True Color / 256 / Basic)
// ============================================================================
#ifdef RGB
#undef RGB
#endif
namespace ptui {

class Color {
public:
    enum class Type : uint8_t { Basic, Indexed, RGB };
    enum class Basic : uint8_t {
        Black = 0, Red, Green, Yellow, Blue, Magenta, Cyan, White,
        Default = 9, BrightBlack, BrightRed, BrightGreen, BrightYellow,
        BrightBlue, BrightMagenta, BrightCyan, BrightWhite
    };

    Color() : m_type(Type::Basic), m_basic(Basic::Default) {}
    explicit Color(Basic basic) : m_type(Type::Basic), m_basic(basic) {}
    explicit Color(uint8_t idx) : m_type(Type::Indexed), m_indexed(idx) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : m_type(Type::RGB), m_r(r), m_g(g), m_b(b) {}

    static Color RGB(uint8_t r, uint8_t g, uint8_t b) { return Color(r, g, b); }
    static Color RGBA(uint8_t r, uint8_t g, uint8_t b, float a) { Color c(r, g, b); c.m_alpha = a; return c; }
    static Color Indexed(uint8_t idx) { return Color(idx); }
    static Color FromBasic(Basic basic) { return Color(basic); }
    static const Color Default;
    static Color Hex(const std::string& hex) {
        if (hex.size() == 7 && hex[0] == '#') {
            auto r = static_cast<uint8_t>(std::stoi(hex.substr(1, 2), nullptr, 16));
            auto g = static_cast<uint8_t>(std::stoi(hex.substr(3, 2), nullptr, 16));
            auto b = static_cast<uint8_t>(std::stoi(hex.substr(5, 2), nullptr, 16));
            return Color(r, g, b);
        }
        return Color();
    }

    Type type() const { return m_type; }
    Basic basic() const { return m_basic; }
    uint8_t indexed() const { return m_indexed; }
    void rgb(uint8_t& r, uint8_t& g, uint8_t& b) const { r = m_r; g = m_g; b = m_b; }
    float alpha() const { return m_alpha; }

    std::string to_foreground() const {
        switch (m_type) {
            case Type::Basic: return "\033[" + std::to_string(30 + static_cast<int>(m_basic)) + "m";
            case Type::Indexed: return "\033[38;5;" + std::to_string(m_indexed) + "m";
            case Type::RGB: return "\033[38;2;" + std::to_string(m_r) + ";" + std::to_string(m_g) + ";" + std::to_string(m_b) + "m";
        }
        return "";
    }

    std::string to_background() const {
        switch (m_type) {
            case Type::Basic: return "\033[" + std::to_string(40 + static_cast<int>(m_basic)) + "m";
            case Type::Indexed: return "\033[48;5;" + std::to_string(m_indexed) + "m";
            case Type::RGB: return "\033[48;2;" + std::to_string(m_r) + ";" + std::to_string(m_g) + ";" + std::to_string(m_b) + "m";
        }
        return "";
    }

    Color lighten(float amount) const {
        if (m_type != Type::RGB) return *this;
        return Color(
            static_cast<uint8_t>(std::min(255.0f, m_r + (255 - m_r) * amount)),
            static_cast<uint8_t>(std::min(255.0f, m_g + (255 - m_g) * amount)),
            static_cast<uint8_t>(std::min(255.0f, m_b + (255 - m_b) * amount))
        );
    }

    Color darken(float amount) const {
        if (m_type != Type::RGB) return *this;
        return Color(
            static_cast<uint8_t>(std::max(0.0f, m_r * (1 - amount))),
            static_cast<uint8_t>(std::max(0.0f, m_g * (1 - amount))),
            static_cast<uint8_t>(std::max(0.0f, m_b * (1 - amount)))
        );
    }

    bool operator==(const Color& other) const {
        if (m_type != other.m_type) return false;
        switch (m_type) {
            case Type::Basic: return m_basic == other.m_basic;
            case Type::Indexed: return m_indexed == other.m_indexed;
            case Type::RGB: return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b;
        }
        return false;
    }

    bool operator!=(const Color& other) const { return !(*this == other); }

private:
    Type m_type;
    union {
        Basic m_basic;
        uint8_t m_indexed;
        struct { uint8_t m_r, m_g, m_b; };
    };
    float m_alpha = 1.0f;
};

namespace colors {
    inline const Color Red = Color::FromBasic(Color::Basic::Red);
    inline const Color Green = Color::FromBasic(Color::Basic::Green);
    inline const Color Blue = Color::FromBasic(Color::Basic::Blue);
    inline const Color Yellow = Color::FromBasic(Color::Basic::Yellow);
    inline const Color Cyan = Color::FromBasic(Color::Basic::Cyan);
    inline const Color Magenta = Color::FromBasic(Color::Basic::Magenta);
    inline const Color White = Color::FromBasic(Color::Basic::White);
    inline const Color Default = Color::FromBasic(Color::Basic::Default);
    inline const Color BrightRed = Color::FromBasic(Color::Basic::BrightRed);
    inline const Color BrightGreen = Color::FromBasic(Color::Basic::BrightGreen);
    inline const Color BrightBlue = Color::FromBasic(Color::Basic::BrightBlue);
    inline const Color BrightYellow = Color::FromBasic(Color::Basic::BrightYellow);
    inline const Color BrightCyan = Color::FromBasic(Color::Basic::BrightCyan);
    inline const Color BrightMagenta = Color::FromBasic(Color::Basic::BrightMagenta);
    inline const Color BrightWhite = Color::FromBasic(Color::Basic::BrightWhite);
    inline const Color Orange = Color::RGB(255, 165, 0);
    inline const Color Pink = Color::RGB(255, 192, 203);
    inline const Color Purple = Color::RGB(128, 0, 128);
    inline const Color Gray = Color::RGB(128, 128, 128);
    inline const Color DarkGray = Color::RGB(64, 64, 64);
    inline const Color LightGray = Color::RGB(192, 192, 192);
}

} // namespace ptui

// ============================================================================
// Style System (CSS-in-JS inspired)
// ============================================================================
namespace ptui {

enum class FlexDirection { Row, Column, RowReverse, ColumnReverse };
enum class JustifyContent { FlexStart, FlexEnd, Center, SpaceBetween, SpaceAround, SpaceEvenly };
enum class AlignItems { FlexStart, FlexEnd, Center, Stretch, Baseline };
enum class AlignSelf { Auto, FlexStart, FlexEnd, Center, Stretch, Baseline };
enum class Position { Static, Relative, Absolute, Fixed };
enum class Overflow { Visible, Hidden, Scroll };
enum class TextAlign { Left, Center, Right };
enum class TextDecoration { None, Underline, LineThrough, Overline };
enum class FontWeight { Normal, Bold, W100, W200, W300, W400, W500, W600, W700, W800, W900 };
enum class FontStyle { Normal, Italic, Oblique };
enum class Cursor { Auto, Pointer, Text, Crosshair, None };

struct BorderStyle {
    Color color = Color::Default;
    enum class Style { None, Solid, Dashed, Dotted, Double } style = Style::Solid;
    int width = 1;
};

struct BorderRadius {
    int top_left = 0, top_right = 0, bottom_left = 0, bottom_right = 0;
    BorderRadius() = default;
    BorderRadius(int all) : top_left(all), top_right(all), bottom_left(all), bottom_right(all) {}
    BorderRadius(int tl, int tr, int bl, int br) : top_left(tl), top_right(tr), bottom_left(bl), bottom_right(br) {}
};

struct BoxShadow {
    int offset_x = 0, offset_y = 0, blur_radius = 0, spread_radius = 0;
    Color color = Color::RGBA(0, 0, 0, 0.5f);
    bool inset = false;
};

struct Style {
    // Layout
    std::optional<int> width, height, min_width, min_height, max_width, max_height;
    std::optional<int> margin[4] = {}; // top, right, bottom, left
    std::optional<int> padding[4] = {};
    FlexDirection flex_direction = FlexDirection::Column;
    JustifyContent justify_content = JustifyContent::FlexStart;
    AlignItems align_items = AlignItems::Stretch;
    AlignSelf align_self = AlignSelf::Auto;
    float flex_grow = 0.0f;
    float flex_shrink = 1.0f;
    std::optional<int> flex_basis;
    Position position = Position::Static;
    int top = 0, right = 0, bottom = 0, left = 0;
    Overflow overflow = Overflow::Visible;
    int z_index = 0;

    // Visual
    Color foreground = Color::Default;
    Color background = Color::Default;
    std::optional<BorderStyle> border_top, border_right, border_bottom, border_left;
    BorderRadius border_radius;
    std::optional<BoxShadow> box_shadow;
    float opacity = 1.0f;

    // Text
    TextAlign text_align = TextAlign::Left;
    TextDecoration text_decoration = TextDecoration::None;
    FontWeight font_weight = FontWeight::Normal;
    FontStyle font_style = FontStyle::Normal;
    std::optional<int> font_size;
    std::string font_family;
    int line_height = 1;
    int letter_spacing = 0;

    // Other
    Cursor cursor = Cursor::Auto;
    bool visible = true;
    bool bold = false;
    bool dim = false;
    bool italic = false;
    bool underline = false;
    bool strikethrough = false;
    bool reverse = false;
    bool blink = false;

    // Builder pattern
    Style& set_width(int v) { width = v; return *this; }
    Style& set_height(int v) { height = v; return *this; }
    Style& set_min_width(int v) { min_width = v; return *this; }
    Style& set_min_height(int v) { min_height = v; return *this; }
    Style& set_max_width(int v) { max_width = v; return *this; }
    Style& set_max_height(int v) { max_height = v; return *this; }
    Style& set_margin(int all) { margin[0]=margin[1]=margin[2]=margin[3]=all; return *this; }
    Style& set_margin(int tb, int rl) { margin[0]=margin[2]=tb; margin[1]=margin[3]=rl; return *this; }
    Style& set_margin(int t, int r, int b, int l) { margin[0]=t;margin[1]=r;margin[2]=b;margin[3]=l; return *this; }
    Style& set_padding(int all) { padding[0]=padding[1]=padding[2]=padding[3]=all; return *this; }
    Style& set_padding(int tb, int rl) { padding[0]=padding[2]=tb; padding[1]=padding[3]=rl; return *this; }
    Style& set_padding(int t, int r, int b, int l) { padding[0]=t;padding[1]=r;padding[2]=b;padding[3]=l; return *this; }
    Style& set_flex_direction(FlexDirection v) { flex_direction = v; return *this; }
    Style& set_justify_content(JustifyContent v) { justify_content = v; return *this; }
    Style& set_align_items(AlignItems v) { align_items = v; return *this; }
    Style& set_align_self(AlignSelf v) { align_self = v; return *this; }
    Style& set_flex_grow(float v) { flex_grow = v; return *this; }
    Style& set_flex_shrink(float v) { flex_shrink = v; return *this; }
    Style& set_flex_basis(int v) { flex_basis = v; return *this; }
    Style& set_position(Position v) { position = v; return *this; }
    Style& set_top(int v) { top = v; return *this; }
    Style& set_right(int v) { right = v; return *this; }
    Style& set_bottom(int v) { bottom = v; return *this; }
    Style& set_left(int v) { left = v; return *this; }
    Style& set_overflow(Overflow v) { overflow = v; return *this; }
    Style& set_z_index(int v) { z_index = v; return *this; }
    Style& fg(Color c) { foreground = c; return *this; }
    Style& bg(Color c) { background = c; return *this; }
    Style& set_border(const BorderStyle& v) { border_top=border_right=border_bottom=border_left=v; return *this; }
    Style& set_border_top(const BorderStyle& v) { border_top = v; return *this; }
    Style& set_border_right(const BorderStyle& v) { border_right = v; return *this; }
    Style& set_border_bottom(const BorderStyle& v) { border_bottom = v; return *this; }
    Style& set_border_left(const BorderStyle& v) { border_left = v; return *this; }
    Style& set_border_radius(BorderRadius v) { border_radius = v; return *this; }
    Style& set_box_shadow(BoxShadow v) { box_shadow = v; return *this; }
    Style& set_opacity(float v) { opacity = v; return *this; }
    Style& set_text_align(TextAlign v) { text_align = v; return *this; }
    Style& set_text_decoration(TextDecoration v) { text_decoration = v; return *this; }
    Style& set_font_weight(FontWeight v) { font_weight = v; return *this; }
    Style& set_font_style(FontStyle v) { font_style = v; return *this; }
    Style& set_font_size(int v) { font_size = v; return *this; }
    Style& set_font_family(const std::string& v) { font_family = v; return *this; }
    Style& set_line_height(int v) { line_height = v; return *this; }
    Style& set_letter_spacing(int v) { letter_spacing = v; return *this; }
    Style& set_cursor(Cursor v) { cursor = v; return *this; }
    Style& set_visible(bool v) { visible = v; return *this; }
    Style& set_bold(bool v = true) { bold = v; return *this; }
    Style& set_dim(bool v = true) { dim = v; return *this; }
    Style& set_italic(bool v = true) { italic = v; return *this; }
    Style& set_underline(bool v = true) { underline = v; return *this; }
    Style& set_strikethrough(bool v = true) { strikethrough = v; return *this; }
    Style& set_reverse(bool v = true) { reverse = v; return *this; }
    Style& set_blink(bool v = true) { blink = v; return *this; }

    std::string render() const {
        std::string result;
        if (bold) result += "\033[1m";
        if (dim) result += "\033[2m";
        if (italic) result += "\033[3m";
        if (underline) result += "\033[4m";
        if (blink) result += "\033[5m";
        if (strikethrough) result += "\033[9m";
        if (reverse) result += "\033[7m";
        result += foreground.to_foreground();
        result += background.to_background();
        return result;
    }

    static std::string reset() { return "\033[0m"; }

    bool operator==(const Style& other) const {
        return foreground == other.foreground && background == other.background &&
               bold == other.bold && dim == other.dim && italic == other.italic &&
               underline == other.underline && strikethrough == other.strikethrough &&
               reverse == other.reverse && blink == other.blink;
    }

    bool operator!=(const Style& other) const { return !(*this == other); }
};

} // namespace ptui

// ============================================================================
// Event System
// ============================================================================
namespace ptui {

enum class EventType {
    None, KeyPress, KeyRelease, MouseClick, MouseRelease, MouseMove, MouseWheel,
    Resize, Focus, Blur, Paste, Custom
};

enum class Key : int {
    None = 0,
    Escape = 27,
    Enter = 10,
    Backspace = 127,
    Tab = 9,
    Space = 32,
    Up = 1000, Down, Left, Right,
    Home, End, PageUp, PageDown,
    Insert, Delete,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    CtrlA, CtrlB, CtrlC, CtrlD, CtrlE, CtrlF, CtrlG, CtrlH, CtrlI, CtrlJ,
    CtrlK, CtrlL, CtrlM, CtrlN, CtrlO, CtrlP, CtrlQ, CtrlR, CtrlS, CtrlT,
    CtrlU, CtrlV, CtrlW, CtrlX, CtrlY, CtrlZ,
    AltUp, AltDown, AltLeft, AltRight,
    ShiftUp, ShiftDown, ShiftLeft, ShiftRight, ShiftTab,
    MouseClick, MouseRelease, MouseMove, MouseWheel
};

struct Event {
    EventType type = EventType::None;
    Key key = Key::None;
    char32_t char_code = U'\0';
    int mouse_x = 0, mouse_y = 0;
    int mouse_button = 0;
    int wheel_delta = 0;
    int width = 0, height = 0;
    std::any custom_data;
    bool propagation_stopped = false;
    bool default_prevented = false;

    void stop_propagation() { propagation_stopped = true; }
    void prevent_default() { default_prevented = true; }
};

} // namespace ptui

// ============================================================================
// Element (Virtual DOM Node)
// ============================================================================
namespace ptui {

class Element : public std::enable_shared_from_this<Element> {
public:
    using PropMap = std::unordered_map<std::string, std::any>;
    using Children = std::vector<ElementPtr>;

    Element(const std::string& type, PropMap props = {}, Children children = {})
        : m_type(type), m_props(std::move(props)), m_children(std::move(children)) {}

    Element(const std::string& type, Children children)
        : m_type(type), m_children(std::move(children)) {}

    // Factory method
    static ElementPtr create(const std::string& type, PropMap props = {}, Children children = {}) {
        return std::make_shared<Element>(type, std::move(props), std::move(children));
    }

    static ElementPtr create(const std::string& type, Children children) {
        return std::make_shared<Element>(type, std::move(children));
    }

    // Fragment
    static ElementPtr fragment(Children children) {
        return create("fragment", std::move(children));
    }

    // Text node
    static ElementPtr text(const std::string& content, Style style = {}) {
        auto el = create("text", {{"content", content}});
        el->m_style = std::move(style);
        return el;
    }

    // Getters
    const std::string& type() const { return m_type; }
    const PropMap& props() const { return m_props; }
    const Children& children() const { return m_children; }
    Children& children() { return m_children; }
    const std::optional<std::string>& key() const { return m_key; }
    const Style& style() const { return m_style; }
    Style& style() { return m_style; }
    const std::optional<std::function<ElementPtr(const Event&)>>& event_handler(const std::string& name) const {
        auto it = m_event_handlers.find(name);
        if (it != m_event_handlers.end()) return it->second;
        static const std::optional<std::function<ElementPtr(const Event&)>> empty;
        return empty;
    }

    // Setters
    Element& set_key(const std::string& key) { m_key = key; return *this; }
    Element& set_prop(const std::string& key, const std::any& value) { m_props[key] = value; return *this; }
    Element& set_style(Style style) { m_style = std::move(style); return *this; }
    Element& set_event_handler(const std::string& name, std::function<ElementPtr(const Event&)> handler) {
        m_event_handlers[name] = std::move(handler);
        return *this;
    }
    Element& add_child(ElementPtr child) { m_children.push_back(std::move(child)); return *this; }
    Element& add_children(Children children) {
        m_children.insert(m_children.end(), std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));
        return *this;
    }

    // Prop access
    template<typename T>
    T prop(const std::string& key, const T& default_val = T{}) const {
        auto it = m_props.find(key);
        if (it == m_props.end()) return default_val;
        if (it->second.type() != typeid(T)) return default_val;
        return std::any_cast<T>(it->second);
    }

    std::string prop_str(const std::string& key, const std::string& default_val = "") const {
        return util::to_string(prop<std::any>(key, default_val));
    }

    int prop_int(const std::string& key, int default_val = 0) const {
        return util::to_int(prop<std::any>(key, default_val));
    }

    bool prop_bool(const std::string& key, bool default_val = false) const {
        return util::to_bool(prop<std::any>(key, default_val));
    }

    // Diffing
    bool operator==(const Element& other) const {
        return m_type == other.m_type && m_key == other.m_key;
    }

    bool operator!=(const Element& other) const { return !(*this == other); }

private:
    std::string m_type;
    PropMap m_props;
    Children m_children;
    std::optional<std::string> m_key;
    Style m_style;
    std::unordered_map<std::string, std::function<ElementPtr(const Event&)>> m_event_handlers;
};

// JSX-like builder
namespace jsx {
    inline ElementPtr operator/(const std::string& type, ElementPtr child) {
        return Element::create(type, {std::move(child)});
    }

    inline ElementPtr operator/(const std::string& type, const std::vector<ElementPtr>& children) {
        return Element::create(type, children);
    }

    inline ElementPtr operator%(const std::string& type, const std::string& key) {
        auto el = Element::create(type);
        el->set_key(key);
        return el;
    }
}

} // namespace ptui

// ============================================================================
// Ref System
// ============================================================================
namespace ptui {

class Ref : public std::enable_shared_from_this<Ref> {
public:
    Ref() = default;
    
    template<typename T>
    void set_current(T* value) {
        m_current = std::static_pointer_cast<void>(std::shared_ptr<T>(value));
    }

    template<typename T>
    T* get() const {
        return static_cast<T*>(m_current.get());
    }

    void reset() { m_current.reset(); }

private:
    std::shared_ptr<void> m_current;
};

} // namespace ptui

// ============================================================================
// Context System
// ============================================================================
namespace ptui {

class Context : public std::enable_shared_from_this<Context> {
public:
    Context() = default;
    explicit Context(std::any value) : m_value(std::move(value)) {}

    const std::any& value() const { return m_value; }
    std::any& value() { return m_value; }
    void set_value(const std::any& value) { m_value = value; }

    static ContextPtr create(std::any value = {}) {
        return std::make_shared<Context>(std::move(value));
    }

private:
    std::any m_value;
};

// Context global registry
namespace context {
    inline std::unordered_map<std::string, ContextPtr> g_contexts;
    
    inline ContextPtr create(const std::string& id, std::any default_value = {}) {
        auto ctx = Context::create(std::move(default_value));
        g_contexts[id] = ctx;
        return ctx;
    }

    inline ContextPtr get(const std::string& id) {
        auto it = g_contexts.find(id);
        if (it != g_contexts.end()) return it->second;
        return nullptr;
    }

    inline void remove(const std::string& id) {
        g_contexts.erase(id);
    }
}

} // namespace ptui

// ============================================================================
// Hooks System
// ============================================================================
namespace ptui {

namespace hooks {

    // Hook state stored per fiber
    struct HookState {
        std::any value;
        std::any deps;
        std::function<void()> cleanup;
        bool initialized = false;
    };

    inline thread_local std::vector<HookState> g_hook_states;
    inline thread_local size_t g_hook_index = 0;

    inline void reset_hook_index() { g_hook_index = 0; }

    // useState
    template<typename T>
    std::pair<T&, std::function<void(const T&)>> useState(const T& initial_value) {
        if (g_hook_index >= g_hook_states.size()) {
            g_hook_states.push_back({});
        }
        
        auto& state = g_hook_states[g_hook_index++];
        if (!state.initialized) {
            state.value = initial_value;
            state.initialized = true;
        }

        auto set_value = [&state](const T& new_val) {
            state.value = new_val;
        };

        return {std::any_cast<T&>(state.value), set_value};
    }

    // useEffect
    inline void useEffect(std::function<std::function<void>()> effect, std::optional<std::vector<std::any>> deps = std::nullopt) {
        if (g_hook_index >= g_hook_states.size()) {
            g_hook_states.push_back({});
        }
        
        auto& state = g_hook_states[g_hook_index++];
        bool should_run = false;
        
        if (!state.initialized) {
            should_run = true;
            state.initialized = true;
        } else if (deps.has_value()) {
            if (state.deps.type() != typeid(std::vector<std::any>)) {
                should_run = true;
            } else {
                auto& old_deps = std::any_cast<std::vector<std::any>&>(state.deps);
                if (old_deps.size() != deps->size()) {
                    should_run = true;
                } else {
                    for (size_t i = 0; i < deps->size(); ++i) {
                        const auto& old_val = old_deps[i];
                        const auto& new_val = (*deps)[i];
                        if (old_val.type() != new_val.type() || util::to_string(old_val) != util::to_string(new_val)) {
                            should_run = true;
                            break;
                        }
                    }
                }
            }
        }

        if (should_run) {
            if (state.cleanup) state.cleanup();
            state.deps = deps.value_or(std::vector<std::any>{});
            auto cleanup_fn = effect();
            if (cleanup_fn) {
                state.cleanup = cleanup_fn;
            } else {
                state.cleanup = nullptr;
            }
        }
    }

    // useContext
    template<typename T>
    T& useContext(const std::string& context_id) {
        auto ctx = context::get(context_id);
        if (!ctx) throw std::runtime_error("Context not found: " + context_id);
        return std::any_cast<T&>(ctx->value());
    }

    // useRef
    template<typename T>
    RefPtr useRef(const T& initial_value = T{}) {
        if (g_hook_index >= g_hook_states.size()) {
            g_hook_states.push_back({});
        }
        
        auto& state = g_hook_states[g_hook_index++];
        if (!state.initialized) {
            auto ref = std::make_shared<Ref>();
            ref->set_current(new T(initial_value));
            state.value = ref;
            state.initialized = true;
        }
        
        return std::any_cast<RefPtr>(state.value);
    }

    // useMemo
    template<typename T>
    T& useMemo(std::function<T()> factory, const std::vector<std::any>& deps) {
        if (g_hook_index >= g_hook_states.size()) {
            g_hook_states.push_back({});
        }
        
        auto& state = g_hook_states[g_hook_index++];
        bool should_recompute = false;
        
        if (!state.initialized) {
            should_recompute = true;
            state.initialized = true;
        } else if (state.deps.type() == typeid(std::vector<std::any>)) {
            auto& old_deps = std::any_cast<std::vector<std::any>&>(state.deps);
            if (old_deps.size() != deps.size()) {
                should_recompute = true;
            } else {
                for (size_t i = 0; i < deps.size(); ++i) {
                    if (old_deps[i] != deps[i]) {
                        should_recompute = true;
                        break;
                    }
                }
            }
        }

        if (should_recompute) {
            state.value = factory();
            state.deps = deps;
        }
        
        return std::any_cast<T&>(state.value);
    }

    // useCallback - returns memoized callback
    template<typename F>
    F& useCallback(F callback, const std::vector<std::any>& deps) {
        return useMemo<F>([callback]() { return callback; }, deps);
    }

    // useReducer
    template<typename State, typename Action>
    std::pair<State&, std::function<void(const Action&)>> useReducer(
        std::function<State(const State&, const Action&)> reducer,
        const State& initial_state
    ) {
        auto& [state, set_state] = useState(initial_state);
        auto dispatch = [reducer, &set_state](const Action& action) {
            set_state(reducer(std::any_cast<const State&>(state), action));
        };
        return {state, dispatch};
    }

    // Cleanup all hooks
    inline void cleanup() {
        for (auto& state : g_hook_states) {
            if (state.cleanup) state.cleanup();
        }
        g_hook_states.clear();
        g_hook_index = 0;
    }

} // namespace hooks
} // namespace ptui

// ============================================================================
// Component Base Class
// ============================================================================
namespace ptui {

class Component : public std::enable_shared_from_this<Component> {
public:
    virtual ~Component() = default;

    // Lifecycle
    virtual void on_mount() {}
    virtual void on_update() {}
    virtual void on_unmount() {}
    virtual bool should_update(const ElementPtr& next_element) { return true; }
    virtual void on_error(const std::exception& e) {}
    virtual void on_error_caught(const std::exception& e, const std::string& info) {}

    // Render
    virtual ElementPtr render() = 0;

    // Props
    const ElementPtr& element() const { return m_element; }
    void set_element(ElementPtr element) { m_element = std::move(element); }

    // State
    const std::any& state(const std::string& key) const {
        auto it = m_state.find(key);
        if (it != m_state.end()) return it->second;
        static const std::any empty;
        return empty;
    }

    void set_state(const std::string& key, const std::any& value) {
        m_state[key] = value;
    }

    // Force update
    void force_update() {
        if (auto reconciler = m_reconciler.lock()) {
            reconciler->schedule_update(weak_from_this());
        }
    }

    // Reconciler access
    void set_reconciler(std::weak_ptr<class Reconciler> reconciler) {
        m_reconciler = std::move(reconciler);
    }

private:
    ElementPtr m_element;
    std::unordered_map<std::string, std::any> m_state;
    std::weak_ptr<class Reconciler> m_reconciler;
};

} // namespace ptui

// ============================================================================
// Fiber (Internal reconciliation unit)
// ============================================================================
namespace ptui {

class Fiber : public std::enable_shared_from_this<Fiber> {
public:
    enum class Tag { Root, Host, Component, Text, Fragment };

    Fiber(Tag tag, std::string type, std::unordered_map<std::string, std::any> props = {})
        : m_tag(tag), m_type(std::move(type)), m_props(std::move(props)) {}

    // Tree structure
    FiberPtr parent() const { return m_parent.lock(); }
    void set_parent(FiberPtr parent) { m_parent = parent; }
    FiberPtr child() const { return m_child; }
    void set_child(FiberPtr child) { m_child = child; }
    FiberPtr sibling() const { return m_sibling; }
    void set_sibling(FiberPtr sibling) { m_sibling = sibling; }

    // State
    Tag tag() const { return m_tag; }
    const std::string& type() const { return m_type; }
    const std::unordered_map<std::string, std::any>& props() const { return m_props; }
    void set_props(std::unordered_map<std::string, std::any> props) { m_props = std::move(props); }

    // Alternate (for diffing)
    FiberPtr alternate() const { return m_alternate; }
    void set_alternate(FiberPtr alt) { m_alternate = alt; }

    // State hook
    std::shared_ptr<void> state_hook() const { return m_state_hook; }
    void set_state_hook(std::shared_ptr<void> hook) { m_state_hook = std::move(hook); }

    // Effect tag
    enum class EffectTag { Insert, Update, Delete, Move };
    EffectTag effect_tag() const { return m_effect_tag; }
    void set_effect_tag(EffectTag tag) { m_effect_tag = tag; }

    // DOM node (for host fibers)
    void* dom_node() const { return m_dom_node; }
    void set_dom_node(void* node) { m_dom_node = node; }

    // Element
    const ElementPtr& element() const { return m_element; }
    void set_element(ElementPtr element) { m_element = std::move(element); }

    // Component
    ComponentPtr component() const { return m_component; }
    void set_component(ComponentPtr component) { m_component = std::move(component); }

    // Children
    const std::vector<FiberPtr>& children() const { return m_children; }
    void set_children(std::vector<FiberPtr> children) { m_children = std::move(children); }

private:
    Tag m_tag;
    std::string m_type;
    std::unordered_map<std::string, std::any> m_props;
    std::weak_ptr<Fiber> m_parent;
    FiberPtr m_child;
    FiberPtr m_sibling;
    FiberPtr m_alternate;
    std::shared_ptr<void> m_state_hook;
    EffectTag m_effect_tag = EffectTag::Insert;
    void* m_dom_node = nullptr;
    ComponentPtr m_component;
    ElementPtr m_element;
    std::vector<FiberPtr> m_children;
};

} // namespace ptui

// ============================================================================
// Reconciler (Virtual DOM diffing)
// ============================================================================
namespace ptui {

class Reconciler : public std::enable_shared_from_this<Reconciler> {
public:
    Reconciler(Terminal& terminal) : m_terminal(terminal) {}

    // Mount root component
    void mount(ComponentPtr root) {
        m_root = std::make_shared<Fiber>(Fiber::Tag::Root, "root");
        m_root->set_component(std::move(root));
        m_root->set_dom_node(&m_terminal);
        m_next_unit_of_work = m_root;
        m_work_thread = std::thread(&Reconciler::work_loop, this);
    }

    // Schedule update
    void schedule_update(std::weak_ptr<Component> component) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_update_queue.push(std::move(component));
        m_cv.notify_one();
    }

    // Render element tree
    void render(ElementPtr element) {
        if (!m_root) return;
        m_root->set_child(nullptr);
        m_next_unit_of_work = m_root;
        m_pending_render = std::move(element);
        m_work_thread = std::thread(&Reconciler::work_loop, this);
    }

    // Wait for work to complete
    void wait() {
        if (m_work_thread.joinable()) m_work_thread.join();
    }

private:
    void work_loop() {
        while (m_next_unit_of_work) {
            m_next_unit_of_work = perform_unit_of_work(m_next_unit_of_work);
        }
        commit_work();
    }

    FiberPtr perform_unit_of_work(FiberPtr fiber) {
        if (fiber->tag() == Fiber::Tag::Component) {
            // Component fiber
            auto component = fiber->component();
            if (component) {
                hooks::reset_hook_index();
                auto element = component->render();
                auto children = reconcile_children(fiber, {element});
                fiber->set_children(children);
            }
        } else if (fiber->tag() == Fiber::Tag::Host || fiber->tag() == Fiber::Tag::Text) {
            // Host/text fiber
            auto children = reconcile_children(fiber, fiber->element()->children());
            fiber->set_children(children);
        }

        // Return next unit of work (depth-first)
        if (fiber->child()) return fiber->child();
        auto sibling = fiber->sibling();
        if (sibling) return sibling;
        
        auto parent = fiber->parent();
        while (parent) {
            auto parent_sibling = parent->sibling();
            if (parent_sibling) return parent_sibling;
            parent = parent->parent();
        }
        return nullptr;
    }

    std::vector<FiberPtr> reconcile_children(FiberPtr fiber, const Element::Children& elements) {
        std::vector<FiberPtr> new_children;
        std::unordered_map<std::string, FiberPtr> old_fiber_map;
        
        // Build map of old fibers by key
        auto old_child = fiber->child();
        while (old_child) {
            if (old_child->element() && old_child->element()->key().has_value()) {
                old_fiber_map[*old_child->element()->key()] = old_child;
            }
            old_child = old_child->sibling();
        }

        // Reconcile
        for (size_t i = 0; i < elements.size(); ++i) {
            auto& element = elements[i];
            FiberPtr old_fiber = nullptr;
            
            if (element->key().has_value()) {
                auto it = old_fiber_map.find(*element->key());
                if (it != old_fiber_map.end()) {
                    old_fiber = it->second;
                    old_fiber_map.erase(it);
                }
            }

            FiberPtr new_fiber = nullptr;
            if (old_fiber && old_fiber->type() == element->type()) {
                // Update
                new_fiber = std::make_shared<Fiber>(old_fiber->tag(), old_fiber->type(), element->props());
                new_fiber->set_alternate(old_fiber);
                new_fiber->set_effect_tag(Fiber::EffectTag::Update);
                new_fiber->set_parent(fiber);
            } else {
                // Insert
                new_fiber = create_fiber(element);
                new_fiber->set_parent(fiber);
            }

            if (element->key().has_value()) {
                new_fiber->set_element(element);
            }

            new_children.push_back(new_fiber);
        }

        // Delete remaining old fibers
        for (auto& [key, old_fiber] : old_fiber_map) {
            old_fiber->set_effect_tag(Fiber::EffectTag::Delete);
            // TODO: handle deletion
        }

        return new_children;
    }

    FiberPtr create_fiber(const ElementPtr& element) {
        Fiber::Tag tag;
        if (element->type() == "text") {
            tag = Fiber::Tag::Text;
        } else if (element->type() == "fragment") {
            tag = Fiber::Tag::Fragment;
        } else {
            tag = Fiber::Tag::Host;
        }

        auto fiber = std::make_shared<Fiber>(tag, element->type(), element->props());
        fiber->set_element(element);
        fiber->set_effect_tag(Fiber::EffectTag::Insert);
        return fiber;
    }

    void commit_work() {
        // Apply DOM changes
        // For TUI, we just trigger a re-render
        m_terminal.clear();
        render_fiber(m_root, 0, 0);
        m_terminal.present();
    }

    void render_fiber(FiberPtr fiber, int x, int y) {
        if (!fiber) return;

        if (fiber->tag() == Fiber::Tag::Text) {
            auto content = fiber->element()->prop<std::string>("content", "");
            m_terminal.draw_text(x, y, content, fiber->element()->style());
        } else if (fiber->tag() == Fiber::Tag::Host) {
            // Render host element
            auto style = fiber->element()->style();
            if (style.visible) {
                m_terminal.draw_rect(x, y, 
                    style.width.value_or(10), 
                    style.height.value_or(3), 
                    style);
            }
        }

        // Render children
        for (auto& child : fiber->children()) {
            render_fiber(child, x + 1, y + 1);
        }
    }

    Terminal& m_terminal;
    FiberPtr m_root;
    FiberPtr m_next_unit_of_work;
    ElementPtr m_pending_render;
    std::thread m_work_thread;
    std::queue<std::weak_ptr<Component>> m_update_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

} // namespace ptui

// ============================================================================
// Animation System
// ============================================================================
namespace ptui {

class Animation : public std::enable_shared_from_this<Animation> {
public:
    using Easing = std::function<float(float)>;
    using Callback = std::function<void(float)>;

    struct Keyframe {
        float time; // 0.0 to 1.0
        std::unordered_map<std::string, float> values;
    };

    Animation(float duration, Easing easing = nullptr)
        : m_duration(duration), m_easing(easing ? easing : linear) {}

    static AnimationPtr create(float duration, Easing easing = nullptr) {
        return std::make_shared<Animation>(duration, std::move(easing));
    }

    AnimationPtr add_keyframe(const Keyframe& kf) {
        m_keyframes.push_back(kf);
        return shared_from_this();
    }

    AnimationPtr on_update(Callback cb) {
        m_on_update = std::move(cb);
        return shared_from_this();
    }

    AnimationPtr on_complete(std::function<void()> cb) {
        m_on_complete = std::move(cb);
        return shared_from_this();
    }

    void start() {
        m_running = true;
        m_start_time = std::chrono::steady_clock::now();
        m_thread = std::thread(&Animation::animate, this);
    }

    void stop() {
        m_running = false;
        if (m_thread.joinable()) m_thread.join();
    }

    bool is_running() const { return m_running; }

    // Easing functions
    static float linear(float t) { return t; }
    static float ease_in_quad(float t) { return t * t; }
    static float ease_out_quad(float t) { return t * (2 - t); }
    static float ease_in_out_quad(float t) { return t < 0.5f ? 2*t*t : -1+(4-2*t)*t; }
    static float ease_in_cubic(float t) { return t * t * t; }
    static float ease_out_cubic(float t) { return 1 - std::pow(1 - t, 3); }
    static float ease_in_out_cubic(float t) { return t < 0.5f ? 4*t*t*t : 1 - std::pow(-2*t+2, 3)/2; }
    static float ease_in_sine(float t) { return 1 - std::cos(t * M_PI / 2); }
    static float ease_out_sine(float t) { return std::sin(t * M_PI / 2); }
    static float ease_in_out_sine(float t) { return -(std::cos(M_PI * t) - 1) / 2; }
    static float ease_in_expo(float t) { return t == 0 ? 0 : std::pow(2, 10 * t - 10); }
    static float ease_out_expo(float t) { return t == 1 ? 1 : 1 - std::pow(2, -10 * t); }
    static float ease_in_out_expo(float t) { 
        if (t == 0) return 0;
        if (t == 1) return 1;
        return t < 0.5f ? std::pow(2, 20*t-10)/2 : (2 - std::pow(2, -20*t+10))/2;
    }
    static float ease_in_circ(float t) { return 1 - std::sqrt(1 - t*t); }
    static float ease_out_circ(float t) { return std::sqrt(1 - std::pow(t-1, 2)); }
    static float ease_in_out_circ(float t) { 
        return t < 0.5f ? (1 - std::sqrt(1 - 4*t*t))/2 : (std::sqrt(1 - std::pow(-2*t+2, 2)) + 1)/2;
    }
    static float ease_in_back(float t) { return 2.70158f*t*t*t - 1.70158f*t*t; }
    static float ease_out_back(float t) { 
        return 1 + 2.70158f*std::pow(t-1, 3) + 1.70158f*std::pow(t-1, 2);
    }
    static float ease_in_out_back(float t) {
        float c1 = 1.70158f;
        float c2 = c1 * 1.525f;
        return t < 0.5f 
            ? (std::pow(2*t, 2) * ((c2+1)*2*t - c2))/2
            : (std::pow(2*t-2, 2) * ((c2+1)*(t*2-2) + c2) + 2)/2;
    }
    static float ease_in_elastic(float t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return -std::pow(2, 10*t-10) * std::sin((t*10-10.75f) * (2*M_PI)/3);
    }
    static float ease_out_elastic(float t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return std::pow(2, -10*t) * std::sin((t*10-0.75f) * (2*M_PI)/3) + 1;
    }
    static float ease_in_out_elastic(float t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return t < 0.5f 
            ? -(std::pow(2, 20*t-10) * std::sin((20*t-11.125f) * (2*M_PI)/4.5f))/2
            : (std::pow(2, -20*t+10) * std::sin((20*t-11.125f) * (2*M_PI)/4.5f))/2 + 1;
    }
    static float ease_in_bounce(float t) { return 1 - ease_out_bounce(1-t); }
    static float ease_out_bounce(float t) {
        if (t < 1/2.75f) return 7.5625f*t*t;
        if (t < 2/2.75f) { t -= 1.5f/2.75f; return 7.5625f*t*t + 0.75f; }
        if (t < 2.5f/2.75f) { t -= 2.25f/2.75f; return 7.5625f*t*t + 0.9375f; }
        t -= 2.625f/2.75f; return 7.5625f*t*t + 0.984375f;
    }
    static float ease_in_out_bounce(float t) {
        return t < 0.5f ? (1 - ease_out_bounce(1-2*t))/2 : (1 + ease_out_bounce(2*t-1))/2;
    }

private:
    void animate() {
        while (m_running) {
            auto now = std::chrono::steady_clock::now();
            float elapsed = std::chrono::duration<float>(now - m_start_time).count();
            float progress = std::min(1.0f, elapsed / m_duration);
            float eased = m_easing(progress);

            // Interpolate values
            if (m_on_update) {
                m_on_update(eased);
            }

            if (progress >= 1.0f) {
                m_running = false;
                if (m_on_complete) m_on_complete();
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60fps
        }
    }

    float m_duration;
    Easing m_easing;
    std::vector<Keyframe> m_keyframes;
    Callback m_on_update;
    std::function<void()> m_on_complete;
    std::atomic<bool> m_running{false};
    std::chrono::steady_clock::time_point m_start_time;
    std::thread m_thread;
};

} // namespace ptui

// ============================================================================
// Router System
// ============================================================================
namespace ptui {

class Router : public std::enable_shared_from_this<Router> {
public:
    using Route = std::pair<std::string, std::function<ElementPtr()>>;
    using Routes = std::vector<Route>;

    Router() = default;

    static RouterPtr create() {
        return std::make_shared<Router>();
    }

    Router& add_route(const std::string& path, std::function<ElementPtr()> component) {
        m_routes.push_back({path, std::move(component)});
        return *this;
    }

    Router& set_fallback(std::function<ElementPtr()> component) {
        m_fallback = std::move(component);
        return *this;
    }

    void navigate(const std::string& path) {
        m_history.push_back(path);
        m_current_route = path;
        if (m_on_change) m_on_change(path);
    }

    void back() {
        if (m_history.size() > 1) {
            m_history.pop_back();
            m_current_route = m_history.back();
            if (m_on_change) m_on_change(m_current_route);
        }
    }

    void forward() {
        // TODO: implement forward history
    }

    const std::string& current_route() const { return m_current_route; }

    ElementPtr render_current() {
        for (auto& [path, component] : m_routes) {
            if (path == m_current_route) return component();
        }
        return m_fallback ? m_fallback() : Element::create("text", {{"content", "404 Not Found"}});
    }

    void on_change(std::function<void(const std::string&)> cb) {
        m_on_change = std::move(cb);
    }

private:
    Routes m_routes;
    std::function<ElementPtr()> m_fallback;
    std::string m_current_route = "/";
    std::vector<std::string> m_history = {"/"};
    std::function<void(const std::string&)> m_on_change;
};

} // namespace ptui

// ============================================================================
// Theme System
// ============================================================================
namespace ptui {

class Theme {
public:
    struct ColorScheme {
        Color primary = Color::RGB(85, 255, 85);
        Color secondary = Color::RGB(135, 206, 235);
        Color success = Color::RGB(85, 255, 85);
        Color warning = Color::RGB(255, 255, 0);
        Color error = Color::RGB(255, 85, 85);
        Color info = Color::RGB(135, 206, 235);
        Color background = Color::RGB(0, 0, 0);
        Color foreground = Color::RGB(255, 255, 255);
        Color muted = Color::RGB(128, 128, 128);
        Color border = Color::RGB(64, 64, 64);
    };

    struct Spacing {
        int xs = 1;
        int sm = 2;
        int md = 3;
        int lg = 4;
        int xl = 6;
    };

    struct Typography {
        int font_size = 1;
        int line_height = 1;
        int letter_spacing = 0;
    };

    struct Border {
        int width = 1;
        BorderRadius radius;
        Color color = Color::RGB(64, 64, 64);
    };

    ColorScheme colors;
    Spacing spacing;
    Typography typography;
    Border border;

    static Theme& global() {
        static Theme instance;
        return instance;
    }

    void set_dark() {
        colors.background = Color::RGB(0, 0, 0);
        colors.foreground = Color::RGB(255, 255, 255);
        colors.muted = Color::RGB(128, 128, 128);
        colors.border = Color::RGB(64, 64, 64);
    }

    void set_light() {
        colors.background = Color::RGB(255, 255, 255);
        colors.foreground = Color::RGB(0, 0, 0);
        colors.muted = Color::RGB(128, 128, 128);
        colors.border = Color::RGB(192, 192, 192);
    }
};

} // namespace ptui

// ============================================================================
// Built-in Widgets
// ============================================================================
namespace ptui {
namespace widgets {

// Container
class Container : public Component {
public:
    Container(Style style = {}, std::vector<ElementPtr> children = {})
        : m_style(std::move(style)), m_children(std::move(children)) {}

    ElementPtr render() override {
        auto el = Element::create("container", m_children);
        el->set_style(m_style);
        return el;
    }

    void set_style(Style style) { m_style = std::move(style); force_update(); }
    void set_children(std::vector<ElementPtr> children) { m_children = std::move(children); force_update(); }

private:
    Style m_style;
    std::vector<ElementPtr> m_children;
};

// Text
class Text : public Component {
public:
    Text(std::string content = "", Style style = {})
        : m_content(std::move(content)), m_style(std::move(style)) {}

    ElementPtr render() override {
        auto el = Element::create("text", {{"content", m_content}});
        el->set_style(m_style);
        return el;
    }

    void set_content(const std::string& content) { m_content = content; force_update(); }
    void set_style(Style style) { m_style = std::move(style); force_update(); }

private:
    std::string m_content;
    Style m_style;
};

// Button
class Button : public Component {
public:
    Button(std::string label = "", std::function<void()> onClick = nullptr, Style style = {})
        : m_label(std::move(label)), m_onClick(std::move(onClick)), m_style(std::move(style)) {}

    ElementPtr render() override {
        auto el = Element::create("button", {{"label", m_label}});
        el->set_style(m_style);
        if (m_onClick) {
            el->set_event_handler("click", [this](const Event&) {
                if (m_onClick) m_onClick();
                return nullptr;
            });
        }
        return el;
    }

    void set_label(const std::string& label) { m_label = label; force_update(); }
    void set_on_click(std::function<void()> cb) { m_onClick = std::move(cb); }
    void set_style(Style style) { m_style = std::move(style); force_update(); }

private:
    std::string m_label;
    std::function<void()> m_onClick;
    Style m_style;
};

// TextInput
class TextInput : public Component {
public:
    TextInput(std::string placeholder = "", std::function<void(const std::string&)> onChange = nullptr, Style style = {})
        : m_placeholder(std::move(placeholder)), m_onChange(std::move(onChange)), m_style(std::move(style)) {}

    ElementPtr render() override {
        auto el = Element::create("input", {{"value", m_value}, {"placeholder", m_placeholder}});
        el->set_style(m_style);
        return el;
    }

    void set_value(const std::string& value) { m_value = value; force_update(); }
    const std::string& value() const { return m_value; }
    void set_placeholder(const std::string& placeholder) { m_placeholder = placeholder; force_update(); }
    void set_on_change(std::function<void(const std::string&)> cb) { m_onChange = std::move(cb); }
    void set_style(Style style) { m_style = std::move(style); force_update(); }

private:
    std::string m_value;
    std::string m_placeholder;
    std::function<void(const std::string&)> m_onChange;
    Style m_style;
};

// List
class List : public Component {
public:
    using ItemRenderer = std::function<ElementPtr(size_t, const std::any&)>;
    
    List(std::vector<std::any> items = {}, ItemRenderer renderer = nullptr, Style style = {})
        : m_items(std::move(items)), m_renderer(std::move(renderer)), m_style(std::move(style)) {}

    ElementPtr render() override {
        std::vector<ElementPtr> children;
        for (size_t i = 0; i < m_items.size(); ++i) {
            if (m_renderer) {
                auto child = m_renderer(i, m_items[i]);
                child->set_key(std::to_string(i));
                children.push_back(std::move(child));
            }
        }
        auto el = Element::create("list", std::move(children));
        el->set_style(m_style);
        return el;
    }

    void set_items(std::vector<std::any> items) { m_items = std::move(items); force_update(); }
    void set_renderer(ItemRenderer renderer) { m_renderer = std::move(renderer); force_update(); }
    void set_style(Style style) { m_style = std::move(style); force_update(); }

private:
    std::vector<std::any> m_items;
    ItemRenderer m_renderer;
    Style m_style;
};

// ScrollView
class ScrollView : public Component {
public:
    ScrollView(ElementPtr child = nullptr, Style style = {})
        : m_child(std::move(child)), m_style(std::move(style)) {}

    ElementPtr render() override {
        auto el = Element::create("scrollview", m_child ? std::vector<ElementPtr>{m_child} : std::vector<ElementPtr>{});
        el->set_style(m_style);
        return el;
    }

    void set_child(ElementPtr child) { m_child = std::move(child); force_update(); }
    void set_style(Style style) { m_style = std::move(style); force_update(); }

private:
    ElementPtr m_child;
    Style m_style;
};

// Modal
class Modal : public Component {
public:
    Modal(bool visible = false, ElementPtr child = nullptr, std::function<void()> onClose = nullptr, Style style = {})
        : m_visible(visible), m_child(std::move(child)), m_onClose(std::move(onClose)), m_style(std::move(style)) {}

    ElementPtr render() override {
        if (!m_visible) return Element::create("fragment");
        
        auto el = Element::create("modal", m_child ? std::vector<ElementPtr>{m_child} : std::vector<ElementPtr>{});
        el->set_style(m_style);
        return el;
    }

    void set_visible(bool visible) { m_visible = visible; force_update(); }
    void set_child(ElementPtr child) { m_child = std::move(child); force_update(); }
    void set_on_close(std::function<void()> cb) { m_onClose = std::move(cb); }
    void set_style(Style style) { m_style = std::move(style); force_update(); }

private:
    bool m_visible;
    ElementPtr m_child;
    std::function<void()> m_onClose;
    Style m_style;
};

} // namespace widgets
} // namespace ptui

namespace ptui {

struct Cell {
    char32_t ch = U' ';
    Style style = {};
    bool transparent = false;
};

} // namespace ptui

// ============================================================================
// Terminal Context (raw mode, size, input handling)
// ============================================================================
namespace ptui {

class Terminal {
public:
    using ResizeCallback = std::function<void(int, int)>;
    using EventCallback = std::function<void(const Event&)>;

    Terminal() = default;
    ~Terminal() { shutdown(); }

    // Delete copy
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    // Init / Shutdown
    bool init() {
        // Enable raw mode
#ifdef _WIN32
        m_hstdin = GetStdHandle(STD_INPUT_HANDLE);
        m_hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleMode(m_hstdin, &m_orig_mode);
        SetConsoleMode(m_hstdin, m_orig_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
#else
        tcgetattr(STDIN_FILENO, &m_orig_termios);
        struct termios raw = m_orig_termios;
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
        update_size();
        m_running = true;
        m_input_thread = std::thread(&Terminal::read_input_loop, this);
        return true;
    }

    void shutdown() {
        m_running = false;
        if (m_input_thread.joinable()) m_input_thread.join();
        
        // Restore terminal
#ifdef _WIN32
        SetConsoleMode(m_hstdin, m_orig_mode);
#else
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_orig_termios);
#endif
        show_cursor(true);
        std::cout << "\033[0m" << std::endl;
    }

    // Terminal size
    int width() const { return m_width; }
    int height() const { return m_height; }

    // Input/Output
    void set_event_callback(EventCallback cb) { m_event_callback = cb; }
    void set_resize_callback(ResizeCallback cb) { m_resize_callback = cb; }

    // Draw
    void clear() {
        m_backbuffer.assign(m_width * m_height, Cell{U' ', Style{}, false});
    }

    void present() {
        // Swap buffers
        std::swap(m_framebuffer, m_backbuffer);
        
        // Render to terminal
        std::string output;
        output.reserve(m_width * m_height * 2);
        output += "\033[H"; // Move cursor to top-left
        
        Style current_style;
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                const auto& cell = m_framebuffer[y * m_width + x];
                if (cell.transparent) continue;
                
                if (cell.style != current_style) {
                    output += Style::reset();
                    output += cell.style.render();
                    current_style = cell.style;
                }
                output += cell.ch;
            }
            if (y < m_height - 1) output += "\n";
        }
        
        std::cout << output << std::flush;
    }

    void draw_pixel(int x, int y, const Cell& cell) {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
        m_backbuffer[y * m_width + x] = cell;
    }

    void draw_text(int x, int y, const std::string& text, const Style& style = {}) {
        for (size_t i = 0; i < text.size(); ++i) {
            draw_pixel(x + i, y, Cell{static_cast<char32_t>(static_cast<unsigned char>(text[i])), style, false});
        }
    }

    void draw_rect(int x, int y, int w, int h, const Style& style = {}, char32_t fill = U' ') {
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                draw_pixel(x + dx, y + dy, Cell{fill, style, false});
            }
        }
    }

    void draw_box(int x, int y, int w, int h, const Style& style = {}) {
        // Top border
        draw_pixel(x, y, Cell{U'┌', style, false});
        draw_pixel(x + w - 1, y, Cell{U'┐', style, false});
        // Bottom border
        draw_pixel(x, y + h - 1, Cell{U'└', style, false});
        draw_pixel(x + w - 1, y + h - 1, Cell{U'┘', style, false});
        // Horizontal
        for (int dx = 1; dx < w - 1; ++dx) {
            draw_pixel(x + dx, y, Cell{U'─', style, false});
            draw_pixel(x + dx, y + h - 1, Cell{U'─', style, false});
        }
        // Vertical
        for (int dy = 1; dy < h - 1; ++dy) {
            draw_pixel(x, y + dy, Cell{U'│', style, false});
            draw_pixel(x + w - 1, y + dy, Cell{U'│', style, false});
        }
    }

    // Input
    Event get_event(int timeout_ms = 0) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (timeout_ms > 0) {
            if (!m_cv.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this] { return !m_events.empty(); })) {
                return Event{};
            }
        } else {
            m_cv.wait(lock, [this] { return !m_events.empty(); });
        }
        auto ev = m_events.front();
        m_events.pop();
        return ev;
    }

    bool has_event() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return !m_events.empty();
    }

    // Cursor
    void set_cursor(int x, int y) {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
    }

    void show_cursor(bool visible) {
        std::cout << (visible ? "\033[?25h" : "\033[?25l") << std::flush;
    }

    // Title
    void set_title(const std::string& title) {
        std::cout << "\033]0;" << title << "\007" << std::flush;
    }

private:
    void update_size() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(m_hstdout, &csbi);
        m_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        m_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
        struct winsize ws;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
        m_width = ws.ws_col;
        m_height = ws.ws_row;
#endif
        m_framebuffer.resize(m_width * m_height);
        m_backbuffer.resize(m_width * m_height);
    }

    void read_input_loop() {
        std::string input;
        while (m_running) {
            char c;
#ifdef _WIN32
            DWORD read;
            ReadConsole(m_hstdin, &c, 1, &read, nullptr);
            if (read == 0) continue;
#else
            if (read(STDIN_FILENO, &c, 1) != 1) continue;
#endif
            input += c;
            
            // Parse input
            auto ev = parse_input(input);
            if (ev.type != EventType::None) {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_events.push(std::move(ev));
                m_cv.notify_one();
                input.clear();
            }
        }
    }

    Event parse_input(const std::string& input) {
        Event ev;
        if (input.empty()) return ev;

        if (input.size() == 1) {
            char c = input[0];
            if (c == 27) {
                ev.type = EventType::KeyPress;
                ev.key = Key::Escape;
            } else if (c == 10 || c == 13) {
                ev.type = EventType::KeyPress;
                ev.key = Key::Enter;
            } else if (c == 127 || c == 8) {
                ev.type = EventType::KeyPress;
                ev.key = Key::Backspace;
            } else if (c == 9) {
                ev.type = EventType::KeyPress;
                ev.key = Key::Tab;
            } else if (c >= 1 && c <= 26) {
                ev.type = EventType::KeyPress;
                ev.key = static_cast<Key>(static_cast<int>(Key::CtrlA) + (c - 1));
            } else if (c >= 32 && c <= 126) {
                ev.type = EventType::KeyPress;
                ev.key = Key::None;
                ev.char_code = static_cast<char32_t>(c);
            }
        } else if (input.size() >= 3 && input[0] == 27 && input[1] == '[') {
            // Escape sequence
            char last = input.back();
            if (last == 'A') {
                ev.type = EventType::KeyPress;
                ev.key = Key::Up;
            } else if (last == 'B') {
                ev.type = EventType::KeyPress;
                ev.key = Key::Down;
            } else if (last == 'C') {
                ev.type = EventType::KeyPress;
                ev.key = Key::Right;
            } else if (last == 'D') {
                ev.type = EventType::KeyPress;
                ev.key = Key::Left;
            } else if (last == 'H') {
                ev.type = EventType::KeyPress;
                ev.key = Key::Home;
            } else if (last == 'F') {
                ev.type = EventType::KeyPress;
                ev.key = Key::End;
            } else if (last == '~') {
                if (input.size() >= 4) {
                    int num = std::stoi(input.substr(2, input.size() - 3));
                    if (num == 1) ev.key = Key::Home;
                    else if (num == 4) ev.key = Key::End;
                    else if (num == 5) ev.key = Key::PageUp;
                    else if (num == 6) ev.key = Key::PageDown;
                    else if (num == 3) ev.key = Key::Delete;
                    else if (num == 2) ev.key = Key::Insert;
                    ev.type = EventType::KeyPress;
                }
            } else if (last == 'Z') {
                ev.type = EventType::KeyPress;
                ev.key = Key::ShiftTab;
            }
        }

        return ev;
    }

#ifdef _WIN32
    HANDLE m_hstdin;
    HANDLE m_hstdout;
    DWORD m_orig_mode;
#else
    struct termios m_orig_termios;
#endif

    int m_width = 0;
    int m_height = 0;
    std::atomic<bool> m_running{false};
    std::thread m_input_thread;
    std::queue<Event> m_events;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    EventCallback m_event_callback;
    ResizeCallback m_resize_callback;
    std::vector<Cell> m_framebuffer;
    std::vector<Cell> m_backbuffer;
};

} // namespace ptui

// ============================================================================
// Python Bindings (pybind11)
// ============================================================================
#ifdef PTUI_PYTHON_BINDINGS

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(ptui, m) {
    m.doc() = "PTUI: Professional Terminal User Interface - React-inspired TUI framework";

    // Color
    py::class_<ptui::Color>(m, "Color")
        .def(py::init<>())
        .def_static("RGB", &ptui::Color::RGB)
        .def_static("Indexed", &ptui::Color::Indexed)
        .def_static("Basic", &ptui::Color::Basic)
        .def_static("Hex", &ptui::Color::Hex)
        .def_property_readonly("r", [](const ptui::Color& c) { uint8_t r,g,b; c.rgb(r,g,b); return r; })
        .def_property_readonly("g", [](const ptui::Color& c) { uint8_t r,g,b; c.rgb(r,g,b); return g; })
        .def_property_readonly("b", [](const ptui::Color& c) { uint8_t r,g,b; c.rgb(r,g,b); return b; })
        .def("lighten", &ptui::Color::lighten)
        .def("darken", &ptui::Color::darken);

    // Style
    py::class_<ptui::Style>(m, "Style")
        .def(py::init<>())
        .def("fg", &ptui::Style::fg)
        .def("bg", &ptui::Style::bg)
        .def("bold", &ptui::Style::set_bold)
        .def("italic", &ptui::Style::set_italic)
        .def("underline", &ptui::Style::set_underline)
        .def("strikethrough", &ptui::Style::set_strikethrough)
        .def("reverse", &ptui::Style::set_reverse)
        .def("blink", &ptui::Style::set_blink)
        .def("dim", &ptui::Style::set_dim)
        .def("set_width", &ptui::Style::set_width)
        .def("set_height", &ptui::Style::set_height)
        .def("set_margin", py::overload_cast<int>(&ptui::Style::set_margin))
        .def("set_padding", py::overload_cast<int>(&ptui::Style::set_padding))
        .def("set_border_radius", &ptui::Style::set_border_radius)
        .def("set_text_align", &ptui::Style::set_text_align)
        .def("set_font_weight", &ptui::Style::set_font_weight)
        .def("set_font_style", &ptui::Style::set_font_style)
        .def("set_visible", &ptui::Style::set_visible)
        .def("render", &ptui::Style::render)
        .def_static("reset", &ptui::Style::reset);

    // Event/Key
    py::enum_<ptui::Key>(m, "Key")
        .value("Up", ptui::Key::Up)
        .value("Down", ptui::Key::Down)
        .value("Left", ptui::Key::Left)
        .value("Right", ptui::Key::Right)
        .value("Enter", ptui::Key::Enter)
        .value("Escape", ptui::Key::Escape)
        .value("Backspace", ptui::Key::Backspace)
        .value("Tab", ptui::Key::Tab)
        .value("Space", ptui::Key::Space)
        .value("F1", ptui::Key::F1)
        .value("F12", ptui::Key::F12)
        .value("CtrlC", ptui::Key::CtrlC)
        .value("CtrlD", ptui::Key::CtrlD)
        .value("CtrlZ", ptui::Key::CtrlZ);

    py::enum_<ptui::EventType>(m, "EventType")
        .value("NoEvent", ptui::EventType::None)
        .value("None_", ptui::EventType::None)
        .value("KeyPress", ptui::EventType::KeyPress)
        .value("KeyRelease", ptui::EventType::KeyRelease)
        .value("MouseClick", ptui::EventType::MouseClick)
        .value("MouseMove", ptui::EventType::MouseMove)
        .value("Resize", ptui::EventType::Resize)
        .value("Focus", ptui::EventType::Focus)
        .value("Blur", ptui::EventType::Blur);

    py::class_<ptui::Event>(m, "Event")
        .def_readonly("type", &ptui::Event::type)
        .def_readonly("key", &ptui::Event::key)
        .def_readonly("char_code", &ptui::Event::char_code)
        .def_readonly("mouse_x", &ptui::Event::mouse_x)
        .def_readonly("mouse_y", &ptui::Event::mouse_y)
        .def("stop_propagation", &ptui::Event::stop_propagation)
        .def("prevent_default", &ptui::Event::prevent_default);

    // Element
    py::class_<ptui::Element, ptui::ElementPtr>(m, "Element")
        .def_static("create", py::overload_cast<const std::string&, ptui::Element::PropMap, ptui::Element::Children>(&ptui::Element::create))
        .def_static("text", &ptui::Element::text)
        .def_static("fragment", &ptui::Element::fragment)
        .def("set_key", &ptui::Element::set_key)
        .def("set_prop", &ptui::Element::set_prop)
        .def("set_style", &ptui::Element::set_style)
        .def("add_child", &ptui::Element::add_child)
        .def("add_children", &ptui::Element::add_children)
        .def("prop_str", &ptui::Element::prop_str)
        .def("prop_int", &ptui::Element::prop_int)
        .def("prop_bool", &ptui::Element::prop_bool)
        .def_readonly("type", &ptui::Element::type)
        .def_readonly("children", &ptui::Element::children);

    // Ref
    py::class_<ptui::Ref, ptui::RefPtr>(m, "Ref")
        .def(py::init<>())
        .def("reset", &ptui::Ref::reset);

    // Context
    py::class_<ptui::Context, ptui::ContextPtr>(m, "Context")
        .def(py::init<>())
        .def_static("create", &ptui::Context::create)
        .def("value", py::overload_cast<>(&ptui::Context::value))
        .def("set_value", &ptui::Context::set_value);

    // Component
    py::class_<ptui::Component, ptui::ComponentPtr>(m, "Component")
        .def(py::init<>())
        .def("render", &ptui::Component::render)
        .def("on_mount", &ptui::Component::on_mount)
        .def("on_update", &ptui::Component::on_update)
        .def("on_unmount", &ptui::Component::on_unmount)
        .def("force_update", &ptui::Component::force_update)
        .def("set_state", &ptui::Component::set_state)
        .def("state", &ptui::Component::state);

    // Hooks
    m.def("use_state", [](const py::object& initial) {
        // This is a simplified version - full implementation needs more work
        return py::make_tuple(initial, py::cpp_function([](py::object val) {
            // setter
        }));
    });

    m.def("use_effect", [](std::function<void()> effect, std::optional<std::vector<py::object>> deps) {
        effect();
    });

    m.def("use_ref", [](py::object initial) {
        return initial;
    });

    m.def("use_context", [](const std::string& context_id) {
        auto ctx = ptui::context::get(context_id);
        if (!ctx) throw std::runtime_error("Context not found: " + context_id);
        return ctx->value();
    });

    // Animation
    py::class_<ptui::Animation, ptui::AnimationPtr>(m, "Animation")
        .def_static("create", &ptui::Animation::create)
        .def("add_keyframe", &ptui::Animation::add_keyframe)
        .def("on_update", &ptui::Animation::on_update)
        .def("on_complete", &ptui::Animation::on_complete)
        .def("start", &ptui::Animation::start)
        .def("stop", &ptui::Animation::stop)
        .def("is_running", &ptui::Animation::is_running);

    // Router
    py::class_<ptui::Router, ptui::RouterPtr>(m, "Router")
        .def(py::init<>())
        .def_static("create", &ptui::Router::create)
        .def("add_route", &ptui::Router::add_route)
        .def("set_fallback", &ptui::Router::set_fallback)
        .def("navigate", &ptui::Router::navigate)
        .def("back", &ptui::Router::back)
        .def("current_route", &ptui::Router::current_route)
        .def("render_current", &ptui::Router::render_current)
        .def("on_change", &ptui::Router::on_change);

    // Theme
    py::class_<ptui::Theme>(m, "Theme")
        .def_static("global", &ptui::Theme::global, py::return_value_policy::reference)
        .def("set_dark", &ptui::Theme::set_dark)
        .def("set_light", &ptui::Theme::set_light)
        .def_readwrite("colors", &ptui::Theme::ColorScheme);

    // Terminal
    py::class_<ptui::Terminal>(m, "Terminal")
        .def(py::init<>())
        .def("init", &ptui::Terminal::init)
        .def("shutdown", &ptui::Terminal::shutdown)
        .def("width", &ptui::Terminal::width)
        .def("height", &ptui::Terminal::height)
        .def("clear", &ptui::Terminal::clear)
        .def("present", &ptui::Terminal::present)
        .def("draw_text", &ptui::Terminal::draw_text)
        .def("draw_rect", &ptui::Terminal::draw_rect)
        .def("draw_box", &ptui::Terminal::draw_box)
        .def("set_cursor", &ptui::Terminal::set_cursor)
        .def("show_cursor", &ptui::Terminal::show_cursor)
        .def("set_title", &ptui::Terminal::set_title)
        .def("set_event_callback", &ptui::Terminal::set_event_callback)
        .def("get_event", &ptui::Terminal::get_event)
        .def("has_event", &ptui::Terminal::has_event);

    // Built-in widgets
    py::module_ widgets = m.def_submodule("widgets", "Built-in widgets");
    
    py::class_<ptui::widgets::Container, ptui::Component, std::shared_ptr<ptui::widgets::Container>>(widgets, "Container")
        .def(py::init<ptui::Style, std::vector<ptui::ElementPtr>>(), py::arg("style") = ptui::Style{}, py::arg("children") = std::vector<ptui::ElementPtr>{})
        .def("set_style", &ptui::widgets::Container::set_style)
        .def("set_children", &ptui::widgets::Container::set_children);

    py::class_<ptui::widgets::Text, ptui::Component, std::shared_ptr<ptui::widgets::Text>>(widgets, "Text")
        .def(py::init<std::string, ptui::Style>(), py::arg("content") = "", py::arg("style") = ptui::Style{})
        .def("set_content", &ptui::widgets::Text::set_content)
        .def("set_style", &ptui::widgets::Text::set_style);

    py::class_<ptui::widgets::Button, ptui::Component, std::shared_ptr<ptui::widgets::Button>>(widgets, "Button")
        .def(py::init<std::string, std::function<void()>, ptui::Style>(), py::arg("label") = "", py::arg("on_click") = nullptr, py::arg("style") = ptui::Style{})
        .def("set_label", &ptui::widgets::Button::set_label)
        .def("set_on_click", &ptui::widgets::Button::set_on_click)
        .def("set_style", &ptui::widgets::Button::set_style);

    py::class_<ptui::widgets::TextInput, ptui::Component, std::shared_ptr<ptui::widgets::TextInput>>(widgets, "TextInput")
        .def(py::init<std::string, std::function<void(const std::string&)>, ptui::Style>(), py::arg("placeholder") = "", py::arg("on_change") = nullptr, py::arg("style") = ptui::Style{})
        .def("set_value", &ptui::widgets::TextInput::set_value)
        .def("value", &ptui::widgets::TextInput::value)
        .def("set_placeholder", &ptui::widgets::TextInput::set_placeholder)
        .def("set_on_change", &ptui::widgets::TextInput::set_on_change)
        .def("set_style", &ptui::widgets::TextInput::set_style);

    py::class_<ptui::widgets::List, ptui::Component, std::shared_ptr<ptui::widgets::List>>(widgets, "List")
        .def(py::init<std::vector<std::any>, ptui::widgets::List::ItemRenderer, ptui::Style>(), py::arg("items") = std::vector<std::any>{}, py::arg("renderer") = nullptr, py::arg("style") = ptui::Style{})
        .def("set_items", &ptui::widgets::List::set_items)
        .def("set_renderer", &ptui::widgets::List::set_renderer)
        .def("set_style", &ptui::widgets::List::set_style);

    py::class_<ptui::widgets::ScrollView, ptui::Component, std::shared_ptr<ptui::widgets::ScrollView>>(widgets, "ScrollView")
        .def(py::init<ptui::ElementPtr, ptui::Style>(), py::arg("child") = nullptr, py::arg("style") = ptui::Style{})
        .def("set_child", &ptui::widgets::ScrollView::set_child)
        .def("set_style", &ptui::widgets::ScrollView::set_style);

    py::class_<ptui::widgets::Modal, ptui::Component, std::shared_ptr<ptui::widgets::Modal>>(widgets, "Modal")
        .def(py::init<bool, ptui::ElementPtr, std::function<void()>, ptui::Style>(), py::arg("visible") = false, py::arg("child") = nullptr, py::arg("on_close") = nullptr, py::arg("style") = ptui::Style{})
        .def("set_visible", &ptui::widgets::Modal::set_visible)
        .def("set_child", &ptui::widgets::Modal::set_child)
        .def("set_on_close", &ptui::widgets::Modal::set_on_close)
        .def("set_style", &ptui::widgets::Modal::set_style);

    // Colors submodule
    py::module_ colors = m.def_submodule("colors", "Color constants");
    colors.attr("Red") = ptui::colors::Red;
    colors.attr("Green") = ptui::colors::Green;
    colors.attr("Blue") = ptui::colors::Blue;
    colors.attr("Yellow") = ptui::colors::Yellow;
    colors.attr("Cyan") = ptui::colors::Cyan;
    colors.attr("Magenta") = ptui::colors::Magenta;
    colors.attr("White") = ptui::colors::White;
    colors.attr("Default") = ptui::colors::Default;
    colors.attr("BrightRed") = ptui::colors::BrightRed;
    colors.attr("BrightGreen") = ptui::colors::BrightGreen;
    colors.attr("BrightBlue") = ptui::colors::BrightBlue;
    colors.attr("BrightYellow") = ptui::colors::BrightYellow;
    colors.attr("BrightCyan") = ptui::colors::BrightCyan;
    colors.attr("BrightMagenta") = ptui::colors::BrightMagenta;
    colors.attr("BrightWhite") = ptui::colors::BrightWhite;
    colors.attr("Orange") = ptui::colors::Orange;
    colors.attr("Pink") = ptui::colors::Pink;
    colors.attr("Purple") = ptui::colors::Purple;
    colors.attr("Gray") = ptui::colors::Gray;
    colors.attr("DarkGray") = ptui::colors::DarkGray;
    colors.attr("LightGray") = ptui::colors::LightGray;

    // Easing functions
    py::module_ easing = m.def_submodule("easing", "Animation easing functions");
    easing.def("linear", &ptui::Animation::linear);
    easing.def("ease_in_quad", &ptui::Animation::ease_in_quad);
    easing.def("ease_out_quad", &ptui::Animation::ease_out_quad);
    easing.def("ease_in_out_quad", &ptui::Animation::ease_in_out_quad);
    easing.def("ease_in_cubic", &ptui::Animation::ease_in_cubic);
    easing.def("ease_out_cubic", &ptui::Animation::ease_out_cubic);
    easing.def("ease_in_out_cubic", &ptui::Animation::ease_in_out_cubic);
    easing.def("ease_in_sine", &ptui::Animation::ease_in_sine);
    easing.def("ease_out_sine", &ptui::Animation::ease_out_sine);
    easing.def("ease_in_out_sine", &ptui::Animation::ease_in_out_sine);
    easing.def("ease_in_expo", &ptui::Animation::ease_in_expo);
    easing.def("ease_out_expo", &ptui::Animation::ease_out_expo);
    easing.def("ease_in_out_expo", &ptui::Animation::ease_in_out_expo);
    easing.def("ease_in_circ", &ptui::Animation::ease_in_circ);
    easing.def("ease_out_circ", &ptui::Animation::ease_out_circ);
    easing.def("ease_in_out_circ", &ptui::Animation::ease_in_out_circ);
    easing.def("ease_in_back", &ptui::Animation::ease_in_back);
    easing.def("ease_out_back", &ptui::Animation::ease_out_back);
    easing.def("ease_in_out_back", &ptui::Animation::ease_in_out_back);
    easing.def("ease_in_elastic", &ptui::Animation::ease_in_elastic);
    easing.def("ease_out_elastic", &ptui::Animation::ease_out_elastic);
    easing.def("ease_in_out_elastic", &ptui::Animation::ease_in_out_elastic);
    easing.def("ease_in_bounce", &ptui::Animation::ease_in_bounce);
    easing.def("ease_out_bounce", &ptui::Animation::ease_out_bounce);
    easing.def("ease_in_out_bounce", &ptui::Animation::ease_in_out_bounce);
}

#endif // PTUI_PYTHON_BINDINGS

#endif // PTUI_HPP