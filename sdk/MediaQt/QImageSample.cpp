#include "MediaQt/QImageSample.h"

QImageSample::operator QImage() const
{
	return m_image;
}

std::string QImageSample::imageFormat() const
{
	return "";
}

int QImageSample::width() const
{
	return m_image.width();
}

int QImageSample::pitch() const
{
	return m_image.bytesPerLine();
}

int QImageSample::height() const
{
	return m_image.height();
}

int QImageSample::channels() const
{
	switch(m_image.format())
	{
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32:
		return 4;
		break;
	case QImage::Format_RGB888:
	case QImage::Format_RGB666:
	case QImage::Format_RGB555:
	case QImage::Format_RGB444:
		return 3;
		break;
	}
	return 0;
}

int QImageSample::channelBits() const
{
	switch(m_image.format())
	{
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32:
	case QImage::Format_RGB888:
		return 8;
		break;
	case QImage::Format_RGB666:
		return 6;
		break;
	case QImage::Format_RGB555:
		return 5;
		break;
	case QImage::Format_RGB444:
		return 4;
		break;
	}
}

unsigned char *QImageSample::buffer()
{
	return NULL;
}

size_t QImageSample::size() const
{
	return 0;
}