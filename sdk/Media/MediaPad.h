#ifndef _Limitless_MediaPad_h_
#define _Limitless_MediaPad_h_

#include "Media/media_define.h"

#include "Media/MediaFormat.h"
#include "Media/MediaSample.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)

class QVariant;

namespace Limitless
{

class MediaPad;

typedef boost::shared_ptr<MediaPad> SharedMediaPad;
typedef std::vector<SharedMediaPad> SharedMediaPads;

class IMediaFilter;
typedef boost::shared_ptr<IMediaFilter> SharedMediaFilter;

class MEDIA_EXPORT MediaPad:public boost::enable_shared_from_this<MediaPad>
{
public:
	MediaPad(SharedMediaFilter parent):m_parent(parent), m_state(INIT){};
	virtual ~MediaPad(){};

	virtual SharedMediaFilter parent() const{return m_parent;}

	enum State
	{
		INIT,
		LINKED,
		LINK_CHANGED,
		PROCESSING
	};

	enum Type
	{
		SOURCE,
		SINK
	};

	virtual Type type()=0;
	MediaPad::State state(){return m_state;}
	virtual void addMediaFormat(SharedMediaFormat format);
	virtual void addMediaFormat(std::string jsonFormat);
	virtual void addMediaFormat(QVariant *jsonVariant);
	virtual void removeAllMediaFormats();
	virtual SharedMediaFormats queryMediaFormats();
	SharedMediaFormat format();
	bool setFormat(MediaFormat format);
	
	SharedMediaPad linkedPad(){return m_linkedPad;}
	
	
	bool accept(SharedMediaFormat format);
//	virtual SharedMediaFormat accept(SharedMediaPad mediaPad, SharedMediaFormat format=SharedMediaFormat())=0;
	bool link(SharedMediaPad mediaPad, SharedMediaFormat format=SharedMediaFormat());
	bool relink();
	bool linked(){return (m_state == LINKED);}
	bool linkChanged(){return (m_state == LINK_CHANGED);}
//	SharedMediaFormat connect();
//	SharedMediaFormat connect(SharedMediaPad mediaPad, SharedMediaFormat format=SharedMediaFormat());
	virtual void disconnect();
//	bool connected(){return (m_state == CONNECTED);}

	virtual bool processSample(SharedMediaSample mediaSample);
	
protected:
	friend class IMediaFilter;
	virtual bool onAccept(SharedMediaFormat format);
//	virtual bool onLink(SharedMediaPad mediaPad, SharedMediaFormat format)=0;

	State m_state;
	SharedMediaPad m_linkedPad;
private:
	SharedMediaFilter m_parent;
	SharedMediaFormats m_mediaFormats;
	SharedMediaFormat m_format;
};

}//namespace Limitless

#pragma warning(pop)

#endif //_Limitless_MediaPad_h_