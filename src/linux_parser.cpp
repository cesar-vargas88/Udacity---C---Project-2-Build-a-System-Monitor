#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float MemTotal;
  float MemFree;
  string line;
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    MemTotal = stof(line.substr(9, line.length()));
    std::getline(stream, line);
    MemFree = stof(line.substr(8, line.length()));
  }

  return (float) ((MemTotal - MemFree) / MemTotal);
}

long LinuxParser::UpTime() { 
  string line;
  long uptime;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    size_t index = line.find(" ");
    
    if (index != string::npos)
        uptime = stof(line.substr(0, index));
  }

  return uptime; 
}

long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() { return 0; }

long LinuxParser::IdleJiffies() { return 0; }

vector<float> LinuxParser::CpuUtilization() { 
  string line, key;
  float value;
  vector<float> values;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      // parser all cpu values
      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> value) {
            values.emplace_back(value);
          }
        }
      }
    }
  }
  
  return values;
}

int LinuxParser::TotalProcesses() { 
  string line;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

	  if (line.find("processes") != string::npos)
        return stoi(line.substr(9, line.length()));
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {   
  string line;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      
	  if (line.find("procs_running") != string::npos)
        return stoi(line.substr(13, line.length()));
    } 
  }
  
  return 0;
}

string LinuxParser::Command(int pid) { 
 string command;
 string line;
  
 std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
		command+=line;
    }
  }
  
 return command;
}

string LinuxParser::Ram(int pid) { 
  string line;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

	  if (line.find("VmSize:") != string::npos)
        return to_string(stoi(line.substr(8, line.length()))/1000);
    }
  }
  
  return string{};
  
  return string(); 
}

string LinuxParser::Uid(int pid) {  
  string line;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

	  if (line.find("Uid:") != string::npos)
        return to_string(stoi(line.substr(4, line.length())));
    }
  }
  
  return string{};
}

string LinuxParser::User(int pid) {  
  string line;
  string uid = Uid(pid);

  std::ifstream stream(kPasswordPath);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
	  size_t end_username = line.find(":x:"+uid+":"+uid);
	  if (end_username != string::npos)
        return line.substr(0, end_username);
    }
  }
  
  return string{};
}

long int LinuxParser::UpTime(int pid) {   
  string line;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
	  std::istringstream buf(line);
 	  std::istream_iterator<std::string> beg(buf), end;
 	  std::vector<std::string> values(beg, end);
    
 	// Using sysconf to get clock ticks of the host machine
 	return long(stof(values[21])/sysconf(_SC_CLK_TCK));
  }
  
  return 0;
}

float LinuxParser::CpuUtilization(int pid) {
  std::string line;
  float cpu_usage;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::string str = line;
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg, end);

    // acquiring relevant times for calculation of active occupation of CPU for selected process
    const float utime = std::stof(values[13]);
    const float stime = std::stof(values[14]);
    const float cutime = std::stof(values[15]);
    const float cstime = std::stof(values[16]);
    const float starttime = std::stof(values[21]);
    const float uptime = LinuxParser::UpTime();

    const float total_time = (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK); // seconds
    const float elapsed_time = uptime - (starttime / sysconf(_SC_CLK_TCK)); // seconds

    cpu_usage = total_time / elapsed_time;   
 }
  
  return cpu_usage;
}