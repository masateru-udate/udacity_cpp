#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long h, m, s;

  h = seconds / 60 / 60;
  m = seconds / 60 % 60;
  s = seconds % 60;
  string Hour = (h < 10 ? "0" : "") + std::to_string(h);
  string Minute = (m < 10 ? "0" : "") + std::to_string(m);
  string Second = (s < 10 ? "0" : "") + std::to_string(s);

  return Hour + ":" + Minute + ":" + Second;
}