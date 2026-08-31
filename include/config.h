
#include <ArduinoJson.h>
// ONLY A DATA CLASS
class Config {
    public:
    JsonDocument config;
        
        Config(char*);
        void static read_file(char* );
};
