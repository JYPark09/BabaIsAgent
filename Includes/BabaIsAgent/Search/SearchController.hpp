#ifndef BABA_IS_AGENT_SEARCH_CONTROLLER_HPP
#define BABA_IS_AGENT_SEARCH_CONTROLLER_HPP

#include <BabaIsAgent/Common/Config.hpp>
#include <BabaIsAgent/Utils/WaitGroup.hpp>

#include <condition_variable>
#include <mutex>

namespace BabaIsAgent::Search
{
enum class SearchState
{
    PAUSE,
    SEARCHING,
    TERMINATE
};

class SearchController final
{
 public:
    explicit SearchController(const Common::Config& config);

    void Pause();
    void AckPause();
    void Resume();
    void Terminate();

    bool WaitResume();

    SearchState GetState() const;

 private:
    const Common::Config& config_;

    mutable std::mutex mutex_;
    std::condition_variable cv_;

    SearchState state_{ SearchState::PAUSE };

    Utils::WaitGroup resumeGroup_;
    Utils::WaitGroup pauseGroup_;
};
}  // namespace BabaIsAgent::Search

#endif  // BABA_IS_AGENT_SEARCH_CONTROLLER_HPP
