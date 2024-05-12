#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ffmpegheader.h"
#include "device.h"
#include <QDebug>
#include <QListView>
#include <QStringListModel>

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
    QMap<string, string> videoMap = m_device->list(DeviceType::video);

    for (QMap<string, string>::const_iterator itor = videoMap.constBegin(); itor != videoMap.constEnd(); ++itor)
    {
        ui->deviceComboBox->addItem(itor.value().c_str(), itor.key().c_str());
    }

    QMap<string, string> audioMap = m_device->list(DeviceType::audio);
    QStringList dataList = {};
    for (QMap<string, string>::const_iterator itor = audioMap.constBegin(); itor != audioMap.constEnd(); ++itor)
    {
        dataList.push_back(QString::fromStdString(itor.value()));
    }
    QStringListModel *model = new QStringListModel(dataList);
    ui->audioListView->setModel(model);
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


    av_dict_set(&dict, "framerate", ui->fpsEdit->text().toStdString().c_str(), 0);
    av_dict_set(&dict, "pixel_format", ui->pixelFormatEdit->text().toStdString().c_str(), 0);//yuyv422

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
    qDebug() << " output pix_fmt=" << av_get_pix_fmt_name((AVPixelFormat)video_stream->codecpar->format) <<" "<< video_stream->codecpar->format << endl;
}
