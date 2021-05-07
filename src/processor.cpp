#include "processor.h"
#include <iostream>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpu_values = LinuxParser::SystemCpuUtilization();
  setUtilizationValues(cpu_values, 0);
  return calcUtilization();
}

void Processor::setUtilizationValues(vector<string> cpu_values,
                                     int cpu_num = 0) {
  user = stoi(cpu_values[cpu_num * 11 + 1]);
  nice = stoi(cpu_values[cpu_num * 11 + 2]);
  system = stoi(cpu_values[cpu_num * 11 + 3]);
  idle = stoi(cpu_values[cpu_num * 11 + 4]);
  iowait = stoi(cpu_values[cpu_num * 11 + 5]);
  irq = stoi(cpu_values[cpu_num * 11 + 6]);
  softirq = stoi(cpu_values[cpu_num * 11 + 7]);
  steal = stoi(cpu_values[cpu_num * 11 + 8]);
  guest = stoi(cpu_values[cpu_num * 11 + 9]);
  guest_nice = stoi(cpu_values[cpu_num * 11 + 10]);
}

float Processor::calcUtilization() {
  // TODO: check pre-defined float int are appropriate or not
  float usertime = user - guest;
  float nicetime = nice - guest_nice;
  unsigned long long int idlealltime = idle + iowait;
  unsigned long long int systemalltime = system + irq + softirq;
  unsigned long long int virtalltime = guest + guest_nice;
  unsigned long long int totaltime =
      usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
  // std::cout << "totaltime:" << totaltime;
  return (float)(totaltime - idlealltime) / totaltime;
}