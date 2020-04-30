#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include "settings.h"

bool Settings::initialise(std::string filepath){

  this->myFilepath = filepath;

  std::ifstream file;
  file.open(filepath);

  if (file.eof()){
    return false;
  }

  std::string delimiter = "=";

  while (!file.eof()){

    std::string tempBuffer;
    std::getline(file, tempBuffer);

    //Checks if the line is a comment
    if(tempBuffer[0] == '#'){
      continue;
    }

    //EOF does not trigger as expected
    //Need to debug later
    if (tempBuffer.size() == 0){
      break;
    }

    std::string key = tempBuffer.substr(0, tempBuffer.find(delimiter));
    std::string tempValue = tempBuffer.substr(tempBuffer.find(delimiter) + 1, tempBuffer.size() - 1);

    float value = stof(tempValue);

    this->settingsList[key] = value;

    std::cout << key << ':' << tempValue << std::endl;

  }

  return true;

}

float Settings::getElement(std::string name){

  std::map<std::string, float>::iterator it;


  it = this->settingsList.find(name);

  if (it == this->settingsList.end()){

    return -1.f;

  }

  return this->settingsList[name.c_str()];

}

bool Settings::setElement(std::string name, float value){

  std::map<std::string, float>::iterator it;

  it = this->settingsList.find(name);

  if (it == this->settingsList.end()){

    return false;

  }

  this->settingsList[name] = value;

  this->saveToFile();

  return true;

}

void Settings::saveToFile(){

  std::ofstream file;
  file.open(myFilepath);

  for (auto const& it : this->settingsList) {

    file << it.first << "=" <<it.second << '\n';

  }

}
