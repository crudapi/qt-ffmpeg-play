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

char* show_dshow_device(int selectType)
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
            if (type == selectType)
            {
                qDebug() << devInfo->device_name;
                qDebug() << devInfo->device_description;
                return devInfo->device_description;
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

    return nullptr;
}

void capture()
{
    char* name = show_dshow_device(AVMEDIA_TYPE_AUDIO);
    qDebug() << name;

    AVFormatContext* m_pCaptureCtx = avformat_alloc_context();
    int total_count = 300;
    int captured_frame_count = 0;
    int fps;
    int64_t start_pts = 0;//for set start pts from 0

    const AVInputFormat* inputFormat = av_find_input_format("dshow");
    AVDictionary* dict = nullptr;

    //name = "video=XiaoMi USB 2.0 Webcam";
    //char audio[255] = "virtual-audio-capturer";
    char audio[255] = "audio=";
    int ret = avformat_open_input(&m_pCaptureCtx, strcat(audio, name), inputFormat, &dict);
    if (ret != 0)
    {
        qDebug() << ret;
    }

    ret = avformat_find_stream_info(m_pCaptureCtx, NULL);
    if (ret != 0)
    {
        qDebug() << ret;
    }

    int videoStreamId = -1;
    videoStreamId = av_find_best_stream(m_pCaptureCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);

    AVStream* video_stream = nullptr;
    video_stream = m_pCaptureCtx->streams[videoStreamId];
    //g_time_base = video_stream->time_base;
    fps = av_q2d(video_stream->avg_frame_rate);
    cout << "capture information:\n capture fps=" << fps << endl
       << " w=" << video_stream->codecpar->width << ", h=" << video_stream->codecpar->height
       << endl;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    capture();
    MainWindow w;
    w.show();
    return a.exec();
}

