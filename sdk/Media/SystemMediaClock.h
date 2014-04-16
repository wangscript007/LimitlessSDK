#ifndef _SystemMediaClock_h_
#define _SystemMediaClock_h_

#include "Media/media_define.h"
#include "Media/MediaClock.h"

class MEDIA_EXPORT SystemMediaClock:public MediaClock
{
public:
	SystemMediaClock();
	virtual ~SystemMediaClock(){}

	virtual MediaTime internalTime();
	virtual MediaTime resolution();

private:
	MediaTime m_startTime;
	MediaTime m_frequency;

	MediaTime m_resolution;
};
#endif //_SystemMediaClock_h_