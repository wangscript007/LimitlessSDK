#ifndef _QImageSample_h_
#define _QImageSample_h_

#include "MediaQt/mediaqt_define.h"
#include "Media/IImageSample.h"
#include <QtGui/QImage>
#include <QtCore/QString>

namespace Limitless
{

class MEDIAQT_EXPORT QImageSample:public AutoRegisterMediaSample<QImageSample, IImageSample>
{
public:
	QImageSample(){};
	virtual ~QImageSample(){};

	operator QImage() const;

	bool load(QString fileName){return m_image.load(fileName);}

//IImageSample
	virtual std::string imageFormat() const;
	virtual int width() const;
	virtual int pitch() const;
	virtual int height() const;
	virtual int channels() const;
	virtual int channelBits() const;

//IMediaSample
	virtual unsigned char *buffer();
	virtual size_t size() const;

private:
	QImage m_image;
};
typedef boost::shared_ptr<QImageSample> SharedQImageSample;

}//namespace Limitless

#endif //_QImageSample_h_