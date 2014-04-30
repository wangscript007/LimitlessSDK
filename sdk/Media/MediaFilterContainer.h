#ifndef _Limitless_MediaFilterContainer_h_
#define _Limitless_MediaFilterContainer_h_

#include "Media/media_define.h"

#include <vector>
#include "Media/IMediaFilter.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Limitless
{

class MEDIA_EXPORT MediaFilterContainer:public IMediaFilter
{
public:
	MediaFilterContainer(std::string instance, SharedMediaFilter parent):IMediaFilter(instance, parent){};
	virtual ~MediaFilterContainer(){};

	void addMediaFilter(SharedMediaFilter mediaFilter);
	void removeMediaFilter(SharedMediaFilter meditFilter);
	SharedMediaFilters mediaFilters();

	void setSourceFilter(SharedMediaFilter mediaFilter);
	SharedMediaFilter sourceFilter(){return m_sourceFilter;}
	void setSinkFilter(SharedMediaFilter mediaFilter);

	SharedMediaSourcePad mediaSourcePad(){return m_mediaSourcePad;}

	bool link(SharedMediaFilter source, SharedMediaFilter sink);
	virtual bool link(boost::shared_ptr<IMediaFilter> sinkFilter);

	virtual StateChange onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();

	virtual SharedMediaPads getSourcePads();
	virtual SharedMediaPads getSinkPads();

	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);

private:
	SharedMediaFilters m_sourceFilters;
	SharedMediaFilters m_sinkFilters;

	SharedMediaFilters m_mediaFilters;
	SharedMediaFilter m_sourceFilter;
	SharedMediaFilter m_sinkFilter;

	SharedMediaSourcePad m_mediaSourcePad;
	SharedMediaSourcePad m_mediaSinkPad;
};

}//namespace Limitless

#pragma warning(pop)

#endif //_Limitless_MediaFilterContainer_h_