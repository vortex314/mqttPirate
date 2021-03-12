#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <ArduinoJson.h>
#include <Log.h>

class Configuration
{

public:
    static bool load(JsonDocument& ,JsonObject& ,const char* fileName);
    static bool save(JsonDocument& ,JsonObject& ,const char* fileName);
};

#endif // CONFIGURATION_H
