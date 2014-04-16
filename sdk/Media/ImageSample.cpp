#include "Media/ImageSample.h"

ImageSample::ImageSample():
m_buffer(NULL),
m_externalBuffer(false),
m_size(0),
m_width(0),
m_pitch(0),
m_height(0), 
m_channels(3),
m_channelBits(8)
{
}

ImageSample::ImageSample(int width, int height, int channels, int channelBits):
m_size(0)
{
	resize(width, height, channels, channelBits);
}

ImageSample::ImageSample(int width, int pitch, int height, int channels, int channelBits):
m_size(0)
{
	resize(width, pitch, height, channels, channelBits);
}

ImageSample::ImageSample(unsigned char *buffer, int width, int height, int channels, int channelBits):
m_size(0)
{
	setImage(buffer, width, height, channels, channelBits);
}

ImageSample::ImageSample(unsigned char *buffer, int width, int pitch, int height, int channels, int channelBits):
m_size(0)
{
	setImage(buffer, width, pitch, height, channels, channelBits);
}

ImageSample::~ImageSample()
{
	freeBuffer();
}

void ImageSample::setImage(unsigned char *buffer, int width, int height, int channels, int channelBits)
{
	setImage(buffer, width, width, height, channels, channelBits);
}

void ImageSample::setImage(unsigned char *buffer, int width, int pitch, int height, int channels, int channelBits)
{
	freeBuffer();
	
	int bytes=ceil((float)(channels*channelBits)/8);

	m_buffer=buffer;
	m_externalBuffer=true;
	m_size=(pitch*bytes)*height;
	m_width=width;
	m_pitch=pitch;
	m_height=height;
	m_channels=channels;
	m_channelBits=channelBits;
}

void ImageSample::resize(int width, int height, int channels, int channelBits)
{
	resize(width, width, height, channels, channelBits);
}

void ImageSample::resize(int width, int pitch, int height, int channels, int channelBits)
{
//	freeBuffer();
	int bytes=ceil((float)(channels*channelBits)/8);
	int size=(pitch*bytes)*height;

	if(size > m_size)
	{
		freeBuffer();
		m_buffer=(unsigned char *)malloc(size);
		m_externalBuffer=false;
		m_size=size;
	}

	m_width=width;
	m_pitch=pitch;
	m_height=height;
	m_channels=channels;
	m_channelBits=channelBits;
}

void ImageSample::freeBuffer()
{
	if((m_buffer != NULL) && (!m_externalBuffer))
		free(m_buffer);
	m_buffer=NULL;
}