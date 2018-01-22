#ifndef WORKER
#define WORKER

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <string>

typedef std::function<void()> fn_type;

class Worker
{
  public:
    Worker();
    ~Worker();
    void addFunc(fn_type fn);
    size_t getTaskCount();
    bool isEmpty();

  private:
    bool m_enabled;
    std::condition_variable m_cv;
    std::queue<fn_type> m_fqueue;
    std::mutex m_mutex;
    std::thread m_thread;

    void threadLoop();
};

typedef std::shared_ptr<Worker> worker_ptr;
#endif