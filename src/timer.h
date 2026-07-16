#pragma once

#include <chrono>
#include <cstdint>

// Tracks elapsed wall-clock seconds for the current game session.
// Call start() once when the game begins (or after loading a saved game).
// Elapsed seconds already accumulated before a save can be injected via
// setOffset() so that a loaded game continues from the right time.
class Timer
{
public:
    Timer() = default;

    // Begin (or resume) counting from the current wall-clock time.
    void start()
    {
        _start = std::chrono::steady_clock::now();
        _running = true;
    }

    // Return total elapsed seconds (offset + time since start()).
    int64_t elapsedSeconds() const
    {
        if (!_running)
            return _offset;
        auto now = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::seconds>(now - _start).count();
        return _offset + delta;
    }

    // Inject previously accumulated seconds (used when loading a saved game).
    void setOffset(int64_t seconds)
    {
        _offset = seconds;
    }

    int64_t offset() const { return _offset; }

private:
    std::chrono::steady_clock::time_point _start{};
    int64_t _offset{0};
    bool _running{false};
};
