#include "ContentValues.h"
#include <utility>
#include <tuple>
#include <sstream>

using namespace std;

ContentValues::ContentValues(){}
//  ContentValues(int size){}
//  ContentValues(ContentValues copy){}


string ContentValues::getCommaSeparatedKeys(){
  ostringstream ss;
  int t=size()-1;
  multimap<string,string>::iterator it=begin();
  for (int c=0; c<t && it!=end(); ++c,++it)
    ss << it->first << ", ";
  ss << it->first;
  const string rvalue = ss.str();
  return rvalue;
}


string ContentValues::getCommaSeparatedValues(){
  ostringstream ss;
  int t=size()-1;
  multimap<string,string>::iterator it=begin();
  for (int c=0; c<t && it!=end(); ++c,++it)
    ss << it->second << ", ";
  ss << it->second;
  const string rvalue = ss.str();
  return rvalue;
}


  bool ContentValues::hasKey(string key){
    return find(key)==end()?false:true;}
  string ContentValues::findString(string key){return find(key)->second;}
//  bool ContentValues::getAsBoolean(string key){}
  double ContentValues::findDouble(string key){return stod(find(key)->second);}
  float ContentValues::findFloat(string key){return stof(find(key)->second);}
  int ContentValues::findInt(string key){return stoi(find(key)->second);}
  long ContentValues::findLong(string key){return stol(find(key)->second);}
  vector<string> ContentValues::keyList(){
    vector<string> keys;
    for (std::multimap<string,string>::iterator it=begin(); it!=end(); ++it)
      keys.push_back(it->first);
    return keys;
  }
  vector<string> ContentValues::valueList(){
    vector<string> keys;
    for (std::multimap<string,string>::iterator it=begin(); it!=end(); ++it)
      keys.push_back(it->second);
    return keys;
  }

  void ContentValues::insert(string key,string value){multimap<string,string>::insert(pair<string,string>(key,value));}
  void ContentValues::insert(string key,double value){multimap<string,string>::insert(pair<string,string>(key,to_string(value)));}
  void ContentValues::insert(string key,float value){multimap<string,string>::insert(pair<string,string>(key,to_string(value)));}
  void ContentValues::insert(string key,int value){multimap<string,string>::insert(pair<string,string>(key,to_string(value)));}
  void ContentValues::insert(string key,long value){multimap<string,string>::insert(pair<string,string>(key,to_string(value)));}
  void ContentValues::insert(string key){multimap<string,string>::insert(pair<string,string>(key,""));}

  std::string ContentValues::serialize(){
    ostringstream os;
    for (multimap<string,string>::iterator it=begin(); it!=end(); ++it){
      os<<it->first<<"|"<<it->second<<"\n";
    }
    string t = os.str();
    return t;
  }
  int ContentValues::deserialize(std::string blob){
    istringstream is(blob);
    std::string value;
    std::string key;
    int a=0;
    while(std::getline(is,value,'|')){
      if(a%2) {
        insert(key,value);
      } else {
        key = value;
      }
      a++;
    }
    if(a%2){
      clear();
      return 1;
    }
    return 0;
  }
