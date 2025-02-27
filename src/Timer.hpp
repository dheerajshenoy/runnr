#pragma once

#include <raylib.h>
#include <functional>

class Timer {

public:
    Timer();
    ~Timer();
    inline void setCallback(const std::function<void()> &func) noexcept {
        m_callback = func;
    }
    std::function<void()> callback() const noexcept { return m_callback; }
    inline void setInterval(const float &s) noexcept { m_interval = s; }
    inline float interval() const noexcept { return m_interval; }
    inline void setSingleshot(const bool &singleshot) noexcept { m_singleshot = singleshot; }
    inline bool isSingleShot() const noexcept { return m_singleshot; }
    inline bool isRunning() const noexcept { return m_running; }
    void start() noexcept;
    void stop() noexcept;
    void update(const float &dt) noexcept;

private:
    float m_interval;
    float m_elapsed;
    bool m_singleshot;
    bool m_running;
    std::function<void()> m_callback;
};
