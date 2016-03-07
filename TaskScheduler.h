#ifndef BT_FLAG_TASK_H
#define BT_FLAG_TASK_H

#include "Task.h"
#include "IExecutor.h"
#include "queue.h"

namespace bt {

  class SchedulerTask : public Task, public IExecutor
  {
  public:
    SchedulerTask(Task::TaskCallback callback) 
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
    Task::TaskCallback mCallback;
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
    }
  private:
    Queue<pSchedulerTask> mTasks;
  };
}

#endif //BT_FLAG_TASK_H