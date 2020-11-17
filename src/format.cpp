#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {   
  string hour = std::to_string((int) (seconds / 3600));
  string min = std::to_string((int) ((seconds % 3600) / 60));
  string sec = std::to_string((int) seconds % 60);

  if (hour.length() == 1) hour = "0" + hour;
  if (min.length() == 1) min = "0" + min;
  if (sec.length() == 1) sec = "0" + sec;
      
  return hour + ":" + min + ":" + sec;
}