#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t threads = 0)
{
    if (threads == 0)
        threads = std::thread::hardware_concurrency();

    for (size_t i = 0; i < threads; i++)
    {
        worker_ptr pWorker(new Worker);
        m_workers.emplace_back(pWorker);
    }
}

ThreadPool::~ThreadPool() {}

worker_ptr ThreadPool::getFreeWorker()
{
    worker_ptr pWorker;
    size_t minTasks = UINT32_MAX;

    for (auto &w : m_workers)
    {
        if (w->isEmpty())
        {
            return w;
        }
        else if (minTasks > w->getTaskCount())
        {
            minTasks = w->getTaskCount();
            pWorker = w;
        }
    }
    return pWorker;
}
