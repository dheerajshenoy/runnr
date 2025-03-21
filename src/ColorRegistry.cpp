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
