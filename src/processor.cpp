#include "processor.h"
#include <sstream>
#include "linux_parser.h"

float Processor::Utilization() { 
  // get cpu values
  vector<float> values = LinuxParser::CpuUtilization();
  
  // asign values to pcu variables
  const float user = values[0];
  const float nice = values[1];
  const float system = values[2];
  const float idle = values[3];
  const float iowait = values[4];
  const float irq = values[5];
  const float softirq = values[6];
  const float steal = values[7];

  // compute cpu usage algorithm 
  const float Prev_Idle = prev_idle + prev_iowait;
  const float Idle = idle + iowait;

  const float Prev_NonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
  const float NonIdle = user + nice + system + irq + softirq + steal;

  const float Prev_Total = Prev_Idle + Prev_NonIdle;
  const float Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  float totald = Total - Prev_Total;
  float idled = Idle - Prev_Idle;

  // assign previous values
  prev_user = values[0];
  prev_nice = values[1];
  prev_system = values[2];
  prev_idle = values[3];
  prev_iowait = values[4];
  prev_irq = values[5];
  prev_softirq = values[6];
  prev_steal = values[7];
  
  return (totald - idled) / totald;
}