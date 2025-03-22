#include "ColorRegistry.hpp"


void ColorRegistry::Initialize() noexcept
{
    m_defaultColors[ColorKey::PLAYER] = { 1.0, 1.0, 0.0, 1.0 };
    m_defaultColors[ColorKey::PLATFORM_REGULAR] = { 0.0, 0.0, 1.0, 1.0 };
    m_defaultColors[ColorKey::POWERUP_FAST] = { 0.4, 0.2, 1.0, 1.0 };
    m_defaultColors[ColorKey::POWERUP_JUMP] = { 0.8, 0.7, 0.3, 1.0 };
}

Vector4 ColorRegistry::GetDefaultColor(const ColorKey &key) noexcept
{
    return m_defaultColors[key];
}

Color ColorRegistry::GetDefaultColor255rl(const ColorKey &key) noexcept
{
    auto color = m_defaultColors[key];
    return {
        color.x * 255.0,
        color.y * 255.0,
        color.z * 255.0,
        color.w * 255.0
    };
}
