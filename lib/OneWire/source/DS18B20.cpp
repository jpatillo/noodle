#include "DS18B20.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

DS18B20::DS18B20(std::string serial):OneWireDevice(serial){}
DS18B20::~DS18B20(){}

double DS18B20::get_fahrenheittemp(){
  return get_fahrenheittemp(get_rawtemp());
}

double DS18B20::get_fahrenheittemp(long t){
  return (t/1000) * (9/5) + 32;
}

double DS18B20::get_celsiustemp(){
  return get_celsiustemp(get_rawtemp());
}

double DS18B20::get_celsiustemp(long t){
  return (double)t/1000;
}

long DS18B20::get_rawtemp(){
  long temp;
  std::string line, value;
  std::ifstream infile;
  infile.open (get_driverpath().c_str());
  // The first line is the CRC
  getline(infile,line);
  if(line.find("YES")!=std::string::npos) {
    // The second line is the temp
    getline(infile,line);
    //get temp
    value = line.substr(29);
    //convert to long
    temp = strtol(value.c_str(),NULL,10);
  }
  //TODO some kind of error if find doesn't work out

  infile.close();

  return temp;

}