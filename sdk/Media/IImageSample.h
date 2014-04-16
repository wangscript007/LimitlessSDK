#ifndef _IImageSample_h_
#define _IImageSample_h_

#include "Media/media_define.h"
#include "Media/MediaSample.h"
#include "Media/MediaSampleFactory.h"
#include <boost/shared_ptr.hpp>

class MEDIA_EXPORT IImageSample:public MediaSample
{
public:
	IImageSample(){};
	virtual ~IImageSample(){};

	virtual std::string imageFormat() const=0;
	virtual int width() const=0;
	virtual int pitch() const=0;
	virtual int height() const=0;
	virtual int channels() const=0;
	virtual int channelBits() const=0;
};
typedef boost::shared_ptr<IImageSample> SharedIImageSample;
#endif //_IImageSample_h_