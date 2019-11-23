#ifndef NOODLE_SIMPLECONFIG_H_
#define NOODLE_SIMPLECONFIG_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <string>
#include <ctype.h>

class SimpleConfig {

  std::string fname;
  std::map<std::string,std::string> store;

public:
  SimpleConfig(){}
  void setpath(std::string file){fname=file;parse();}
  std::string getvalue(std::string key){return store.find(key)->second;}
  int getintvalue(std::string key){return stoi(getvalue(key));}
  bool hasKey(std::string key){return store.find(key)==store.end()?false:true;}

protected:

  int parse(){
    std::ifstream cFile (fname);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
                                 line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");

            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            store.insert(std::pair<std::string,std::string>(name,value));
        }
        cFile.close();
    }
    else
        std::cout << "Unable to open config file." << '\n';
    return 0;
  }




};

#endif
