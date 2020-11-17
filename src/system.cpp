#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
  
  vector<int> pids = LinuxParser::Pids();

  for (unsigned int i = 0 ; i < pids.size() ; i++) {
    int a = pids[i];
    
    if(find_if(processes_.begin(), processes_.end(), [a] (Process obj) {return obj.Ram() == std::to_string(a);}) == processes_.end()) {
      processes_.push_back(Process(pids[i]));
    }   
  }

  sort(processes_.begin(), processes_.end());

  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel();}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization();}

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }