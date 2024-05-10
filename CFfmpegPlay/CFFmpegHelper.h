#ifndef CFFMPEGHELPER_H
#define CFFMPEGHELPER_H

using namespace std;
namespace FFmpegHelper
{
    void registerAll();
    char* listDevice(string name);
    char* getDevice(string name);
}

#endif // CFFMPEGHELPER_H
