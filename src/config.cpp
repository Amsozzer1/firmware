#include "config.h"


Config::Config(char* raw_config){
    {/*IDK WHAT DATA IT TAKES YET AND WHAT IT ENTAILS*/};
    ArduinoJson::deserializeJson(this->config, raw_config);
    this->config["printerId"];
}

void Config::read_file(char* fileName) {
    
}