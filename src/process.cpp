#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }
void Process::Pid(int pid) { pid_ = pid; }

float Process::CpuUtilization() { return cpuutilization_; }
void Process::CpuUtilization(int pid) {
  cpuutilization_ = LinuxParser::ProcessCpuUtilization(pid);
}

string Process::Command() { return command_; }
void Process::Command(int pid) { command_ = LinuxParser::Command(pid); }

string Process::Ram() { return ram_; }
void Process::Ram(int pid) {
  int ram_mb;
  string ram_string;
  ram_string = LinuxParser::Ram(pid);
  ram_mb = std::stof(ram_string) / 1000;
  ram_ = std::to_string(ram_mb);
}

string Process::User() { return user_; }
void Process::User(int pid) {
  string user_name, uid;
  uid = LinuxParser::Uid(pid);
  user_ = LinuxParser::User(std::stoi(uid));
}

long int Process::UpTime() { return uptime_; }
void Process::UpTime(int pid) { uptime_ = LinuxParser::UpTime(pid); }

bool Process::operator<(Process const& a) const {
  return a.cpuutilization_ < cpuutilization_;
}