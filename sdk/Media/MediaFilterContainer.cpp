#include "Media/MediaFilterContainer.h"
#include "boost/foreach.hpp"

using namespace Limitless;

void MediaFilterContainer::addMediaFilter(SharedMediaFilter mediaFilter)
{
	m_mediaFilters.push_back(mediaFilter);

	//every filter is considered a source and sink till they are linked
	m_sourceFilters.push_back(mediaFilter);
	m_sinkFilters.push_back(mediaFilter);
}

void MediaFilterContainer::removeMediaFilter(SharedMediaFilter mediaFilter)
{
	SharedMediaFilters::iterator iter=std::find(m_mediaFilters.begin(), m_mediaFilters.end(), mediaFilter);
	
	if(iter != m_mediaFilters.end())
		m_mediaFilters.erase(iter);
}

SharedMediaFilters MediaFilterContainer::mediaFilters()
{
	SharedMediaFilters mediaFilters;

	BOOST_FOREACH(SharedMediaFilter mediaFilter, m_mediaFilters)
	{
		mediaFilters.push_back(mediaFilter);
	}
	return mediaFilters;
}

void MediaFilterContainer::setSourceFilter(SharedMediaFilter mediaFilter)
{
	SharedMediaFilters::iterator iter=std::find(m_mediaFilters.begin(), m_mediaFilters.end(), mediaFilter);
	
	if(iter == m_mediaFilters.end())
		m_mediaFilters.push_back(mediaFilter);

	m_sourceFilter=mediaFilter;
	SharedMediaPads mediaPads=m_sourceFilter->getSourcePads();

	BOOST_FOREACH(SharedMediaPad mediaPad, mediaPads)
	{
		SharedMediaSourcePad mediaSourcePad=boost::dynamic_pointer_cast<MediaSourcePad>(mediaPad);

		if(mediaSourcePad != SharedMediaSourcePad())
		{
			if(mediaSourcePad->linked())
			{
				m_mediaSourcePad=mediaSourcePad;
				break;
			}
		}
	}
}

void MediaFilterContainer::setSinkFilter(SharedMediaFilter mediaFilter)
{
	SharedMediaFilters::iterator iter=std::find(m_mediaFilters.begin(), m_mediaFilters.end(), mediaFilter);
	
	if(iter == m_mediaFilters.end())
		m_mediaFilters.push_back(mediaFilter);

	m_sinkFilter=mediaFilter;
}

bool MediaFilterContainer::link(SharedMediaFilter source, SharedMediaFilter sink)
{
	size_t sampleType=-1;
	std::vector<MediaSample *> sourceSamples;

	bool linked=source->link(sink);

	if(linked)
	{
		//remove source filter from sinkFilters as now it is only a source
		SharedMediaFilters::iterator iter=std::find(m_sinkFilters.begin(), m_sinkFilters.end(), source);

		if(iter != m_sinkFilters.end())
			m_sinkFilters.erase(iter);

		//remove sink filter from sourceFilters as now it is only a sink
		iter=std::find(m_sourceFilters.begin(), m_sourceFilters.end(), sink);

		if(iter != m_sourceFilters.end())
			m_sourceFilters.erase(iter);
	}

	return linked;

//	if(format != SharedMediaFormat())
//	{
//		//need to check current source and make sure it is not down stream
//		if(m_sourceFilter == SharedMediaFilter())
//			m_sourceFilter=source;
//		else
//		{
//			SharedMediaFilters upStreamFilters=m_sourceFilter->upStreamFilters();
//			
//			while(!upStreamFilters.empty())
//			{
//				m_sourceFilter=upStreamFilters
//			}
//		}
//	}
//	return format;
}

bool MediaFilterContainer::link(boost::shared_ptr<IMediaFilter> sinkFilter)
{
	bool linked=false;

	BOOST_FOREACH(SharedMediaFilter &filter, m_sinkFilters)
	{
		if(filter->link(sinkFilter))
		{
			linked=true;
			break;
		}
	}
	return linked;
}

IMediaFilter::StateChange MediaFilterContainer::onReady()
{
	StateChange stateChange;
//	m_sourceFilter->ready();
	BOOST_FOREACH(SharedMediaFilter mediaFilter, m_mediaFilters)
	{
		stateChange=mediaFilter->ready();
		if(stateChange != SUCCESS)
			return stateChange;
	}
	return SUCCESS;
//	m_sinkFilter->ready();
}

IMediaFilter::StateChange MediaFilterContainer::onPaused()
{
	StateChange stateChange;
//	m_sourceFilter->pause();
	BOOST_FOREACH(SharedMediaFilter mediaFilter, m_mediaFilters)
	{
		stateChange=mediaFilter->pause();
	}
	return SUCCESS;
//	m_sinkFilter->pause();
}

IMediaFilter::StateChange MediaFilterContainer::onPlaying()
{
	StateChange stateChange;
//	m_sourceFilter->play();
	BOOST_FOREACH(SharedMediaFilter mediaFilter, m_mediaFilters)
	{
		stateChange=mediaFilter->play();
	}
	return SUCCESS;
//	m_sinkFilter->play();
}

bool MediaFilterContainer::processSample(SharedMediaPad sinkPad, SharedMediaSample sample)
{
	//Container holds a source filter
	if(sinkPad == SharedMediaPad())
	{
		m_sourceFilter->processSample(sinkPad, sample);
	}
	else //push sample to source filter sink pad
	{
	}
	//process each filter then return
	return false;
}

SharedMediaPads MediaFilterContainer::getSourcePads()
{
	SharedMediaPads pads;

	BOOST_FOREACH(SharedMediaFilter &filter, m_sinkFilters)
	{
		SharedMediaPads filterPads=filter->getSourcePads();

		BOOST_FOREACH(SharedMediaPad &mediaPad, filterPads)
		{
			pads.push_back(mediaPad);
		}
	}
	return pads;
}

SharedMediaPads MediaFilterContainer::getSinkPads()
{
	SharedMediaPads pads;

	BOOST_FOREACH(SharedMediaFilter &filter, m_sourceFilters)
	{
		SharedMediaPads filterPads=filter->getSinkPads();

		BOOST_FOREACH(SharedMediaPad &mediaPad, filterPads)
		{
			pads.push_back(mediaPad);
		}
	}
	return pads;
}