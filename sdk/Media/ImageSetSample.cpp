#include "ImageSetSample.h"

using namespace Limitless;

ImageSetSample::ImageSetSample():
m_buffer(NULL),
m_size(0)
{
}

ImageSetSample::~ImageSetSample()
{
	freeBuffer();
}


IImageSample *ImageSetSample::operator[](int index)
{
	return m_imageSamples[index];
}

size_t ImageSetSample::setSize()
{
	return m_imageSamples.size();
}

void ImageSetSample::resize(unsigned int count, int width, int height, int channels, int channelBits)
{
	resize(count, width, width, height, channels, channelBits);
}

void ImageSetSample::resize(unsigned int count, int width, int pitch, int height, int channels, int channelBits)
{
	//only resize if needed
	if((m_imageSamples.size() != count) || (m_pitch != pitch) || (m_height != height) || (m_channels != channels) || (m_channelBits != channelBits))
	{
		freeBuffer();

		int bytes=(int)ceil((float)(channels*channelBits)/8);

		unsigned char *buffer=(unsigned char *)malloc(count*(pitch*bytes)*height);

		m_imageSamples.clear();
		unsigned char *pos=buffer;
		for(unsigned int i=0; i<count; ++i)
		{
			m_imageSamples.push_back(new ImageSample(pos, width, pitch, height, channels, channelBits));
			pos+=(pitch*bytes)*height;
		}

		m_buffer=buffer;
		m_size=count*(pitch*bytes)*height;
		m_pitch=pitch;
		m_width=width;
		m_height=height;
		m_channels=channels;
		m_channelBits=channelBits;
	}
}

void ImageSetSample::freeBuffer()
{
	if(m_buffer != NULL)
		free(m_buffer);
	m_buffer=NULL;
}