#pragma once

#include <raylib.h>
#include <functional>

class Button {

public:
    Button(const char *text, const float &x, const float &y,
           const float &width, const float &height);
    ~Button();
    void setText(const char *text) noexcept;
    void update() noexcept;
    void render() noexcept;
    void pressed(const std::function<void()> &callback);

private:
    const char *m_text;
    bool m_is_pressed;
    Rectangle m_bounds;
    Color m_normalColor;
    Color m_hoverColor;
    Color m_pressColor;
    Color m_textColor;
    std::function<void()> m_callback;
    Sound m_click_sound;
};
