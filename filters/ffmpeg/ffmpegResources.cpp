#include "ffmpegResources.h"
#include <boost/format.hpp>

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
}

#include "windows.h"

#ifndef DEBUG
void avLogCallback(void *ptr, int level, const char* format, va_list args)
{
	char logEntry[1024];

	vsnprintf(logEntry, 1022, format, args); 
	OutputDebugStringA(logEntry);
}
#endif //DEBUG

FfmpegResources *FfmpegResources::s_instance=0;

FfmpegResources::FfmpegResources():
m_registered(false)
{
	m_formats.push_back(FfmpegFormat("YUV420P", AV_PIX_FMT_YUV420P));
	m_formats.push_back(FfmpegFormat("YUYV422", AV_PIX_FMT_YUYV422));
	m_formats.push_back(FfmpegFormat("RGB24", AV_PIX_FMT_RGB24));
	m_formats.push_back(FfmpegFormat("BGR24", AV_PIX_FMT_BGR24));

	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_MPEG2VIDEO, "video/mpeg"));
	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_MPEG4, "video/mp4"));
	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_H264, "video/h264"));
	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_THEORA, "video/theora"));

	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_MP3, "audio/mp3"));
	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_AAC, "audio/aav"));
	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_AC3, "audio/ac3"));
	m_codecs.push_back(FfmpegCodec(AV_CODEC_ID_VORBIS, "image/vorbis"));
}

FfmpegResources &FfmpegResources::instance()
{
	if(s_instance == 0)
		s_instance=new FfmpegResources();

	return *s_instance;
}

AVPixelFormat FfmpegResources::getAvPixelFormat(std::string name)
{
	FfmpegFormats::iterator iter=std::find(m_formats.begin(), m_formats.end(), name);

	if(iter != m_formats.end())
		return iter->avFormat;
	return AV_PIX_FMT_NONE;
}

std::string FfmpegResources::getAvPixelFormatName(AVPixelFormat format)
{
	FfmpegFormats::iterator iter=std::find(m_formats.begin(), m_formats.end(), format);

	if(iter != m_formats.end())
		return iter->name;
	return "None";
}

AVCodecID FfmpegResources::getAvCodecID(std::string mime)
{
	FfmpegCodecs::iterator iter=std::find(m_codecs.begin(), m_codecs.end(), mime);

	if(iter != m_codecs.end())
		return iter->id;
	return AV_CODEC_ID_NONE;
}

std::string FfmpegResources::getAvCodecMime(AVCodecID id)
{
	FfmpegCodecs::iterator iter=std::find(m_codecs.begin(), m_codecs.end(), id);

	if(iter != m_codecs.end())
		return iter->mime;
	return "None";
}

void FfmpegResources::registerAll()
{
	if(!m_registered)
	{
		av_register_all();
		avformat_network_init()	;
	}


#ifndef DEBUG
	av_log_set_callback(avLogCallback);
#endif //DEBUG
}