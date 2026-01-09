#include "dosmParallelCPU.hpp"

#include <thread>
#include <vector>

namespace dosm
{
    DosmParallelCPU::DosmParallelCPU(idx_t tids)
    {
        if (tids <= 0)
            tids = static_cast<idx_t>(std::thread::hardware_concurrency());

        if (tids <= 0)
            tids = 1;

        this->tids = tids;
    }

    void DosmParallelCPU::init(void)
    {
    }

    void DosmParallelCPU::release(void)
    {
    }

    void DosmParallelCPU::dispatch(idx_t N, const std::function<void(idx_t)>& routine)
    {
        if (N <= 0)
            return;

        std::vector<std::thread> threads;
        for (idx_t tid = 0; tid < this->tids; ++tid)
            threads.emplace_back([&, tid]() { for (idx_t i = tid; i < N; i += this->tids) routine(i); });

        for (auto& th : threads)
            th.join();
    }

    void DosmParallelCPU::dispatch(idx_t N, const std::function<void(idx_t, r64_t&)>& routine, r64_t& result)
    {
        if (N <= 0)
        {
            result = 0.0;
            return;
        }

        std::vector<std::thread> threads;
        std::vector<r64_t> partial(this->tids, 0.0);

        for (idx_t tid = 0; tid < this->tids; ++tid)
            threads.emplace_back([&, tid]() { r64_t acc = 0.0; for (idx_t i = tid; i < N; i += this->tids) routine(i, acc); partial[tid] = acc; });

        for (auto& th : threads)
            th.join();

        r64_t acc = 0.0;
        for (idx_t tid = 0; tid < this->tids; ++tid)
            acc += partial[tid];

        result = acc;
    }

} // namespace dosm
