#include "FfmpegDecoder.h"

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

FfmpegDecoder::FfmpegDecoder(std::string name, SharedMediaFilter parent):
MediaAutoRegister(name, parent),
m_currentVideoCodec(0),
m_videoCodec(NULL),
m_frame(NULL)
{
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_MJPEG, "image/mjpeg"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_MPEG2VIDEO, "video/mpeg"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_MPEG4, "video/mp4"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_H264, "video/h264"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_THEORA, "video/theora"));
//
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_MP3, "audio/mp3"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_AAC, "audio/aav"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_AC3, "audio/ac3"));
//	m_accessibleCodecs.push_back(CodecDetail(AV_CODEC_ID_VORBIS, "image/vorbis"));
}

FfmpegDecoder::~FfmpegDecoder()
{

}

bool FfmpegDecoder::initialize(const Attributes &attributes)
{
	FfmpegResources::instance().registerAll();

	m_imageSampleId=MediaSampleFactory::instance().getTypeId("ImageSample");
	m_bufferSampleId=MediaSampleFactory::instance().getTypeId("BufferSample");

	queryCodecs();

	//Use h264 by default
//	int index=getVideoEncoderIndexFromId(AV_CODEC_ID_H264);
//	int index=getVideoEncoderIndexFromId(AV_CODEC_ID_MPEG2VIDEO);
//	
//	if(index != -1)
//		m_currentVideoCodec=index;
//	else
//		m_currentVideoCodec=0;
//
//	CodecDescription &codecDescription=m_videoCodecs[m_currentVideoCodec];
//	Strings videoCodecs;
//
//	for(int i=0; i<m_videoCodecs.size(); ++i)
//	{
//		videoCodecs.push_back(m_videoCodecs[i].name);
//	}
//	addAttribute("videoCodec", codecDescription.name, videoCodecs);
//
//	Strings audioCodecs;
//
//	for(int i=0; i<m_audioCodecs.size(); ++i)
//	{
//		audioCodecs.push_back(m_audioCodecs[i].name);
//	}
//	addAttribute("audioCodec", audioEncoders[0], audioCodecs);
//
//	queryFormats();
//
//	Strings avFormats;
//	for(int i=0; i<m_avFormats.size(); ++i)
//	{
//		avFormats.push_back(m_avFormats[i].name);
//	}
//
//	//default mp4
//	std::string avFormatName="mp4";
//	Strings::iterator iter=std::find(avFormats.begin(), avFormats.end(), "mp4");
//
//	if(iter == avFormats.end())
//		avFormatName=avFormats[0];
//
//	addAttribute("outputFormat", avFormatName, avFormats);
//	addAttribute("outputLocation", "test.mpg");
//
	std::string sinkPadDescription="[";
	bool first=true;

	foreach(const CodecDescription &description, m_videoCodecs)
	{
		if(first)
		{
			sinkPadDescription+=(boost::format("{\"mime\":\"video/%s\"}")%description.name).str();
			first=false;
		}
		else
			sinkPadDescription+=(boost::format(", {\"mime\":\"video/%s\"}")%description.name).str();
	}
	sinkPadDescription+="]";

	addSinkPad(sinkPadDescription);

	updateVideoCodecAttributes();
	return true;
}

SharedPluginView FfmpegDecoder::getView()
{
	if(m_view == SharedPluginView())
	{
//		FfmpegControls *controls=new FfmpegControls(this);
//		m_view.reset(new QtPluginView(controls));
	}
	return m_view;
}

bool FfmpegDecoder::processSample(SharedMediaPad sinkPad, SharedMediaSample sample)
{
	if(sample->type() == m_imageSampleId)
	{
		SharedImageSample imageSample=boost::dynamic_pointer_cast<ImageSample>(sample);

		if(imageSample == SharedImageSample())
			return false;

		int output;
		int avError;
	//	pushSample(sample);
		SharedMediaSample sourceSample=newSample(m_bufferSampleId);
		SharedBufferSample bufferSample=boost::dynamic_pointer_cast<BufferSample>(sourceSample);

		bufferSample->allocate(imageSample->width()*imageSample->height()*3);

		int imageSize=(imageSample->size() < m_avFrameSize)?imageSample->size():m_avFrameSize;
		
		if(imageSize <= 0)
			return false;

//		savePPM("test.ppm", imageSample->buffer(), imageSample->width(), imageSample->height());
//		memcpy(m_frame->data[0], imageSample->buffer(), imageSize);
//		m_frame->data[0]=imageSample->buffer();
		uint8_t *bufferArray[1];
		int bufferSizeArray[1];

		bufferArray[0]=imageSample->buffer();
		bufferSizeArray[0]=imageSample->width()*3;

//		sws_scale(m_swsContext, bufferArray, bufferSizeArray, imageSample->width(), imageSample->height(), m_frame->data, m_frame->linesize);
		sws_scale(m_swsContext, bufferArray, bufferSizeArray, 0, imageSample->height(), m_frame->data, m_frame->linesize);

		m_pkt.data=bufferSample->buffer();
		m_pkt.size=bufferSample->actualSize();

		avError=avcodec_encode_video2(m_videoCodec, &m_pkt, m_frame, &output);

		if(output == 1)
		{
			bufferSample->setSize(m_pkt.size);
			pushSample(bufferSample);
		}

		deleteSample(sample);
	}
	return true;
}

IMediaFilter::StateChange FfmpegDecoder::onReady()
{
//	if((m_currentVideoCodec < 0) || (m_currentVideoCodec >= m_videoCodecs.size()))
//		return;
//	std::string videoEncoder=attribute("videoEncoder")->toString();
//	std::string audioEncoder=attribute("audioEncoder")->toString();s
//
//	if(m_videoCodecs[m_currentVideoCodec].name != videoEncoder)
//		m_currentVideoCodec=getVideoEncoderIndex(videoEncoder);
//
	SharedMediaPads sinkMediaPads=getSinkPads();

	if(sinkMediaPads.size() <= 0)
		return FAILED;

//	SharedMediaPad sinkMediaPad=sinkMediaPads[0];
//
//	if(!sinkMediaPad->linked())
//		return FAILED;

//	SharedMediaPads sourceMediaPads=getSourcePads();
//
//	if(sourceMediaPads.size() <= 0)
//		return FAILED;
//
	foreach(SharedMediaPad sinkPad, sinkMediaPads)
	{
		if(!sinkPad->linked())
			continue;

		SharedMediaFormat mediaFormat=sinkPad->format();

		if(!mediaFormat->exists("mime"))
			continue;

		std::string mime=mediaFormat->attribute("mime")->toString();

		if(mime.compare(0, 5, "video"))
		{
			std::string type=mime.substr(6, mime.size()-6);

			foreach(const CodecDescription &description, m_videoCodecs)
			{
				if(type == description.name)
				{
					SharedMediaFormat mediaFormat(new MediaFormat());

					mediaFormat->addAttribute("mime", "video/raw");

					addSourcePad(mediaFormat);
				}
			}
		}
	}
//	if((m_currentVideoCodec >= 0) && (m_currentVideoCodec < m_videoCodecs.size()))
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
//				m_frame->format=m_videoCodec->pix_fmt;
//				m_frame->width=m_videoCodec->width;
//				m_frame->height=m_videoCodec->height;
//			}
//
////			avpicture_alloc(&m_picture, m_videoCodec->pix_fmt, width, height);
////			*((AVPicture *)m_frame)=m_picture;
//			m_avFrameSize=av_image_alloc(m_frame->data, m_frame->linesize, m_frame->width, m_frame->height, (AVPixelFormat)m_frame->format, 32);		
//
//			av_init_packet(&m_pkt);
//
//			m_swsContext=sws_getContext(width, height, AV_PIX_FMT_BGR24, width, height, m_videoCodec->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);
//		}
//	}
	return SUCCESS;
}

IMediaFilter::StateChange FfmpegDecoder::onPaused()
{
	return SUCCESS;
}

IMediaFilter::StateChange FfmpegDecoder::onPlaying()
{
	return SUCCESS;
}

bool FfmpegDecoder::onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SINK)
	{
		return IMediaFilter::onAcceptMediaFormat(pad, format);
	}
	else if(pad->type() == MediaPad::SOURCE)
		return true;
	return false;
}

void FfmpegDecoder::onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SINK)
	{
		if(!format->exists("mime"))
			return;
		if((format->attribute("mime")->toString() != "video/raw") &&
			(format->attribute("mime")->toString() != "image/raw"))
			return;

//		CodecDetails::iterator iter=std::find(m_accessibleCodecs.begin(), m_accessibleCodecs.end(), m_videoCodecs[m_currentVideoCodec].id);
		FfmpegCodecs codecs=FfmpegResources::instance().codecs();

		FfmpegCodecs::iterator iter=std::find(codecs.begin(), codecs.end(), m_videoCodecs[m_currentVideoCodec].id);

//		if(iter != m_accessibleCodecs.end())
		if(iter != codecs.end())
		{
			MediaFormat sourceFormat;

			sourceFormat.addAttribute("mime", iter->mime);
			if(format->exists("width"))
			{
				sourceFormat.addAttribute("width", format->attribute("width")->toString());
				m_videoCodec->width=format->attribute("width")->toInt();
			}
			if(format->exists("height"))
			{
				sourceFormat.addAttribute("height", format->attribute("height")->toString());
				m_videoCodec->height=format->attribute("height")->toInt();
			}

//			sourceFormat.addAttribute("bitrate", 3000000);
//			sourceFormat.addAttribute("timeBaseNum", 1);
//			sourceFormat.addAttribute("timeBaseDen", 15);
//			sourceFormat.addAttribute("keyframeRate", 10);
//			sourceFormat.addAttribute("format", "YUV420P");
			sourceFormat.addAttribute("bitrate", m_videoCodec->bit_rate);
			sourceFormat.addAttribute("timeBaseNum", m_videoCodec->time_base.num);
			sourceFormat.addAttribute("timeBaseDen", m_videoCodec->time_base.den);
			sourceFormat.addAttribute("keyframeRate", m_videoCodec->gop_size);
			sourceFormat.addAttribute("format", "YUV420P");

//			//default settings
//			m_videoCodec->bit_rate = 400000;
//			//frames per second
//			AVRational rational={1, 15};
//			m_videoCodec->time_base=rational;
////			m_videoCodec->time_base= (AVRational){1,15};
//			m_videoCodec->gop_size=10; /* emit one intra frame every ten frames */
//			m_videoCodec->max_b_frames=1;
//			m_videoCodec->pix_fmt=AV_PIX_FMT_YUV420P;

			SharedMediaPads sourcePads=getSourcePads();

			BOOST_FOREACH(SharedMediaPad &sourcePad, sourcePads)
			{
				sourcePad->setFormat(sourceFormat);
			}
		}
	}
}

void FfmpegDecoder::onAttributeChanged(std::string name, SharedAttribute attribute)
{
//	if(name == "videoEncoder")
//	{
//		std::string videoEncoder=attribute->toString();
//
//		if(m_videoCodecs[m_currentVideoCodec].name != videoEncoder)
//			m_currentVideoCodec=getVideoEncoderIndex(videoEncoder);
//		updateVideoCodecAttributes();
//	}
}

void FfmpegDecoder::queryCodecs()
{	
	AVCodec *avCodec=NULL;
	int index=0;

	while((avCodec=av_codec_next(avCodec)) != NULL)
	{
//		CodecDetails::iterator iter=std::find(m_accessibleCodecs.begin(), m_accessibleCodecs.end(), avCodec->id);
//
//		if(iter != m_accessibleCodecs.end())
		std::string mime=FfmpegResources::instance().getAvCodecMime(avCodec->id);

		if(mime != "None")
		{
			if(avCodec->decode == NULL)
				continue;

			if(avCodec->type == AVMEDIA_TYPE_VIDEO)
			{
				m_videoCodecs.push_back(CodecDescription(avCodec));
				index++;
			}
			else if(avCodec->type == AVMEDIA_TYPE_AUDIO)
				m_audioCodecs.push_back(CodecDescription(avCodec));
		}
	}

//	updateVideoCodecAttributes();
}

int FfmpegDecoder::getVideoCodecIndex(std::string name)
{
	for(size_t i=0; i<m_videoCodecs.size(); ++i)
	{
		if(m_videoCodecs[i].name == name)
			return i;
	}
	return -1;
}

int FfmpegDecoder::getVideoCodecIndexFromId(AVCodecID id)
{
	for(size_t i=0; i<m_videoCodecs.size(); ++i)
	{
		if(m_videoCodecs[i].id == id)
			return i;
	}
	return -1;
}

int FfmpegDecoder::getAudioCodecIndex(std::string name)
{
	for(size_t i=0; i<m_audioCodecs.size(); ++i)
	{
		if(m_audioCodecs[i].name == name)
			return i;
	}
	return -1;
}

void FfmpegDecoder::updateVideoCodecAttributes()
{
	if(m_videoCodec != NULL)
	{
		avcodec_close(m_videoCodec);
		av_free(m_videoCodec);
		m_videoCodec=NULL;
	}

	//Backup current attributes to set new
	Attributes attributesBackup=attributes();
	Attributes &localAttributes=attributes();

	for(Attributes::iterator iter=localAttributes.begin(); iter!=localAttributes.end(); )
	{
		if(iter->first.compare(0, 6, "codec_") == 0)
			iter=localAttributes.erase(iter);
		else
			++iter;
	}

	if((m_currentVideoCodec >= 0) && (m_currentVideoCodec < m_videoCodecs.size()))
	{
		AVCodec *avCodec=m_videoCodecs[m_currentVideoCodec].avCodec;
		int avError;

		m_videoCodec=avcodec_alloc_context3(avCodec);

		AVDictionary *avDictionary=NULL;

		//default settings
		m_videoCodec->bit_rate = 3000000;

		m_videoCodec->width=1920;
		m_videoCodec->height=1080;
		//frames per second
		AVRational rational={1, 15};
		m_videoCodec->time_base=rational;
		//			m_videoCodec->time_base= (AVRational){1,15};
		m_videoCodec->gop_size=10; /* emit one intra frame every ten frames */
		m_videoCodec->max_b_frames=1;
		m_videoCodec->pix_fmt=AV_PIX_FMT_YUV420P;
	//	av_dict_set(&avDictionary, "", "", 0);
		char errorStr[64];

		if((avError=avcodec_open2(m_videoCodec, avCodec, &avDictionary)) < 0)
		{
			

			av_strerror(avError, errorStr, 64);
		}

		const AVOption *option=NULL;

		while((option=av_opt_next(m_videoCodec, option)) != NULL)
		{
			m_videoOptions.push_back(CodecOption(option));

			buildAttribute(m_videoCodec, option, "codec_");
		}
	}
}

//void FfmpegDecoder::queryFormats()
//{
//	AVOutputFormat *avFormat=NULL;
//
//	while((avFormat=av_oformat_next(avFormat)) != NULL)
//	{
//		Strings::iterator iter=std::find(m_accessibleFormats.begin(), m_accessibleFormats.end(), avFormat->name);
//
//		if(iter != m_accessibleFormats.end())
//			m_avFormats.push_back(FormatDescription(avFormat));
//	}
//}
//
//void FfmpegDecoder::setupFormat()
//{
//	//	if((m_currentFormat < 0) || (m_currentFormat >= m_avFormats.size()))
//	//		return;
//	//
//	//	FormatDescription &format=m_avFormats[m_currentFormat];
//	if(!exists("outputFormat"))
//		return;
//	if(!exists("outputLocation"))
//		return;
//
//	std::string formatName=attribute("outputFormat")->toString();
//	std::string location=attribute("outputLocation")->toString();
//
//	FormatDescriptions::iterator iter=std::find(m_avFormats.begin(), m_avFormats.end(), formatName);
//
//	if(iter == m_avFormats.end())
//		return;
//
//	avformat_alloc_output_context2(&m_avFormatContext, NULL, formatName.c_str(), location.c_str());
//
//	m_avOutputFormat=m_avFormatContext->oformat;
//
//	AVCodec *codec;
//
//	codec=avcodec_find_encoder(m_avOutputFormat->video_codec);
//
//	if(codec == NULL)
//		return;
//
//	m_videoStream=avformat_new_stream(m_avFormatContext, codec);
//
//	av_init_packet(&m_pkt);
//}