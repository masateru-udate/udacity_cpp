#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();
  void setUtilizationValues(vector<string> cpu_values, int cpu_num);
  float calcUtilization();

 private:
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

#endif