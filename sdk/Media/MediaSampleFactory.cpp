#include "Media/MediaSampleFactory.h"
#include "boost/foreach.hpp"

unsigned int MediaSampleFactory::s_sampleTypeIndex=0;

MediaSampleFactory::MediaSampleFactory()
{}
MediaSampleFactory::~MediaSampleFactory()
{}

MediaSampleFactory &MediaSampleFactory::instance()
{
	static MediaSampleFactory s_instance;

	return s_instance;
}

SharedMediaSample MediaSampleFactory::createType(std::string typeName)
{
	BOOST_FOREACH(MediaSampleNode &mediaSampleNode, m_mediaSampleNodes)
	{
		if(mediaSampleNode.typeName() == typeName)
		{
//				CLASS *object=dynamic_cast<CLASS *>(mediaSampleNode.factoryFunction());
//				if(object != NULL)
//					return object;

			SharedMediaSample object(mediaSampleNode.factoryFunction());
			
			if(object != SharedMediaSample())
				return object;
			break;
		}
	}
	return SharedMediaSample();
//		return NULL;
}

SharedMediaSample MediaSampleFactory::createType(unsigned int type)
{
	if((type < 0) || (type >= m_mediaSampleNodes.size()))
//		return NULL;
		return SharedMediaSample();

//		CLASS *object=dynamic_cast<CLASS *>(m_mediaSampleNodes[type].factoryFunction());
	SharedMediaSample object(m_mediaSampleNodes[type].factoryFunction());

	return object;
}

size_t MediaSampleFactory::getTypeId(std::string typeName)
{
	BOOST_FOREACH(MediaSampleNode &mediaSampleNode, m_mediaSampleNodes)
	{
		if(mediaSampleNode.typeName() == typeName)
			return mediaSampleNode.type();
	}
	return -1;
}