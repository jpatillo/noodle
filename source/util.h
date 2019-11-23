
#ifndef NOODLE_UTIL_H
#define NOODLE_UTIL_H

#include <string>

// Returns the current date/time in GMT formatted as "Y-m-d H:M:S"
std::string get_formatnowdate();

// @param date char[] that the formatted date will be stored to.
// @param size Size of the char[] date. Should be >19.
// Returns >0 if something bad happens
int get_formatnowdate(char *date, int size);

std::string tail(std::string const& source, size_t const length);


std::string build_serial(int protocol, int family, int pin);


#endif
