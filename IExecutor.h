#pragma once
#ifndef I_EXECUTOR_H
#define I_EXECUTOR_H

class IExecutor {
public:
  virtual ~IExecutor() {}
  virtual uint8_t execute() = 0;
};

#endif //I_EXECUTOR_H