#ifndef _FfmpegResources_h_
#define _FfmpegResources_h_

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavutil/avutil.h>
	#include <libavutil/opt.h>
}

#include <string>
#include <vector>

class FfmpegFormat
{
public:
	FfmpegFormat(std::string name, AVPixelFormat avFormat):name(name), avFormat(avFormat){}
	~FfmpegFormat(){}

	bool operator==(const std::string &thatName){return name == thatName;}
	bool operator==(const AVPixelFormat &thatAvFormat){return avFormat == thatAvFormat;}

	std::string name;
	AVPixelFormat avFormat;
};
typedef std::vector<FfmpegFormat> FfmpegFormats;

class FfmpegCodec
{
public:
	FfmpegCodec(AVCodecID id, std::string mime):id(id), mime(mime){}
	AVCodecID id;
	std::string mime;

	bool operator==(const AVCodecID &thatId){return id == thatId;}
	bool operator==(const std::string &thatMime){return mime == thatMime;}
};
typedef std::vector<FfmpegCodec> FfmpegCodecs;

class FfmpegResources
{
public:
	~FfmpegResources();

	static FfmpegResources &instance();
	FfmpegFormats formats(){return m_formats;}
	AVPixelFormat getAvPixelFormat(std::string name);
	std::string getAvPixelFormatName(AVPixelFormat format);

	FfmpegCodecs codecs(){return m_codecs;}
	AVCodecID getAvCodecID(std::string name);
	std::string getAvCodecMime(AVCodecID id);

	void registerAll();
private:
	FfmpegResources();

	static FfmpegResources *s_instance;

	bool m_registered;
	FfmpegFormats m_formats;
	FfmpegCodecs m_codecs;
};


template <class PluginObject> void buildAttribute(PluginObject *object, const AVOption *option)
{
	void *dst=((uint8_t*)object)+option->offset;

	switch(option->type)
	{
	case AV_OPT_TYPE_FLAGS:
		object->addAttribute(option->name, *(int *)dst);
		break;
//	case AV_OPT_TYPE_PIXEL_FMT:
//	case AV_OPT_TYPE_SAMPLE_FMT:
	case AV_OPT_TYPE_INT:       
	case AV_OPT_TYPE_INT64:
		object->addAttribute(option->name, *(int *)dst);
		break;
	case AV_OPT_TYPE_FLOAT:
	case AV_OPT_TYPE_DOUBLE:
		object->addAttribute(option->name, *(double *)dst);
		break;
	case AV_OPT_TYPE_STRING:
		object->addAttribute(option->name, *(char **)dst);
		break;
//	case AV_OPT_TYPE_RATIONAL:  *intnum = ((AVRational*)dst)->num;
//	                            *den    = ((AVRational*)dst)->den;
//	                                                    return 0;
//	case AV_OPT_TYPE_CONST:     *num    = o->default_val.dbl; return 0;
	}
}
#endif //_FfmpegResources_h_