#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  void Pid(int pid);
  std::string User();
  void User(int pid);
  std::string Command();
  void Command(int pid);
  float CpuUtilization();
  void CpuUtilization(int pid);
  std::string Ram();
  void Ram(int pid);
  long int UpTime();
  void UpTime(int pid);
  bool operator<(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpuutilization_;
  std::string ram_;
  long int uptime_;
};

#endif