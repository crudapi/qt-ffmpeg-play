#ifndef DEVICE_H
#define DEVICE_H

#include<QMap>
#include<string>
using namespace std;

enum class DeviceType {
    audio = 1,  // 音频
    video = 2,   // 视频
    all = 3     // 音频视频
};

class Device
{
public:
    Device();
    void registerAll();
    QMap<string, string> list(DeviceType deviceType = DeviceType::all);
    string get(string key);
};

#endif // DEVICE_H
