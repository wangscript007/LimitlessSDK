#include "MediaPad.h"
#include "IMediaFilter.h"
#include "boost/foreach.hpp"

using namespace Limitless;

void MediaPad::addMediaFormat(SharedMediaFormat format)
{
	m_mediaFormats.push_back(format);
}

void MediaPad::addMediaFormat(std::string jsonFormat)
{
	SharedMediaFormat format(new MediaFormat());

	format->addMediaAttributes(jsonFormat);
	m_mediaFormats.push_back(format);
}

void MediaPad::addMediaFormat(QVariant *jsonVariant)
{
	SharedMediaFormat format(new MediaFormat());

	format->addMediaAttributes(jsonVariant);
	m_mediaFormats.push_back(format);
}

void MediaPad::removeAllMediaFormats()
{
	m_mediaFormats.clear();
}

SharedMediaFormats MediaPad::queryMediaFormats()
{
	if(m_format != SharedMediaFormat())
	{
		SharedMediaFormats mediaFormats;

		mediaFormats.push_back(m_format);
		return mediaFormats;
	}
	return m_mediaFormats;
}

SharedMediaFormat MediaPad::format()
{
	return m_format;
}

bool MediaPad::setFormat(MediaFormat format)
{
	if(m_format == SharedMediaFormat())
		m_format.reset(new MediaFormat);

	*m_format=format;
//	m_state=LINK_CHANGED;

	//need to relink pad
	if(m_linkedPad != SharedMediaPad())
		return link(m_linkedPad, m_format);
	return true;
}

bool MediaPad::accept(SharedMediaFormat format)
{
	return parent()->onAcceptMediaFormat(shared_from_this(), format);
}

bool MediaPad::link(SharedMediaPad mediaPad, SharedMediaFormat format)
{
	if(!accept(format))
		return false;
	if(!mediaPad->accept(format))
		return false;

	parent()->onLinked(shared_from_this(), mediaPad);
	mediaPad->parent()->onLinked(mediaPad, shared_from_this());
	mediaPad->parent()->onLinkFormatChanged(mediaPad, format);

	m_linkedPad=mediaPad;
	m_format=format;
	m_state=LINKED;

	if(mediaPad->m_format == SharedMediaFormat())
		mediaPad->m_format.reset(new MediaFormat());

	mediaPad->m_linkedPad=shared_from_this();
	*mediaPad->m_format=*format;
	mediaPad->m_state=LINKED;
	
	return true;
}

bool MediaPad::relink()
{
	if(m_linkedPad != SharedMediaPad())
	{
		SharedMediaPad linkedPad=m_linkedPad;

		disconnect();
		return link(linkedPad, m_format);
	}
	return true;
}

void MediaPad::disconnect()
{
	SharedMediaPad pad=m_linkedPad;

	m_format.reset();
	m_linkedPad.reset();
	m_state=INIT;

	if(pad != SharedMediaPad())
	{
		parent()->onDisconnected(shared_from_this(), pad);
		pad->parent()->onDisconnected(pad, shared_from_this());
	}
}

bool MediaPad::onAccept(SharedMediaFormat format)
{
	if(m_format != SharedMediaFormat())
	{
		return m_format->match(format);
	}
	else
	{
		SharedMediaFormats mediaFormats=queryMediaFormats();

		BOOST_FOREACH(SharedMediaFormat &mediaFormat, mediaFormats)
		{
			if(mediaFormat->match(format))
				return true;
		}
	}
	return false;
}

bool MediaPad::processSample(SharedMediaSample mediaSample)
{
	return parent()->processSample(shared_from_this(), mediaSample);
}