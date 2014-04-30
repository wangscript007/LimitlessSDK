#include "MediaPipeline/MediaPipeline.h"
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include "Base/PluginFactory.h"
#include "Media/MediaSampleFactory.h"

using namespace Limitless;

MediaPipeline::MediaPipeline(std::string instance, SharedMediaFilter parent):
MediaFilterContainer(instance, parent),
m_running(false)
{
//	for(int i=0; i<10; ++i)
//		m_sourceSamples.push_back(new MediaSample());
}

MediaPipeline::~MediaPipeline()
{}

//SharedMediaFilter MediaPipeline::addSource(const std::string &name, const std::string &instance, const Attributes &attributes)
//{
//	SharedMediaFilter source(PluginFactory::instance().createType<IMediaFilter>(name, instance));
//
//	if(source != SharedMediaFilter())
//	{
//		source->initialize(attributes);
//
//		if(m_filters.size() > 0)
//		{
//			size_t sampleType;
//			
//			sampleType=connect(m_source->getMediaFilter(), m_filters[0]->getMediaFilter());
//			if(sampleType >= 0)
//			{
//				m_source.reset(new MediaPipelineNode(source, sampleType));
//				for(int i=0; i<10; ++i)
//					m_sourceSamples.push_back(SharedMediaSample(MediaSampleFactory::instance().createType<MediaSample>(sampleType)));
//			}
//		}
//		return source;
//	}
//	return SharedMediaFilter();
//}
//
//SharedMediaFilter MediaPipeline::addFilter(const std::string &name, const std::string &instance, const Attributes &attributes)
//{
//	SharedMediaFilter filter(PluginFactory::instance().createType<IMediaFilter>(name, instance));
//
//	if(filter != SharedMediaFilter())
//	{
//		filter->initialize(attributes);
//		m_filters.push_back(SharedMediaPipelineNode(new MediaPipelineNode(filter, 0)));
//		return filter;
//	}
//	return SharedMediaFilter();
//}
//
//SharedMediaFilter MediaPipeline::addConsumer(const std::string &name, const std::string &instance, const Attributes &attributes)
//{
//	SharedMediaFilter consumer(PluginFactory::instance().createType<IMediaFilter>(name, instance));
//
//	if(consumer != SharedMediaFilter())
//	{
//		m_consumer=consumer;
//		m_consumer->getMediaFilter()->initialize(attributes);
//		return consumer;
//	}
//	return SharedMediaFilter();
//}

//size_t MediaPipeline::connect(SharedMediaFilter source, SharedMediaFilter sink)
//{
//	size_t sampleType=-1;
//	std::vector<MediaSample *> sourceSamples;
//
//	source->accept(sink);
////	source->getSources(sourceSamples);
////	boost::foreach(MediaSample *sourceSample, sourceSamples)
////	{
////		if(sink->accept(sourceSample))
////		{
////			sampleType=sourceSample->getType();
////			break;
////		}
////	}
//	return sampleType;
//}

//bool MediaPipeline::start()
//{
////	if((m_source != NULL) && (m_consumer != NULL))
////	{
////		SharedMediaSample sample=m_sourceSamples.front();
////		
////		m_sourceSamples.pop_front();
//////		m_source->startPush(this, sample);
////		m_running=true;
//		m_thread=boost::thread(boost::bind(&MediaPipeline::process, this));
////		return true;
////	}
//	return false;
//}
//
//void MediaPipeline::stop()
//{
////	m_source->stopPush();
//	m_running=false;
//	m_thread.interrupt();
//}
//
//bool MediaPipeline::isRunning()
//{
//	return m_running;
//}
IMediaFilter::StateChange MediaPipeline::onPaused()
{
	StateChange statChange=MediaFilterContainer::onPaused();

	if(statChange == SUCCESS)
	{
//		if(getState() == IMediaFilter::PLAYING)
//		{
			m_running=false;
			m_thread.interrupt();
			return SUCCESS;
//		}
	}
	return FAILED;
}

IMediaFilter::StateChange MediaPipeline::onPlaying()
{
	StateChange statChange=MediaFilterContainer::onPlaying();

	m_running=true;
	m_thread=boost::thread(boost::bind(&MediaPipeline::processThread, this));

	return statChange;
}


void MediaPipeline::processThread()
{
//	IMediaSource *source=dynamic_cast<IMediaSource *>(m_source->getMediaFilter().get());
//	SharedMediaSourcePad sourcePad=mediaSourcePad();
//	
////	m_sourceSamples.pop_front();
//	if(sourcePad != SharedMediaSourcePad())
//	{
//		while(m_running)
//		{
//			MediaFilterContainer::processSample(SharedMediaPad(), SharedMediaSample());
//		}
//	}
#ifdef WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif

	SharedMediaFilter sourceMediaFilter=sourceFilter();

	if(sourceMediaFilter != SharedMediaFilter())
	{
		while(m_running)
		{
			sourceMediaFilter->processSample(SharedMediaPad(), SharedMediaSample());
		}
	}
}

//bool MediaPipeline::processSample(SharedMediaPad sinkPad, SharedMediaSample sample)
//{
//	//process eac filter then return
//	return false;
//}

//bool MediaPipeline::sampleCallback(MediaSample *pushSample, MediaSample **newSample)
//{
//	MediaSample *sample=m_sourceSamples.front();
//		
//	m_sourceSamples.pop_front();
//	
//	return true;
//}

SharedMediaSample MediaPipeline::newSample(unsigned int type, unsigned __int64 bin)
{
	SharedMediaSample sample;

	{
		boost::unique_lock<boost::mutex> lock(m_freeStackMutex);

//		MediaSampleStack &freeStack=m_freeSamples[type];
		MediaSampleStack &freeStack=m_freeSamples[bin];

		if(!freeStack.empty())
		{	
			sample=freeStack.back();
			freeStack.pop_back();
		}
	}

	if(sample == SharedMediaSample())
	{
		sample=MediaSampleFactory::instance().createType(type);
		sample->setAllocBin(bin);
	}

//	OutputDebugStringA((boost::format("0x%08x(0x%08x) - newSample %u\n")%this%GetCurrentThreadId()%sample->uniqueId()).str().c_str());

	sample->setSequenceNumber(0);
//	if(sample->flags())
//		sample->setFlags(0x00);
//	sample->addFlags(0x02);
	return sample;
}

void MediaPipeline::deleteSample(SharedMediaSample sample)
{
	boost::unique_lock<boost::mutex> lock(m_freeStackMutex);
	
//	MediaSampleStack &freeStack=m_freeSamples[sample->type()];
	MediaSampleStack &freeStack=m_freeSamples[sample->allocBin()];
//	sample->setFlags(0x00);
	freeStack.push_back(sample);
//	OutputDebugStringA((boost::format("0x%08x(0x%08x) - deleteSample %u\n")%this%GetCurrentThreadId()%sample->uniqueId()).str().c_str());
}
