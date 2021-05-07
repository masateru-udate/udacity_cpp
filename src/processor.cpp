#include "processor.h"
#include <iostream>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpu_values = LinuxParser::SystemCpuUtilization();
  setUtilizationValues(cpu_values);
  return calcUtilization();
}

void Processor::setUtilizationValues(vector<string> cpu_values) {
  user = (cpu_values[1] != "") ? stoi(cpu_values[1]) : 0;
  nice = (cpu_values[2] != "") ? stoi(cpu_values[2]) : 0;
  system = (cpu_values[3] != "") ? stoi(cpu_values[3]) : 0;
  idle = (cpu_values[4] != "") ? stoi(cpu_values[4]) : 0;
  iowait = (cpu_values[5] != "") ? stoi(cpu_values[5]) : 0;
  irq = (cpu_values[6] != "") ? stoi(cpu_values[6]) : 0;
  softirq = (cpu_values[7] != "") ? stoi(cpu_values[7]) : 0;
  steal = (cpu_values[8] != "") ? stoi(cpu_values[8]) : 0;
  guest = (cpu_values[9] != "") ? stoi(cpu_values[9]) : 0;
  guest_nice = (cpu_values[10] != "") ? stoi(cpu_values[10]) : 0;
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