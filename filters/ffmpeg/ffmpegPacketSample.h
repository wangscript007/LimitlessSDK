#ifndef _FfmpegPacketSample_h_
#define _FfmpegPacketSample_h_

#include "Media/MediaSample.h"
#include "Media/MediaSampleFactory.h"

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
}

class FfmpegPacketSample:public Limitless::AutoRegisterMediaSample<FfmpegPacketSample, Limitless::MediaSample>
{
public:
	FfmpegPacketSample();
	virtual ~FfmpegPacketSample();

	AVPacket *getPacket(){return &m_packet;}
	void allocate(size_t size);

	int64_t presentationTimestamp(){return m_packet.pts;}
	int64_t decompressionTimestamp(){return m_packet.dts;}

	virtual MediaTime timestamp() const{return m_packet.pts;}

//MediaSample
	virtual unsigned char *buffer();
	virtual size_t size() const;
	virtual size_t actualSize() const;

private:
	void freeBuffer();

	AVPacket m_packet;
};
typedef boost::shared_ptr<FfmpegPacketSample> SharedFfmpegPacketSample;
#endif //_FfmpegPacketSample_h_