#include "Timer.hpp"
#include <raylib.h>

Timer::Timer()
:   m_interval(0.0f),
    m_elapsed(0.0f),
    m_singleshot(false),
    m_running(false),
    m_callback { nullptr }
{}

Timer::~Timer()
{}

void Timer::start() noexcept
{
    m_running = true;
    m_elapsed = 0.0f;
}

void Timer::update(const float &dt) noexcept
{
    if (!m_running)
        return;

    m_elapsed += dt;

    if (m_elapsed >= m_interval) {

        if (m_singleshot) {
            m_running = false;

        } else {
            m_elapsed = 0.0f;
        }

        if (m_callback)
            m_callback();

    }

}

void Timer::stop() noexcept
{

    m_running = false;
}
