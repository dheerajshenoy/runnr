#pragma once

#include <functional>
#include <vector>

class TimerManager
{
    public:
    struct Timer
    {
        float timeLeft;
        std::function<void()> callback;
    };

    void addTimer(const Timer &timer) noexcept
        {
            timers.push_back(timer);
        }

    void addTimer(const float &duration,
                  const std::function<void()> &callback) noexcept
        {
            timers.emplace_back(duration, callback);
        }

    void update(const float &dt) noexcept
        {
            for (auto it = timers.begin(); it != timers.end();)
            {
                it->timeLeft -= dt;
                if (it->timeLeft <= 0.0f)
                {
                    it->callback();
                    it = timers.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

    private:
    std::vector<Timer> timers;
};
