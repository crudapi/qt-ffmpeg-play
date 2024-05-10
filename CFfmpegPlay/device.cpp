#include "device.h"
#include <QDebug>

#include "ffmpegheader.h"

Device::Device()
{

}


void Device::registerAll()
{
    avdevice_register_all();
    qInfo() << "av_version_info:" << av_version_info();
}

QMap<string, string> Device::list(DeviceType deviceType)
{
    QMap<string, string> qmap;
    const AVInputFormat* iFormat = av_find_input_format("dshow");
    AVDeviceInfoList* devList = nullptr;
    avdevice_list_input_sources(iFormat, nullptr, nullptr, &devList);
    if (devList != nullptr)
    {
        qDebug() << QString::number(devList->nb_devices);
        for ( int i = 0; i < devList->nb_devices; i++)
        {
            AVDeviceInfo* devInfo = devList->devices[i];
            enum AVMediaType type = *(devInfo->media_types);
            if (deviceType == DeviceType::audio)
            {
                if (type == AVMEDIA_TYPE_AUDIO) {
                    qmap[devInfo->device_name] = devInfo->device_description;
                }
            }
            else if (deviceType == DeviceType::video)
            {
                if (type == AVMEDIA_TYPE_VIDEO) {
                    qmap[devInfo->device_name] = devInfo->device_description;
                }
            }
            else {
                qmap[devInfo->device_name] = devInfo->device_description;
            }
        }
    }

    avdevice_free_list_devices(&devList);

    return qmap;
}

string Device::get(string key)
{
    const AVInputFormat* iFormat = av_find_input_format("dshow");
    AVDeviceInfoList* devList = nullptr;
    avdevice_list_input_sources(iFormat, nullptr, nullptr, &devList);
    if (devList != nullptr)
    {
        qDebug() << QString::number(devList->nb_devices);
        string devicePrefix = "";
        for ( int i = 0; i < devList->nb_devices; i++)
        {
            AVDeviceInfo* devInfo = devList->devices[i];
            char* device_name = devInfo->device_name;
            if (key != device_name) {
                continue;
            }

            char* device_description = devInfo->device_description;
            enum AVMediaType type = *(devInfo->media_types);

            if (type == AVMEDIA_TYPE_AUDIO) {
                devicePrefix = "audio=";
            }
            else if (type == AVMEDIA_TYPE_VIDEO) {
                devicePrefix = "video=";
            }
            devicePrefix += device_description;
            break;
        }

        avdevice_free_list_devices(&devList);
        return devicePrefix;
    }

    return nullptr;
}
