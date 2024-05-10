#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

extern "C" {
#include <libavcodec/avcodec.h>
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug() << "ffmpeg信息: " << av_version_info();
    return a.exec();
}
