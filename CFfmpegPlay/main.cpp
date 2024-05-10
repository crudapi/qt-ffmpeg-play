#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <iostream>
#include "device.h"

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


void capture()
{
    Device device;
    AVFormatContext* m_pCaptureCtx = avformat_alloc_context();
    int total_count = 300;
    int captured_frame_count = 0;
    int fps;
    int64_t start_pts = 0;//for set start pts from 0

    const AVInputFormat* inputFormat = av_find_input_format("dshow");
    AVDictionary* dict = nullptr;

    device.registerAll();
    QMap<string, string> map = device.list();

    string deviceName = device.get("");

    int ret = avformat_open_input(&m_pCaptureCtx, deviceName.c_str(), inputFormat, &dict);
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
    //capture();
    MainWindow w;
    w.show();
    return a.exec();
}

