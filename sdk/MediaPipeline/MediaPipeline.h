#ifndef _MediaPipeline_h_
#define _MediaPipeline_h_

#include "mediapipeline_define.h"

#include "Media/MediaSample.h"
#include "Media/IMediaFilter.h"
#include "Media/IMediaSource.h"
#include "Media/MediaFilterContainer.h"
//#include "IPipelineCallback.h"
#include <string>
#include <vector>
#include <boost/thread.hpp>

#include <boost/lockfree/stack.hpp>
//class MediaPipelineNode
//{
//public:
//	MediaPipelineNode(SharedMediaFilter mediaFilter, size_t sampleType):
//	  m_mediaFilter(mediaFilter), m_sampleType(sampleType){};
//
//	SharedMediaFilter getMediaFilter(){return m_mediaFilter;}
//	size_t getSampleType(){return m_sampleType;}
//
//private:
//	SharedMediaFilter m_mediaFilter;
//	size_t m_sampleType;
////	SharedMediaSamples m_mediaSamples;
//};

//typedef boost::shared_ptr<MediaPipelineNode> SharedMediaPipelineNode;
//typedef std::vector<SharedMediaPipelineNode> SharedMediaPipelineNodes;
#pragma warning(push)
#pragma warning(disable:4251)

namespace Limitless
{

class MEDIAPIPELINE_EXPORT MediaPipeline:public MediaFilterContainer
{
public:
	MediaPipeline(std::string instance, SharedMediaFilter parent);
	virtual ~MediaPipeline();

	virtual bool initialize(const Attributes &attributes){return false;}
	virtual bool shutdown(){return false;}
//	SharedMediaFilter addSource(const std::string &name, const std::string &instance, const Attributes &attributes=Attributes());
//	SharedMediaFilter addFilter(const std::string &name, const std::string &instance, const Attributes &attributes=Attributes());
//	SharedMediaFilter addConsumer(const std::string &name, const std::string &instance, const Attributes &attributes=Attributes());

//	size_t connect(SharedMediaFilter source, SharedMediaFilter sink);

//	bool start();
//	void stop();
//	bool isRunning();

//	virtual void onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();
	
	void processThread();
//	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);
//	virtual bool sampleCallback(MediaSample *pushSample, MediaSample **newSample);

	virtual SharedMediaSample newSample(unsigned int type, unsigned __int64 bin);
	virtual void deleteSample(SharedMediaSample sample);

private:
//	static MediaPipeline m_instance;
	boost::thread m_thread;
	bool m_running;

	typedef std::vector<SharedMediaSample> MediaSampleStack;
//	typedef std::map<unsigned int, MediaSampleStack> FreeSampleStacks;
	typedef std::map<unsigned __int64, MediaSampleStack> FreeSampleStacks;

	FreeSampleStacks m_freeSamples;
	boost::mutex m_freeStackMutex;
//	std::vector<SharedMediaSample> m_samples;

//	SharedMediaPipelineNode m_source;
//	SharedMediaPipelineNodes m_filters;
//	SharedMediaPipelineNode m_consumer;

//	std::list<SharedMediaSample> m_sourceSamples;
};
typedef boost::shared_ptr<MediaPipeline> SharedMediaPipeline;

/*
#include "PipelineObject.h"
#include "IPipelineSource.h"
#include "IPipelineFilter.h"
#include "IPipelineCallback.h"
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include "MediaPipeline_define.h"

template <typename T> struct TypeName
{	
	static std::string get()
	{
		const char *fullName=typeid(T).name();
		const char *name=strstr(fullName, "class");
		return (name)? name+6 : fullName;
	}
};

class MEDIAPIPELINE_EXPORT MediaPipeline:public IPipelineCallback
{
	typedef PipelineObject *(*FactoryFunc)(std::string);
	typedef boost::unordered_map<std::string, FactoryFunc> FactoryFunctions;

private:
	MediaPipeline();
public:
	~MediaPipeline();

	static MediaPipeline &instance();

//	std::vector<std::string> imageSources();
	std::vector<std::string> getType(std::string type);
//	StreamObject *createType(std::string type, std::string instance);
	template<typename CLASS> CLASS *createType(std::string type, std::string instance)
	{
		FactoryFunctions::iterator iter=m_objects.find(type);

		if(iter != m_objects.end())
		{
			CLASS *object=dynamic_cast<CLASS *>(iter->second(instance));

			if(object != NULL)
				return object;
		}
		return NULL;
	}

	SharedPipelineSource addSource(const std::string &name, const std::string &instance, const Attributes &attributes=Attributes());
	bool addFilter(const std::string &name, const std::string &instance, const Attributes &attributes=Attributes());
	bool addConsumer(const std::string &name, const std::string &instance, const Attributes &attributes=Attributes());

//Setup devices
	std::string registerType(std::string typeName, FactoryFunc factoryFunc)
	{
		m_objects[typeName]=factoryFunc;
		return typeName;
	}

	bool start();
	void stop();
	bool isRunning();

	void process();
	virtual bool sampleCallback(PipelineSample *pushSample, PipelineSample **newSample);
private:
//	static MediaPipeline m_instance;
	boost::thread m_thread;
	bool m_running;

	FactoryFunctions m_objects;
	SharedPipelineSource m_source;

	typedef std::vector<IPipelineFilter *> PipelineFilters;
	PipelineFilters m_filters;

	IPipelineFilter *m_consumer;

	std::list<PipelineSample *> m_sourceSamples;
};


template<typename CLASS, typename INTERFACE> std::string AutoRegister<CLASS, INTERFACE>::s_typeName=\
MediaPipeline::instance().registerType(TypeName<CLASS>::get(), &AutoRegister<CLASS, INTERFACE>::create);

//template <typename BASE, typename DERIVED> PipelineObject *createInstance()
//{    return new DERIVED;}
//
//#define REGISTER_PIPLELINEOBJECT(NAME, BASE, DERIVED) static PipelineObject *_factory_##DERIVED=MediaPipeline::instance().registerType(#NAME, createInstance<#BASE, #DERIVED>)
//template <int TYPE, typename IMPL> const uint16_t PipelineObjectInst<TYPE, IMPL>::typeName = MediaPipeline::instance().registerType(
//     PipelineObjectInst<TYPE, IMPL>::typeName, &PipelineObjectInst<TYPE, IMPL>::create);
*/

}//namespace Limitless

#pragma warning(pop)

#endif //_MediaPipeline_h_

