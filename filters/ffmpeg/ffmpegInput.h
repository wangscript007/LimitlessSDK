#ifndef _FFMPEG_OUTPUT_H
#define _FFMPEG_OUTPUT_H

#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"
#include "ffmpegMediaFilter.h"
#include "formatDescription.h"

extern "C"
{
	#include <libavformat/avformat.h>
}

class FfmpegInput:public MediaAutoRegister<FfmpegInput, FfmpegMediaFilter>
{
public:
	FfmpegInput(std::string name, SharedMediaFilter parent);
	~FfmpegInput();

	virtual bool initialize(const Attributes &attributes);
	virtual bool shutdown(){return true;}

	virtual SharedPluginView getView();

	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);

protected:
	//IMediaFilter
	virtual StateChange onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();

	virtual void onLinkFormatChanged(SharedMediaPad mediaPad, SharedMediaFormat format);
	virtual bool onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format);
	//PluginObject
	virtual void onAttributeChanged(std::string name, SharedAttribute attribute);

private:
	void setupFormat();
	void queryFormats();

	bool m_firstSample;

	SharedPluginView m_view;
	size_t m_imageSampleId;
	size_t m_bufferSampleId;

	Strings m_accessibleFormats;
	FormatDescriptions m_avFormats;
	int m_currentFormat;

	AVFormatContext *m_avFormatContext;
	AVOutputFormat *m_avOutputFormat;
	AVStream *m_videoStream;
	AVPacket m_pkt;

	AVCodecID m_codecId;
	int m_bitrate;
	AVRational m_timeBase;
	int m_keyframeRate;
	AVPixelFormat m_pixelFormat;
	int m_width;
	int m_height;
};

#endif // _FFMPEG_OUTPUT_H
