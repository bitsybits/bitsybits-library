#ifndef BT_TASK_H
#define BT_TASK_H

#include <stdint.h>
#include <stddef.h>

extern "C" {
#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "osapi.h"
}

namespace bt {

  extern "C" {
    typedef struct _ETSTIMER_ ETSTimer;
  }

  class Task
  {
  public:
    using TaskCallback = void (*)(void);
    using TaskArgCallback = void (*)(void*);
    template<typename T>
    using TaskTypeCallback = void (*)(T);
    
    Task() 
    : mpTimer(nullptr) {}

    ~Task() {
      detach();
    }

    void attach(uint32_t ms, bool repeat, TaskCallback callback) {
      attach_arg(ms, repeat, reinterpret_cast<TaskArgCallback>(callback), 0);
    }

    template<typename T>
    void attach(uint32_t ms, bool repeat, TaskTypeCallback<T> callback, T arg) {
      static_assert(sizeof(T) <= sizeof(uint32_t), "sizeof arg must be <= sizeof(uint32_t), i.e 4 bytes");
      attach_arg(ms, repeat, reinterpret_cast<TaskArgCallback>(callback), (uint32_t) arg);
    }

    void detach()
    {
      if (mpTimer) {
        os_timer_disarm(mpTimer);
        delete mpTimer;
        mpTimer = nullptr;
      }
    }

  private:
    ETSTimer* mpTimer;

    void attach_arg(uint32_t ms, bool repeat, TaskArgCallback callback, uint32_t arg)
    {
      if (mpTimer) {
        os_timer_disarm(mpTimer);
      } else {
        mpTimer = new ETSTimer;
      }

      os_timer_setfn(mpTimer, reinterpret_cast<ETSTimerFunc*>(callback), reinterpret_cast<void*>(arg));
      os_timer_arm(mpTimer, ms, repeat);
    }
  };
}

#endif //BT_TASK_H