#pragma once

#include <unordered_map>
#include <raylib.h>

enum class ColorKey
{
    PLAYER = 0,
    PLATFORM_REGULAR,
    PLATFORM_STICKY,
    POWERUP_JUMP,
    POWERUP_FAST,
    POWERUP_SLOW,
};

class ColorRegistry
{
    public:
    static void Initialize() noexcept;
    static Vector4 GetDefaultColor(const ColorKey &key) noexcept;
    static Color GetDefaultColor255rl(const ColorKey &key) noexcept;

    private:
    static inline std::unordered_map<ColorKey, Vector4> m_defaultColors;

};
