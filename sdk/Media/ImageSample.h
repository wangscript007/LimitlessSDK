#ifndef _ImageSample_h_
#define _ImageSample_h_

#include "Media/media_define.h"
#include "Media/IImageSample.h"
#include "Media/MediaSampleFactory.h"

namespace Limitless
{

class MEDIA_EXPORT ImageSample:public AutoRegisterMediaSample<ImageSample, IImageSample>
{
public:
	ImageSample();
	ImageSample(int width, int height, int channles=3, int channelBits=8);
	ImageSample(int width, int pitch, int height, int channles, int channelBits);
	ImageSample(unsigned char *buffer, int width, int height, int channles=3, int channelBits=8);
	ImageSample(unsigned char *buffer, int width, int pitch, int height, int channles, int channelBits);
	virtual ~ImageSample();

//MediaSample
	virtual unsigned char *buffer(){return m_buffer;}
	virtual size_t size() const{return m_size;}

//IImageSample
	virtual std::string imageFormat() const{return "RGB";}
	virtual int width() const{return m_width;}
	virtual int pitch() const{return m_pitch;}
	virtual int height() const{return m_height;}
	virtual int channels() const{return m_channels;}
	virtual int channelBits() const{return m_channelBits;}

	void setImage(unsigned char *buffer, int width, int height, int channles=3, int channelBits=8);
	void setImage(unsigned char *buffer, int width, int pitch, int height, int channles, int channelBits);
	void resize(int width, int height, int channles=3, int channelBits=8);
	void resize(int width, int pitch, int height, int channles, int channelBits);

private:
	void freeBuffer();

	bool m_externalBuffer;
	unsigned char *m_buffer;
	size_t m_size;

	int m_width, m_pitch;
	int m_height;

	int m_channels;
	int m_channelBits;
};
typedef boost::shared_ptr<ImageSample> SharedImageSample;

}//namespace Limitless

#endif //_ImageSample_h_