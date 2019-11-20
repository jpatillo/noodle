#include "util.h"
#include <sstream>
#include <iomanip>
#include <time.h>


// Returns the current date/time in GMT formatted as "Y-m-d H:M:S"
std::string get_formatnowdate(){
  time_t r;
  char dt[32];
  struct tm* t;
  time(&r);
  t = gmtime(&r);
  strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", t);
  //string date(dt);
  return std::string(dt);
}

// @param date char[] that the formatted date will be stored to.
// @param size Size of the char[] date. Should be >19.
// Returns >0 if something bad happens
int get_formatnowdate(char *date, int size){
  if(size<20) {return 1;}
  time_t r;
  struct tm* t;
  time(&r);
  t = gmtime(&r);
  strftime(date, size, "%Y-%m-%d %H:%M:%S", t);
  //string date(dt);
  return 0;
}

std::string tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
} // tail

// Has to be 15 chars
// First is the protocol (i.e. 1-wire)
// Next 2 are the family code
// Next 2 are pin or address
// Final 10 are the serial. This is all 0 unless the serial already exists.
std::string build_serial(int protocol, int family, int pin){
  std::ostringstream is;
  is << std::setw(1) << std::hex << protocol
    << std::setfill('0') << std::setw(2) << std::hex << family
    << std::setfill('0') << std::setw(2) << std::hex << pin
    << std::setfill('0') << std::setw(10) << std::hex << 0;
  const std::string rvalue = is.str();
  return rvalue;
}
