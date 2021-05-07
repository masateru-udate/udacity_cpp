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
  string os, version_prefix, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version_prefix >> kernel;
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
  float MemTotal{0}, MemFree{0};
  string line, key, value, value_unit;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value >> value_unit;
      if (key == "MemTotal:") {
        MemTotal = std::stof(value);
      } else if (key == "MemFree:") {
        MemFree = std::stof(value);
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

long LinuxParser::UpTime() {
  string uptime{"0"};
  string idletime{"0"};
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return std::stol(uptime);
}

// TODO: Read and return CPU utilization
float LinuxParser::ProcessCpuUtilization(int pid) {
  float utime, stime, cutime, cstime, starttime, system_uptime;
  string line, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int value_num = 1;
    while (linestream >> value) {
      if (value_num == 14) {
        utime = std::stof(value);
      } else if (value_num == 15) {
        stime = std::stof(value);
      } else if (value_num == 16) {
        cutime = std::stof(value);
      } else if (value_num == 17) {
        cstime = std::stof(value);
      } else if (value_num == 22) {
        starttime = std::stof(value);
      }
      value_num++;
    }
  }

  system_uptime = float(LinuxParser::UpTime());
  float total_time = utime + stime + cutime + cstime;
  float seconds = system_uptime - starttime / sysconf(_SC_CLK_TCK);

  return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
}

vector<string> LinuxParser::SystemCpuUtilization() {
  string user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  vector<string> return_vector;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key.substr(0, 3) == "cpu") {
        return_vector.push_back(key);
        while (linestream >> value) {
          return_vector.push_back(value);
        }
      }
    }
  }
  return return_vector;
}

int LinuxParser::TotalProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return std::stof(value);
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return std::stof(value);
      }
    }
  }

  return 0;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    getline(stream, line);
  }

  return line;
}

string LinuxParser::Ram(int pid) {
  string line, key, value1, value2, ret_value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value1 >> value2;
      if (key == "VmSize:") {
        ret_value = value1;
      }
    }
  }
  return ret_value;
}

string LinuxParser::Uid(int pid) {
  string value, key, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> value;
      }
    }
  }

  return value;
}

string LinuxParser::User(int uid) {
  string key, value, line, _, ret_uname{"none"};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> value >> _ >> key;

      if (key == std::to_string(uid)) {
        ret_uname = value;
      }
    }
  }
  return ret_uname;
}

long LinuxParser::UpTime(int pid) {
  string line, value;
  long ret_value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int lineN{1};
    while (linestream >> value) {
      if (lineN == 22) {
        ret_value = std::stol(value);
      }

      lineN++;
    }
  }

  return ret_value / sysconf(_SC_CLK_TCK);
}