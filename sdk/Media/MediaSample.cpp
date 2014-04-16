#include <Media/MediaSample.h>

unsigned int MediaSample::m_uniqueIdCount=0;

void MediaSample::copyHeader(boost::shared_ptr<MediaSample> sample)
{
	m_timestamp=sample->timestamp();
	m_sequenceNumber=sample->sequenceNumber();
	m_mediaIndex=sample->m_mediaIndex;
	m_flags=sample->m_flags;
}