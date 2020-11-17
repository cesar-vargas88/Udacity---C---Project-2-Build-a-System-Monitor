#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();  

 private:
  int prev_idle;
  int prev_iowait;
  int prev_user;
  int prev_nice;
  int prev_system;
  int prev_irq;
  int prev_softirq;
  int prev_steal;
};

#endif