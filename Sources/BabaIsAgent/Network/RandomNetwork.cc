#include <BabaIsAgent/Network/RandomNetwork.hpp>

#include <BabaIsAgent/Utils/Utils.hpp>

#include <effolkronium/random.hpp>

namespace BabaIsAgent::Network
{
void RandomNetwork::Initialize([
    [maybe_unused]] const std::string& weightFileName)
{
}

void RandomNetwork::Evaluate([[maybe_unused]] const std::vector<Tensor>& inputs,
                             std::vector<Tensor>& policy, Tensor& value)
{
    using Random = effolkronium::random_static;

    const std::size_t batchSize = policy.size();

    const std::size_t actionSize = Utils::ACTION_SPACE.size();

    for (std::size_t batch = 0; batch < batchSize; ++batch)
    {
        policy[batch].resize(actionSize);

        float sum = 1e-10f;
        for (auto& p : policy[batch])
        {
            p = Random::get<float>(0, 1);
            sum += p;
        }

        for (auto& p : policy[batch])
            p /= sum;

        value[batch] = Random::get<float>(0, 1);
    }
}
}  // namespace BabaIsAgent::Network
