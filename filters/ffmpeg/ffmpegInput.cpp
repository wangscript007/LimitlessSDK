#include "FfmpegInput.h"

#include "Base/QtPluginView.h"
#include "Media/MediaPad.h"
#include "Media/MediaSampleFactory.h"
#include "Media/ImageSample.h"
#include "Media/BufferSample.h"

#include "ffmpegResources.h"
#include "ffmpegControls.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

extern "C"
{
	#include <libavutil/avutil.h>
	#include <libavutil/opt.h>
	#include <libavutil/imgutils.h>
}

using namespace Limitless;

FfmpegInput::FfmpegInput(std::string name, SharedMediaFilter parent):
MediaAutoRegister(name, parent),
m_firstSample(true)
{
	m_accessibleFormats.push_back("f4v");
	m_accessibleFormats.push_back("flv");
	m_accessibleFormats.push_back("h264");
	m_accessibleFormats.push_back("hls");
	m_accessibleFormats.push_back("mp4");
	m_accessibleFormats.push_back("mpeg");
	m_accessibleFormats.push_back("mpeg2video");
	m_accessibleFormats.push_back("ogg");
	m_accessibleFormats.push_back("webm");
	m_accessibleFormats.push_back("mpegts");
	m_accessibleFormats.push_back("yuv4mpegpipe");
	m_accessibleFormats.push_back("rtp");
	m_accessibleFormats.push_back("rtsp");
}

FfmpegInput::~FfmpegInput()
{

}

bool FfmpegInput::initialize(const Attributes &attributes)
{
	FfmpegResources::instance().registerAll();

	queryFormats();

	Strings avFormats;
	for(int i=0; i<m_avFormats.size(); ++i)
	{
		avFormats.push_back(m_avFormats[i].name);
	}

	addAttribute("format", "");
	addAttribute("location", "");

	m_bufferSampleId=MediaSampleFactory::instance().getTypeId("BufferSample");

	av_init_packet(&m_pkt);
//	addSourcePad("[{\"mime\":\"video/*\"}, {\"mime\":\"image/*\"}]");

	return true;
}

SharedPluginView FfmpegInput::getView()
{
	return SharedPluginView();
}

bool FfmpegInput::processSample(SharedMediaPad sinkPad, SharedMediaSample sample)
{
	int avError;

//	if(m_firstSample)
//	{
//		std::string location=attribute("location")->toString();
//
//		if(!(m_avOutputFormat->flags & AVFMT_NOFILE))
//		{
//			avError=avformat_open_input(&m_avFormatContext, location.c_str(), NULL, NULL);
//		}
//		m_firstSample=false;
//	}

//	if(sample->type() == m_bufferSampleId)
	{
		m_pkt.data=sample->buffer();
		m_pkt.size=sample->size();
		avError=av_read_frame(m_avFormatContext, &m_pkt);
	}
	deleteSample(sample);
	return true;
}

IMediaFilter::StateChange FfmpegInput::onReady()
{
	if(!exists("location"))
		return FAILED;

	int avError;
	std::string location=attribute("location")->toString();

//	setupFormat();

	m_avFormatContext=avformat_alloc_context();

	if(m_avFormatContext == NULL)
		return FAILED;

	int error;
	AVInputFormat *fileInputFormat;

	error=avformat_open_input(&m_avFormatContext, location.c_str(), NULL, NULL);

	if(error < 0)
		return FAILED;

	error=avformat_find_stream_info(m_avFormatContext, nullptr);
	
	if(error < 0)
		return FAILED;
	
	for(unsigned int i = 0; i < m_avFormatContext->nb_streams; ++i)
	{
		auto stream=m_avFormatContext->streams[i];
		AVCodecContext * codecContext=stream->codec;
		auto codecType=codecContext->codec_type;
		auto codecID=codecContext->codec_id;

		const auto codec=avcodec_find_decoder(codecContext->codec_id);

		if(codecContext->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			SharedMediaFormat mediaFormat(new MediaFormat());
			std::string mimeType=(boost::format("video/%s")%codec->name).str();

			mediaFormat->addAttribute("mime", mimeType);

			addSourcePad(mediaFormat);
		}
	}

	return SUCCESS;
}

IMediaFilter::StateChange FfmpegInput::onPaused()
{
	return SUCCESS;
}

IMediaFilter::StateChange FfmpegInput::onPlaying()
{
	return SUCCESS;
}

void FfmpegInput::onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SOURCE)
	{
//		if(format->exists("mime"))
//			m_codecId=FfmpegResources::instance().getAvCodecID(format->attribute("mime")->toString());
//		if(format->exists("bitrate"))
//			m_bitrate=format->attribute("bitrate")->toInt();
//		if(format->exists("timeBaseNum"))
//			m_timeBase.num=format->attribute("timeBaseNum")->toInt();
//		if(format->exists("timeBaseDen"))
//			m_timeBase.den=format->attribute("timeBaseDen")->toInt();
//		if(format->exists("keyframeRate"))
//			m_keyframeRate=format->attribute("keyframeRate")->toInt();
//		if(format->exists("format"))
//			m_pixelFormat=FfmpegResources::instance().getAvPixelFormat(format->attribute("format")->toString());
//
//		if(format->exists("width"))
//			m_width=format->attribute("width")->toInt();
//		if(format->exists("height"))
//			m_height=format->attribute("height")->toInt();
	}
}

bool FfmpegInput::onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SOURCE)
	{
		SharedMediaPads pads=getSourcePads();
		SharedMediaPads::iterator iter=std::find(pads.begin(), pads.end(), pad);

		if(iter != pads.end())
		{
			if(format->exists("mime"))
			{
				std::string mime=format->attribute("mime")->toString();

				if(mime.compare(0, 5, "video") == 0)
					return true;
				if(mime.compare(0, 5, "image") == 0)
					return true;
			}
		}
	}
	return false;
}

void FfmpegInput::onAttributeChanged(std::string name, SharedAttribute attribute)
{
}

void FfmpegInput::queryFormats()
{
	AVOutputFormat *avFormat=NULL;

	while((avFormat=av_oformat_next(avFormat)) != NULL)
	{
		Strings::iterator iter=std::find(m_accessibleFormats.begin(), m_accessibleFormats.end(), avFormat->name);

		if(iter != m_accessibleFormats.end())
			m_avFormats.push_back(FormatDescription(avFormat));
	}
}