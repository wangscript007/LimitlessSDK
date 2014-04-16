#include "Media/IMediaFilter.h"
#include "Media/MediaPad.h"
#include "Media/MediaSampleFactory.h"
#include "Media/MediaPluginFactory.h"
#include "Media/MediaSourcePad.h"
#include "Media/SystemMediaClock.h"

#include "boost/foreach.hpp"
#include "Base/json.h"
#include <QtCore/QStringList>

IMediaFilter::IMediaFilter(std::string instance, SharedMediaFilter parent):
PluginObject(instance), 
m_parent(parent),
m_state(INIT)
{
	m_baseClasses.push_back("IMediaFilter");
}

//IMediaFilter::~IMediaFilter()
//{
//	MediaPluginFactory::instance().removeFilter(this);
//}

void IMediaFilter::addSourcePad(SharedMediaPad mediaPad)
{
	m_mediaSourcePads.push_back(mediaPad);
}

void IMediaFilter::addSourcePad(const char *jsonFormat)
{
	addSourcePad(std::string(jsonFormat));
}

void IMediaFilter::addSourcePad(const std::string &jsonFormat)
{
	QVariant jsonVariant=QtJson::parse(QString::fromStdString(jsonFormat));

	if(jsonVariant.type() == QVariant::List)
	{
		SharedMediaPad mediaPad(new MediaSourcePad(shared_from_this()));
		QVariantList formats=jsonVariant.toList();

		for(int i=0; i<formats.size(); ++i)
		{
			mediaPad->addMediaFormat(&formats[i]);
		}
		m_mediaSourcePads.push_back(mediaPad);
	}
	else if(jsonVariant.type() == QVariant::Map)
	{
		QVariantMap variantMap=jsonVariant.toMap();
		QStringList keys=variantMap.keys();

		if(keys.size() > 0)
		{
			if(keys[0] == "formats")
			{
				SharedMediaPad mediaPad(new MediaSourcePad(shared_from_this()));
				QVariant &formatsVariant=variantMap["formats"];

				if(formatsVariant.type() == QVariant::List)
				{
					QVariantList formats=formatsVariant.toList();

					for(int i=0; i<formats.size(); ++i)
					{
						mediaPad->addMediaFormat(&formats[i]);
					}
					m_mediaSourcePads.push_back(mediaPad);
				}
			}
			else
			{
				SharedMediaPad mediaPad(new MediaSourcePad(shared_from_this()));

				mediaPad->addMediaFormat(&jsonVariant);
				m_mediaSourcePads.push_back(mediaPad);
			}
		}
	}
//	mediaPad->addMediaFormat(jsonFormat);
//	m_mediaSourcePads.push_back(mediaPad);
}

void IMediaFilter::addSourcePad(SharedMediaFormat format)
{
	SharedMediaPad mediaPad(new MediaSourcePad(shared_from_this()));

	mediaPad->addMediaFormat(format);
	m_mediaSourcePads.push_back(mediaPad);
}

SharedMediaPads IMediaFilter::getSourcePads()
{
	return m_mediaSourcePads;
}

void IMediaFilter::addSinkPad(SharedMediaPad mediaPad)
{
	m_mediaSinkPads.push_back(mediaPad);
}

void IMediaFilter::addSinkPad(const char *jsonFormat)
{
	addSinkPad(std::string(jsonFormat));
}

void IMediaFilter::addSinkPad(const std::string &jsonFormat)
{
//	SharedMediaPad mediaPad(new MediaSinkPad(shared_from_this()));
//
//	mediaPad->addMediaFormat(jsonFormat);
//	m_mediaSinkPads.push_back(mediaPad);
	QVariant jsonVariant=QtJson::parse(QString::fromStdString(jsonFormat));

	if(jsonVariant.type() == QVariant::List)
	{
		SharedMediaPad mediaPad(new MediaSinkPad(shared_from_this()));
		QVariantList formats=jsonVariant.toList();

		for(int i=0; i<formats.size(); ++i)
		{
			mediaPad->addMediaFormat(&formats[i]);
		}
		m_mediaSinkPads.push_back(mediaPad);
	}
	else if(jsonVariant.type() == QVariant::Map)
	{
		QVariantMap variantMap=jsonVariant.toMap();
		QStringList keys=variantMap.keys();

		if(keys.size() > 0)
		{
			if(keys[0] == "formats")
			{
				SharedMediaPad mediaPad(new MediaSinkPad(shared_from_this()));
				QVariant &formatsVariant=variantMap["formats"];

				if(formatsVariant.type() == QVariant::List)
				{
					QVariantList formats=formatsVariant.toList();

					for(int i=0; i<formats.size(); ++i)
					{
						mediaPad->addMediaFormat(&formats[i]);
					}
					m_mediaSinkPads.push_back(mediaPad);
				}
			}
			else
			{
				SharedMediaPad mediaPad(new MediaSinkPad(shared_from_this()));

				mediaPad->addMediaFormat(&jsonVariant);
				m_mediaSinkPads.push_back(mediaPad);
			}
		}
	}
}

void IMediaFilter::addSinkPad(SharedMediaFormat format)
{
	SharedMediaPad mediaPad(new MediaSinkPad(shared_from_this()));

	mediaPad->addMediaFormat(format);
	m_mediaSinkPads.push_back(mediaPad);
}

SharedMediaPads IMediaFilter::getSinkPads()
{
	return m_mediaSinkPads;
}

//bool IMediaFilter::accept(SharedMediaPad mediaPad, SharedMediaFormat mediaFormat)
//{
//	if(mediaPad->type() == MediaPad::SOURCE)
//	{
//		SharedMediaPads sourcePads=getSourcePads();
//
//		BOOST_FOREACH(SharedMediaPad &sourcePad, sourcePads)
//		{
//			if(sourcePad->accept(mediaFormat))
//				return true;
//		}
//	}
//	else
//	{
//		SharedMediaPads sinkPads=getSinkPads();
//
//		BOOST_FOREACH(SharedMediaPad &sinkPad, sinkPads)
//		{
//			if(sinkPad->accept(mediaFormat))
//				return true;
//		}
//	}
//	return true;
//}

//bool IMediaFilter::accept(SharedMediaFilter sink, SharedMediaFormat format)
//{
//	SharedMediaPads sourcePads=getSourcePads();
//
//	if(format != SharedMediaFormat())
//	{
//		BOOST_FOREACH(SharedMediaPad mediaPad, sourcePads)
//		{
//			SharedMediaSourcePad sourcePad=boost::dynamic_pointer_cast<MediaSourcePad>(mediaPad);
//
//			if(sourcePad->accept(format))
//			{
//				SharedMediaPads sinkPads=sink->getSinkPads();
//
//				BOOST_FOREACH(SharedMediaPad sinkPad, sinkPads)
//				{
//					if(sinkPad->accept(format))
//						return true;
//				}
//			}
//		}
//	}
//	else
//	{
//		BOOST_FOREACH(SharedMediaPad mediaPad, sourcePads)
//		{
//			SharedMediaSourcePad sourcePad=boost::dynamic_pointer_cast<MediaSourcePad>(mediaPad);
//			SharedMediaFormats sourceMediaFormats=sourcePad->queryMediaFormats();
//
//			BOOST_FOREACH(SharedMediaFormat sourceMediaFormat, sourceMediaFormats)
//			{
//				SharedMediaPads sinkPads=sink->getSinkPads();
//
//				BOOST_FOREACH(SharedMediaPad sinkPad, sinkPads)
//				{
//					if(sinkPad->accept(sourceMediaFormat))
//						return true;
//				}
//			}
//		}
//	}
//	return false;
//}

bool IMediaFilter::link(boost::shared_ptr<IMediaFilter> sinkFilter)
{
	SharedMediaPads sourcePads=getSourcePads();
	SharedMediaPads sinkPads=sinkFilter->getSinkPads();

	BOOST_FOREACH(SharedMediaPad &sourcePad, sourcePads)
	{
		if(!sourcePad->linked())
		{
			SharedMediaFormats sourceMediaFormats=sourcePad->queryMediaFormats();

			BOOST_FOREACH(SharedMediaFormat &sourceMediaFormat, sourceMediaFormats)
			{
				DEBUG_MEDIA_FORMAT(sourceMediaFormat);
				BOOST_FOREACH(SharedMediaPad sinkPad, sinkPads)
				{
					if(!sinkPad->linked())
					{
						if(sinkPad->accept(sourceMediaFormat))
						{
							return sourcePad->link(sinkPad, sourceMediaFormat);
						}
					}
				}
			}
		}
	}
	return false;
}

//SharedMediaFormat IMediaFilter::connect(boost::shared_ptr<IMediaFilter> sink, SharedMediaFormat format)
//{
//	SharedMediaFormat acceptedFormat=accept(sink);
//	SharedMediaPads sourcePads=getSourcePads();
//
//	if(acceptedFormat != SharedMediaFormat())
//	{
//		BOOST_FOREACH(SharedMediaPad sourcePad, sourcePads)
//		{
//			if(!sourcePad->accept(acceptedFormat))
//				continue;
//
//			SharedMediaPads sinkPads=sink->getSinkPads();
//			
//			BOOST_FOREACH(SharedMediaPad sinkPad, sinkPads)
//			{
//				if(!sinkPad->accept(acceptedFormat))
//					continue;
//
//				acceptedFormat=sourcePad->connect(sinkPad, acceptedFormat);
//				return acceptedFormat;
//			}
//		}
//	}
//	return SharedMediaFormat();
//}

//bool IMediaFilter::connectLinkedPads()
//{
//	bool connected=false;
//
//	BOOST_FOREACH(SharedMediaPad &sourceMediaPad, m_mediaSourcePads)
//	{
//		if(!sourceMediaPad->connect())
//			return false;
//		else
//			connected=true;
//	}
//	return connected;
//}

bool IMediaFilter::reLinkPads()
{
	bool linked=true;

	BOOST_FOREACH(SharedMediaPad &sourceMediaPad, m_mediaSourcePads)
	{
		if(sourceMediaPad->linked())
			linked=true;
		else if(sourceMediaPad->linkChanged())
		{
			linked=sourceMediaPad->relink();
			if(!linked)
				return false;
		}
	}
	return linked;
}

bool IMediaFilter::onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SOURCE)
	{
		SharedMediaPads::iterator iter=std::find(m_mediaSourcePads.begin(), m_mediaSourcePads.end(), pad);

		if(iter != m_mediaSourcePads.end())
		{
			return pad->onAccept(format);
		}
	}
	else
	{
		SharedMediaPads::iterator iter=std::find(m_mediaSinkPads.begin(), m_mediaSinkPads.end(), pad);

		if(iter != m_mediaSinkPads.end())
		{
			return pad->onAccept(format);
		}
	}
	return false;
}

//bool IMediaFilter::linkSourcePad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format)
//{
//	return sourcePad->onLink(sinkPad, format);
//}
//
//bool IMediaFilter::linkSinkPad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format)
//{
//	return sourcePad->onLink(sinkPad, format);
//}
//
//SharedMediaFormat IMediaFilter::connectSourcePad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format)
//{
//	return sourcePad->onConnect(sinkPad, format);
//}
//
//SharedMediaFormat IMediaFilter::connectSinkPad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format)
//{
//	return sourcePad->onConnect(sinkPad, format);
//}

void IMediaFilter::disconnect(boost::shared_ptr<IMediaFilter> sink)
{
	SharedMediaPads sourcePads=getSourcePads();

	BOOST_FOREACH(SharedMediaPad sourcePad, sourcePads)
	{
		if(!sourcePad->linked())
		{
			SharedMediaPad linkedPad=sourcePad->linkedPad();
			
			if(linkedPad != SharedMediaPad())
			{
				if(linkedPad->parent() == sink)
					sourcePad->disconnect();
			}
		}
	}
}

bool IMediaFilter::inStream(boost::shared_ptr<IMediaFilter> filter)
{
	if(upStream(filter))
		return true;
	if(downStream(filter))
		return true;
	return false;
}

bool IMediaFilter::upStream(boost::shared_ptr<IMediaFilter> filter)
{
	BOOST_FOREACH(SharedMediaPad mediaPad, m_mediaSinkPads)
	{
		if(mediaPad->linked())
		{
			if(mediaPad->parent() == filter)
				return true;
			if(mediaPad->parent()->upStream(filter))
				return true;
		}
	}
	return false;
}

bool IMediaFilter::downStream(SharedMediaFilter filter)
{
	BOOST_FOREACH(SharedMediaPad mediaPad, m_mediaSourcePads)
	{
		if(mediaPad->linked())
		{
			if(mediaPad->parent() == filter)
				return true;
			if(mediaPad->parent()->downStream(filter))
				return true;
		}
	}
	return false;
}

SharedMediaFilters IMediaFilter::upStreamFilters()
{
	SharedMediaFilters upStreamFilters;

	BOOST_FOREACH(SharedMediaPad mediaPad, m_mediaSinkPads)
	{
		if(mediaPad->linked())
			upStreamFilters.push_back(mediaPad->parent());
	}
	return upStreamFilters;
}

SharedMediaFilters IMediaFilter::downStreamFilters()
{
	SharedMediaFilters downStreamFilters;

	BOOST_FOREACH(SharedMediaPad mediaPad, m_mediaSourcePads)
	{
		if(mediaPad->linked())
			downStreamFilters.push_back(mediaPad->parent());
	}
	return downStreamFilters;
}

SharedMediaSample IMediaFilter::newSample(unsigned int type)
{
	return  newSample(type, (unsigned __int64)this);
}

SharedMediaSample IMediaFilter::newSample(unsigned int type, unsigned __int64 allocBin)
{
	if(m_parent != SharedMediaFilter())
		return m_parent->newSample(type, allocBin);

	SharedMediaSample sample=MediaSampleFactory::instance().createType(type);

	return sample;
}

void IMediaFilter::deleteSample(SharedMediaSample sample)
{
	if(m_parent != SharedMediaFilter())
		return m_parent->deleteSample(sample);
	sample.reset();
}

MediaTime IMediaFilter::getTime()
{
	if(m_mediaClock != SharedMediaClock())
		return m_mediaClock->time();
	else
	{
		if(m_parent != SharedMediaFilter())
			return m_parent->getTime();
		else
		{
			m_mediaClock.reset(new SystemMediaClock());
			return m_mediaClock->time();
		}
	}
}

IMediaFilter::FilterState IMediaFilter::getState() const
{
	return m_state;
}

IMediaFilter::StateChange IMediaFilter::setState(IMediaFilter::FilterState state)
{
	StateChange stateChange;

	if(state=READY)
		stateChange=ready();
	else if(state=PAUSED)
		stateChange=pause();
	else if(state=PLAYING)
		stateChange=play();
	return stateChange;
}

IMediaFilter::StateChange IMediaFilter::ready()
{
	StateChange stateChange;

	if(m_state == PLAYING)
	{
		stateChange=onPaused();
		if(stateChange != SUCCESS)
			return stateChange;
		m_state=PAUSED;
	}
	if((m_state == INIT) || (m_state == PAUSED))
	{
//		if(m_state == INIT)
//		{
//			if(!connectLinkedPads())
//				return FAILED;
//		}
		stateChange=onReady();
		if(stateChange != SUCCESS)
			return stateChange;
		if(!reLinkPads())
			return FAILED;
		m_state=READY;
		return stateChange;
//		m_state=READY;
//		return stateChange;
	}
	return FAILED;
}

IMediaFilter::StateChange IMediaFilter::pause()
{
	StateChange stateChange;

	if(m_state == INIT)
	{
		stateChange=onReady();
		if(stateChange != SUCCESS)
			return stateChange;
		m_state=READY;
	}
	if((m_state == READY) || (m_state == PLAYING))
	{
		stateChange=onPaused();
		if(stateChange != SUCCESS)
			return stateChange;
		m_state=PAUSED;
		return stateChange;
	}
	return FAILED;
}

IMediaFilter::StateChange IMediaFilter::play()
{
	StateChange stateChange;

	if(m_state == INIT)
	{
		stateChange=onReady();
		if(stateChange != SUCCESS)
			return stateChange;
		m_state=READY;
	}
	if(m_state == READY)
	{
		stateChange=onPaused();
		if(stateChange != SUCCESS)
			return stateChange;
		m_state=PAUSED;
	}
	if(m_state == PAUSED)
	{
		stateChange=onPlaying();
		if(stateChange != SUCCESS)
			return stateChange;
		m_state=PLAYING;
		return stateChange;
	}
	return FAILED;
}

void IMediaFilter::onLinkFormatChanged(SharedMediaPad mediaPad, SharedMediaFormat format)
{
}

void IMediaFilter::pushSample(SharedMediaSample sample)
{
	SharedMediaPads sourcePads=getSourcePads();
	bool pushed=false;

	BOOST_FOREACH(SharedMediaPad &sourcePad, sourcePads)
	{
		if(sourcePad->linked())
		{
//			sourcePad->processSample(sample);
			sourcePad->linkedPad()->processSample(sample);
			pushed=true;
		}
	}
	if(!pushed) //sample was not pushed please delete
		deleteSample(sample);
}