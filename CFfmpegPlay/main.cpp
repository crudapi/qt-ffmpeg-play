#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavutil/opt.h>
#include "libavutil/imgutils.h"
#include <libavutil/time.h>
#include "libavutil/pixdesc.h"
#include "libavutil/hash.h"
#include "libavutil/bswap.h"
}

using namespace std;

void show_dshow_device()
{
    qDebug() << "ffmpeg信息: " << av_version_info();
    avdevice_register_all();
    AVFormatContext* pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options, "list_devices", "true", 0);
    const AVInputFormat* iformat = av_find_input_format("dshow");
    qDebug() << "========Enum Device Info begin=============\n";
    avformat_open_input(&pFormatCtx, "video=dummy", iformat, &options);
    qDebug() << "========Enum Device Info end=============\n";
    avformat_free_context(pFormatCtx);

    AVDeviceInfoList *devList = nullptr;
    avdevice_list_input_sources(iformat, nullptr, nullptr, &devList);
    if (devList != nullptr)
    {
        qDebug() << QString::number(devList->nb_devices);
        for ( int i = 0; i < devList->nb_devices; i++)
        {
            AVDeviceInfo* devInfo = devList->devices[i];
            enum AVMediaType type = *devInfo->media_types;
            if (type == AVMEDIA_TYPE_AUDIO)
            {
                qDebug() << devInfo->device_name;
                qDebug() << devInfo->device_description;
            }
            else
            {
                qDebug() << devInfo->device_name;
                qDebug() << devInfo->device_description;
            }
        }
    }
    else
    {
        qDebug() << "没有找到设备";
    }
}

int main(int argc, char *argv[])
{
    show_dshow_device();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

