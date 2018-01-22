#ifndef TH_POOL
#define TH_POOL

#include <Worker.hpp>
#include <vector>

class ThreadPool
{
public:
  ThreadPool(size_t);
  ~ThreadPool();

  template <class FN, class... ARGS>
  void add(FN fn, ARGS... args)
  {
    getFreeWorker()->addFunc(std::bind(fn, args...));
  }

private:
  worker_ptr getFreeWorker();

  std::vector<worker_ptr> m_workers;
};
#endif