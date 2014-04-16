#ifndef _IIMediaSDK_MediaSourcePad_h_
#define _IIMediaSDK_MediaSourcePad_h_

#include "Media/media_define.h"

#include "Media/MediaPad.h"
#include "boost/shared_ptr.hpp"
#include <vector>

class MEDIA_EXPORT MediaSourcePad:public MediaPad
{
public:
	MediaSourcePad(SharedMediaFilter parent):MediaPad(parent){};
	virtual ~MediaSourcePad(){};

	Type type(){return SOURCE;}

protected:
//	virtual bool onAccept(SharedMediaFormat format);

private:
};
typedef boost::shared_ptr<MediaSourcePad> SharedMediaSourcePad;
typedef std::vector<SharedMediaSourcePad> SharedMediaSourcePads;

#endif //_IIMediaSDK_MediaSourcePad_h_