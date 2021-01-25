#include <BabaIsAgent/Utils/WaitGroup.hpp>

namespace BabaIsAgent::Utils
{
void WaitGroup::Add(int incr)
{
    std::lock_guard<std::mutex> lock(mutex_);

    counter_ += incr;
    if (counter_ <= 0)
        cv_.notify_all();
}

void WaitGroup::Done()
{
    Add(-1);
}

void WaitGroup::Wait()
{
    std::unique_lock<std::mutex> lock(mutex_);

    cv_.wait(lock, [this] { return counter_ <= 0; });
}
}