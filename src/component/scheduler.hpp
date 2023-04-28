#pragma once

#include <functional>
#include <chrono>

using namespace std::literals;

namespace scheduler
{
    enum pipeline
    {
        server,
        async,
        count,
    };

    static const bool cond_continue = false;
    static const bool cond_end = true;

    void schedule(const std::function<bool()>& callback, pipeline type = pipeline::server,
        std::chrono::milliseconds delay = 0ms, bool update_after_call = false);
    void loop(const std::function<void()>& callback, pipeline type = pipeline::server,
        std::chrono::milliseconds delay = 0ms, bool update_after_call = false);
    void once(const std::function<void()>& callback, pipeline type = pipeline::server,
        std::chrono::milliseconds delay = 0ms, bool update_after_call = false);
}
