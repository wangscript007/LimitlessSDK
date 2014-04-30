#include "FfmpegOutput.h"

#include "Base/QtPluginView.h"
#include "Media/MediaPad.h"
#include "Media/MediaSampleFactory.h"
#include "Media/ImageSample.h"
#include "Media/BufferSample.h"

#include "ffmpegResources.h"
#include "ffmpegControls.h"

#include <boost/foreach.hpp>

extern "C"
{
	#include <libavutil/avutil.h>
	#include <libavutil/opt.h>
	#include <libavutil/imgutils.h>
}

using namespace Limitless;
//FormatDescription::FormatDescription(AVOutputFormat *avFormat):
//avFormat(avFormat)
//{
//	name=avFormat->name;
//	fullName=avFormat->long_name;
//}

FfmpegOutput::FfmpegOutput(std::string name, SharedMediaFilter parent):
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

FfmpegOutput::~FfmpegOutput()
{

}

bool FfmpegOutput::initialize(const Attributes &attributes)
{
	FfmpegResources::instance().registerAll();

	queryFormats();

	Strings avFormats;
	for(int i=0; i<m_avFormats.size(); ++i)
	{
		avFormats.push_back(m_avFormats[i].name);
	}

	//default mp4
//	std::string avFormatName="mp4";
//	std::string avFormatName="h264";
	std::string avFormatName="f4v";
//	std::string avFormatName="flv";
	Strings::iterator iter=std::find(avFormats.begin(), avFormats.end(), avFormatName);

	if(iter == avFormats.end())
		avFormatName=avFormats[0];
	
	addAttribute("outputFormat", avFormatName, avFormats);
//	addAttribute("outputLocation", "test.mp4");
//	addAttribute("outputLocation", "test.h264");
	addAttribute("outputLocation", "rtmp://mediamixmaster.com/live/test");

	m_bufferSampleId=MediaSampleFactory::instance().getTypeId("BufferSample");

	addSinkPad("[{\"mime\":\"video/*\"}, {\"mime\":\"image/*\"}]");
//	addSinkPad("[{\"mime\":\"audio/*\"}]");

	return true;
}

SharedPluginView FfmpegOutput::getView()
{
//	if(m_view == SharedPluginView())
//	{
//		FfmpegControls *controls=new FfmpegControls(this);
//		m_view.reset(new QtPluginView(controls));
//	}
//	return m_view;
	return SharedPluginView();
}

bool FfmpegOutput::processSample(SharedMediaPad sinkPad, SharedMediaSample sample)
{
	int avError;

	if(m_firstSample)
	{
		std::string location=attribute("outputLocation")->toString();

		if(!(m_avOutputFormat->flags & AVFMT_NOFILE))
		{
			avError=avio_open(&m_avFormatContext->pb, location.c_str(), AVIO_FLAG_WRITE);
		}

		avError=avformat_write_header(m_avFormatContext, NULL);
		m_firstSample=false;
	}
//	if(sample->type() == m_bufferSampleId)
	{
		m_pkt.data=sample->buffer();
		m_pkt.size=sample->size();
		avError=av_interleaved_write_frame(m_avFormatContext, &m_pkt);
	}
	deleteSample(sample);
	return true;
}

IMediaFilter::StateChange FfmpegOutput::onReady()
{
//	if((m_currentVideoEncoder < 0) || (m_currentVideoEncoder >= m_videoCodecs.size()))
//		return;
//	std::string videoEncoder=attribute("videoEncoder")->toString();
//	std::string audioEncoder=attribute("audioEncoder")->toString();
//
//	if(m_videoCodecs[m_currentVideoEncoder].name != videoEncoder)
//		m_currentVideoEncoder=getVideoEncoderIndex(videoEncoder);
//
	SharedMediaPads sinkMediaPads=getSinkPads();

	if(sinkMediaPads.size() <= 0)
		return FAILED;

	SharedMediaPad sinkMediaPad=sinkMediaPads[0];

	if(!sinkMediaPad->linked())
		return FAILED;

	if(!exists("outputLocation"))
		return FAILED;

	int avError;
	std::string location=attribute("outputLocation")->toString();

	setupFormat();

//	if(!(m_avOutputFormat->flags & AVFMT_NOFILE))
//	{
//		avError=avio_open(&m_avFormatContext->pb, location.c_str(), AVIO_FLAG_WRITE);
//	}
//	avError=avformat_write_header(m_avFormatContext, NULL);

//	if((m_currentVideoEncoder >= 0) && (m_currentVideoEncoder < m_videoCodecs.size()))
//	{
//		SharedMediaFormat format=sinkMediaPad->format();
//		int width=0, height=0;
//		int avError;
//
//		if(format->exists("width"))
//			width=format->attribute("width")->toInt();
//		if(format->exists("height"))
//			height=format->attribute("height")->toInt();
//
//		if((width != 0) && (height != 0))
//		{
//			if(m_frame == NULL)
//			{
//				m_frame=avcodec_alloc_frame();
//				
////				m_frame->format=AV_PIX_FMT_BGR24;
//				m_frame->format=m_videoEncoder->pix_fmt;
//				m_frame->width=m_videoEncoder->width;
//				m_frame->height=m_videoEncoder->height;
//			}
//
////			avpicture_alloc(&m_picture, m_videoEncoder->pix_fmt, width, height);
////			*((AVPicture *)m_frame)=m_picture;
//			m_avFrameSize=av_image_alloc(m_frame->data, m_frame->linesize, m_frame->width, m_frame->height, (AVPixelFormat)m_frame->format, 32);		
//
//			av_init_packet(&m_pkt);
//
//			m_swsContext=sws_getContext(width, height, AV_PIX_FMT_BGR24, width, height, m_videoEncoder->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);
//		}
//	}
	return SUCCESS;
}

IMediaFilter::StateChange FfmpegOutput::onPaused()
{
	return SUCCESS;
}

IMediaFilter::StateChange FfmpegOutput::onPlaying()
{
	return SUCCESS;
}

void FfmpegOutput::onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SINK)
	{
		if(format->exists("mime"))
			m_codecId=FfmpegResources::instance().getAvCodecID(format->attribute("mime")->toString());
		if(format->exists("bitrate"))
			m_bitrate=format->attribute("bitrate")->toInt();
		if(format->exists("timeBaseNum"))
			m_timeBase.num=format->attribute("timeBaseNum")->toInt();
		if(format->exists("timeBaseDen"))
			m_timeBase.den=format->attribute("timeBaseDen")->toInt();
		if(format->exists("keyframeRate"))
			m_keyframeRate=format->attribute("keyframeRate")->toInt();
		if(format->exists("format"))
			m_pixelFormat=FfmpegResources::instance().getAvPixelFormat(format->attribute("format")->toString());

		if(format->exists("width"))
			m_width=format->attribute("width")->toInt();
		if(format->exists("height"))
			m_height=format->attribute("height")->toInt();
	}
}

bool FfmpegOutput::onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SINK)
	{
		SharedMediaPads sinkPads=getSinkPads();
		SharedMediaPads::iterator iter=std::find(sinkPads.begin(), sinkPads.end(), pad);

		if(iter != sinkPads.end())
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

void FfmpegOutput::onAttributeChanged(std::string name, SharedAttribute attribute)
{
	if(name == "outputFormat")
	{
		std::string outputFormat=attribute->toString();

		for(int i=0; i<m_avFormats.size(); ++i)
		{
			if(m_avFormats[i].name == outputFormat)
			{
//				m_currentFormat=i;

				
			}
		}
	}
}

void FfmpegOutput::setupFormat()
{
//	if((m_currentFormat < 0) || (m_currentFormat >= m_avFormats.size()))
//		return;
//
//	FormatDescription &format=m_avFormats[m_currentFormat];
	if(!exists("outputFormat"))
		return;
	if(!exists("outputLocation"))
		return;

	std::string formatName=attribute("outputFormat")->toString();
	std::string location=attribute("outputLocation")->toString();

	FormatDescriptions::iterator iter=std::find(m_avFormats.begin(), m_avFormats.end(), formatName);

	if(iter == m_avFormats.end())
		return;

	avformat_alloc_output_context2(&m_avFormatContext, NULL, formatName.c_str(), location.c_str());

	m_avOutputFormat=m_avFormatContext->oformat;

	AVCodec *codec;

	codec=avcodec_find_encoder(m_codecId);

	if(codec == NULL)
		return;
	
	m_videoStream=avformat_new_stream(m_avFormatContext, codec);
//	m_videoStream=avformat_new_stream(m_avFormatContext, NULL);

	AVCodecContext *streamCodec=m_avFormatContext->streams[0]->codec;
	
	streamCodec->codec=codec;
	streamCodec->codec_id=m_codecId;
	streamCodec->bit_rate=m_bitrate;
	streamCodec->time_base=m_timeBase;
	streamCodec->width=m_width;
	streamCodec->height=m_height;
	streamCodec->gop_size=m_keyframeRate;
	streamCodec->pix_fmt=m_pixelFormat;

	av_init_packet(&m_pkt);
}

void FfmpegOutput::queryFormats()
{
	AVOutputFormat *avFormat=NULL;

	while((avFormat=av_oformat_next(avFormat)) != NULL)
	{
		Strings::iterator iter=std::find(m_accessibleFormats.begin(), m_accessibleFormats.end(), avFormat->name);

		if(iter != m_accessibleFormats.end())
			m_avFormats.push_back(FormatDescription(avFormat));
	}
}