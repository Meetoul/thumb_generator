#include "Worker.hpp"

Worker::Worker() : m_enabled(true), m_fqueue(),
                   m_thread(&Worker::threadLoop, this) {}

Worker::~Worker()
{
    m_enabled = false;
    m_cv.notify_one();
    m_thread.join();
}

void Worker::addFunc(fn_type fn)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_fqueue.push(fn);
    m_cv.notify_one();
}

size_t Worker::getTaskCount()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_fqueue.size();
}

bool Worker::isEmpty()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_fqueue.empty();
}

void Worker::threadLoop()
{
    while (m_enabled)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait(lock, [&] { return !m_fqueue.empty() || !m_enabled; });

        while (!m_fqueue.empty())
        {
            fn_type fn = m_fqueue.front();
            lock.unlock();
            fn();
            lock.lock();
            m_fqueue.pop();
        }
    }
}