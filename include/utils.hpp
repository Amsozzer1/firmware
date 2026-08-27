#pragma once
#include <esp_random.h>
#include <string.h>
#include <iostream>
inline std::string generateUuidV4() {
    uint8_t b[16];
    esp_fill_random(b, sizeof b);
    b[6] = (b[6] & 0x0F) | 0x40;
    b[8] = (b[8] & 0x3F) | 0x80;
    char s[37];
    snprintf(s, sizeof s,
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],
        b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]);
    return std::string(s);
}
