#pragma once

#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <map>
#include <string>

class Settings {

private:

std::map<std::string, float> settingsList;
std::string myFilepath;

public:
  
  Settings() = default;
  bool initialise(std::string filepath);
  bool setElement(std::string name, float value);
  float getElement(std::string name);
  void saveToFile();

};

#endif
