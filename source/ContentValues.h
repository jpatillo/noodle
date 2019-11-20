// NOTE pipes '|' are not allowed.

#include <map>
#include <string>
#include <vector>

#ifndef VIVARIA_CONTENTVALUES_H_
#define VIVARIA_CONTENTVALUES_H_

class ContentValues: public std::multimap<std::string,std::string> {

public:

//  typedef map<string,string>::iterator iterator;

  ContentValues();

  std::string getCommaSeparatedKeys();
  std::string getCommaSeparatedValues();

  bool hasKey(std::string key);
  std::string findString(std::string key);
//  bool getAsBoolean(string key){}
  double findDouble(std::string key);
  float findFloat(std::string key);
  int findInt(std::string key);
  long findLong(std::string key);
  std::vector<std::string> keyList();
  std::vector<std::string> valueList();
  void insert(std::string key,std::string value);
  void insert(std::string key,double value);
  void insert(std::string key,float value);
  void insert(std::string key,int value);
  void insert(std::string key,long value);
  void insert(std::string key);

  std::string serialize();
  int deserialize(std::string);

};

#endif
