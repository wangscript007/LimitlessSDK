#ifndef _IIMediaSDK_MediaSinkPad_h_
#define _IIMediaSDK_MediaSinkPad_h_

#include "Media/media_define.h"

#include "Media/MediaPad.h"
#include "boost/shared_ptr.hpp"

class MEDIA_EXPORT MediaSinkPad:public MediaPad
{
public:
	MediaSinkPad(SharedMediaFilter parent):MediaPad(parent){};
	virtual ~MediaSinkPad(){};

	Type type(){return SINK;}
//	SharedMediaFormat accept(SharedMediaPad sourcePad, SharedMediaFormat format=SharedMediaFormat());
//	virtual SharedMediaFormat onAccept(SharedMediaPad sourcePad, SharedMediaFormat format=SharedMediaFormat());
//
////	bool link(SharedMediaPad sourcePad);
////	SharedMediaFormat connect(SharedMediaPad sourcePad, SharedMediaFormat format=SharedMediaFormat());
//	void disconnect();
//	bool connected();
//	virtual bool processSample(SharedMediaSample mediaSample);

protected:
//	virtual bool onAccept(SharedMediaFormat format);
//	virtual bool onLink(SharedMediaPad mediaPad, SharedMediaFormat format);
//	virtual SharedMediaFormat onConnect(SharedMediaPad mediaPad, SharedMediaFormat format);

private:
//	SharedMediaPad m_connectedSourcePad;
};
typedef boost::shared_ptr<MediaSinkPad> SharedMediaSinkPad;
typedef std::vector<SharedMediaSinkPad> SharedMediaSinkPads;

#endif //_IIMediaSDK_MediaSinkPad_h_