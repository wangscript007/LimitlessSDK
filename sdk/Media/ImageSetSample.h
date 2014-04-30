#ifndef _ImageSetSample_h_
#define _ImageSetSample_h_

#include <boost/shared_ptr.hpp>
#include "Media/media_define.h"
#include "Media/MediaSampleFactory.h"
#include "Media/IImageSetSample.h"
#include "Media/ImageSample.h"

namespace Limitless
{

#pragma warning(push)
#pragma warning(disable:4251)

class MEDIA_EXPORT ImageSetSample:public AutoRegisterMediaSample<ImageSetSample, IImageSetSample>
{
public:
	ImageSetSample();
	virtual ~ImageSetSample();

//MediaSample
	virtual unsigned char *buffer(){return m_buffer;}
	virtual size_t size() const{return m_size;}

//IImageSetSample
	virtual IImageSample *operator[](int index);
	virtual size_t setSize();

	void resize(unsigned int count, int width, int height, int channels=3, int channelBits=8);
	void resize(unsigned int count, int width, int pitch, int height, int channels, int channelBits);
	
private:
	void freeBuffer();

	std::vector<ImageSample *> m_imageSamples;
	unsigned char *m_buffer;
	int m_width;
	int m_pitch;
	int m_height;
	size_t m_size;
	int m_channels;
	int m_channelBits;
};
typedef boost::shared_ptr<ImageSetSample> SharedImageSetSample;

#pragma warning(pop)

}//namespace Limitless

#endif //_ImageSetSample_h_