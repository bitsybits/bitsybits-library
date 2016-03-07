#ifndef BT_FLAG_TASK_H
#define BT_FLAG_TASK_H

#include <functional>
#include "Task.h"
#include "IExecutor.h"
#include "queue.h"

namespace bt {

  class SchedulerTask : public Task, public IExecutor
  {
  public:
    using SchedulerTaskCallback = std::function<void(void)>;

    SchedulerTask(IExecutor* executor) 
    : SchedulerTask([=]{executor->execute();}) {}

    SchedulerTask(SchedulerTaskCallback callback) 
    : Task(), mCallback(callback), mCanDoHardWork(false) {}

    void attach(uint32_t ms, bool repeat) {
      Task::attach<volatile bool*>(ms, repeat, [](volatile bool* can){ *can = true; }, &mCanDoHardWork);
    }

    virtual uint8_t execute() {
      if(mCanDoHardWork) {
        mCallback();
        mCanDoHardWork = false;
      }
      return 0;
    }
  private:
    volatile bool mCanDoHardWork;
    SchedulerTaskCallback mCallback;
  };

  class TaskScheduler : public IExecutor
  {
  public:
    using pSchedulerTask = SchedulerTask*;

    TaskScheduler* push(pSchedulerTask task){
      mTasks.push(task);
      return this;
    }

    virtual uint8_t execute() {
      mTasks.forEach( [](pSchedulerTask task){ task->execute(); yield(); } );
      return 0;
    }
  private:
    Queue<pSchedulerTask> mTasks;
  };
}

#endif //BT_FLAG_TASK_H