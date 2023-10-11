#pragma once

#include <stdarg.h>
#include <cassert>
#include <functional>
#include <list>
#include <string>
#include <vector>
#include "Arduino.h"

//******************************************************************************
// Teensy specific implementation
//******************************************************************************
#define CANFETTI_NO_INLINE __attribute__((noinline))

namespace canfetti {

unsigned newGeneration();

class Logger {
 public:
  static constexpr bool needNewline = true;
  static Logger logger;
  bool debug = false;

  template <typename... Args>
  inline void emitLogMessage(const char* file, const char* func, int line, const char* fmt, Args... args)
  {
    Serial.printf(fmt, args...);
  }
};

class System {
  struct TimerData {
    uint32_t lastFireTime;
    uint32_t delay;
    uint32_t period;
    std::function<void()> cb;
    bool enable    = false;
    bool available = true;
  };
  std::vector<TimerData> timers;

 public:
  using TimerHdl                         = size_t;
  static constexpr TimerHdl InvalidTimer = SIZE_MAX;

  void deleteTimer(TimerHdl& hdl);
  TimerHdl scheduleDelayed(uint32_t delayMs, std::function<void()> cb);
  TimerHdl schedulePeriodic(uint32_t periodMs, std::function<void()> cb, bool staggeredStart = true);

  void service();
};

}  // namespace canfetti
