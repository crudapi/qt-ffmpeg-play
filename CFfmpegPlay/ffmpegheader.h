#ifndef FFMPEGHEADER_H
#define FFMPEGHEADER_H

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

#endif // FFMPEGHEADER_H
