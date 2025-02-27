#include "Button.hpp"
#include <raylib.h>

Button::Button(const char *text, const float &x, const float &y, const float &width, const float &height)
:   m_text(text),
    m_bounds {x, y, width, height },
    m_is_pressed(false),
    m_textColor(BLACK),
    m_normalColor(LIGHTGRAY),
    m_hoverColor(GRAY)

{
    m_click_sound = LoadSound("../resources/sounds/African1.mp3");
}

Button::~Button()
{
    UnloadSound(m_click_sound);
}

void Button::update() noexcept
{
    if (CheckCollisionPointRec(GetMousePosition(), m_bounds)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !m_is_pressed) {
            m_is_pressed = true;
            PlaySound(m_click_sound);
            if (m_callback)
                m_callback();
        }

        if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
            m_is_pressed = false;
        }
    }
}

void Button::render() noexcept
{
    Color currentColor = m_normalColor;
    if (CheckCollisionPointRec(GetMousePosition(), m_bounds)) {
        currentColor = m_hoverColor;
        if (m_is_pressed) {
            currentColor = m_pressColor;
        }
    }

    DrawRectangleRec(m_bounds, currentColor);
    int textWidth = MeasureText(m_text, 20);
    DrawText(m_text, m_bounds.x + (m_bounds.width - textWidth) / 2, m_bounds.y + (m_bounds.height - 20) / 2, 20, m_textColor);
}

void Button::pressed(const std::function<void()> &callback)
{
    m_callback = callback;
}


void Button::setText(const char *text) noexcept
{
    m_text = text;
}
