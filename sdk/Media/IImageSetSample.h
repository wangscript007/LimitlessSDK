#ifndef _IImageSetSample_h_
#define _IImageSetSample_h_

#include "Media/MediaSample.h"
#include "Media/IImageSample.h"
#include "Media/media_define.h"

namespace Limitless
{

class MEDIA_EXPORT IImageSetSample:public MediaSample
{
public:
	IImageSetSample(){};
	virtual ~IImageSetSample(){};

	virtual IImageSample *operator[](int index)=0;
	virtual size_t setSize()=0;
};

}//namespace Limitless

#endif //_IImageSetSample_h_