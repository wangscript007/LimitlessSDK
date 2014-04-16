#ifndef _IImageSetSample_h_
#define _IImageSetSample_h_

#include "Media/MediaSample.h"
#include "Media/IImageSample.h"

class IImageSetSample:public MediaSample
{
public:
	IImageSetSample(){};
	virtual ~IImageSetSample(){};

	virtual IImageSample *operator[](int index)=0;
	virtual unsigned int setSize()=0;
};
#endif //_IImageSetSample_h_