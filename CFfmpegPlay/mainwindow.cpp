#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ffmpegheader.h"
#include "device.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_device(new Device())
{
    ui->setupUi(this);

    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_device->registerAll();
    QMap<string, string> map = m_device->list();

    for (QMap<string, string>::const_iterator itor = map.constBegin(); itor != map.constEnd(); ++itor)
    {
        ui->deviceComboBox->addItem(itor.value().c_str(), itor.key().c_str());
    }
}


void MainWindow::on_startButton_clicked()
{
    qDebug() << "on_startButton_clicked";

    QVariant data = ui->deviceComboBox->currentData();

    QByteArray byteArray = data.toByteArray();
    std::string str(byteArray.constData(), byteArray.length());

    string deviceName = m_device->get(str);

    qDebug() << QString::fromStdString(deviceName);

    this->capture(deviceName);
}

void MainWindow::on_stopButton_clicked()
{
    qDebug() << "on_stopButton_clicked";
}

void MainWindow::capture(string deviceName)
{
    AVFormatContext* m_pCaptureCtx = avformat_alloc_context();
    int total_count = 300;
    int captured_frame_count = 0;
    int fps;
    int64_t start_pts = 0;//for set start pts from 0

    const AVInputFormat* inputFormat = av_find_input_format("dshow");
    AVDictionary* dict = nullptr;

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
    qDebug() << "capture information:\n capture fps=" << fps << endl
       << " w=" << video_stream->codecpar->width << ", h=" << video_stream->codecpar->height
       << endl;

}
