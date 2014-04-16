#ifndef _JoinFilter_h_
#define _JoinFilter_h_

#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"

#include <boost/thread.hpp>

class JoinFilter:public MediaAutoRegister<JoinFilter, IMediaFilter>
{
public:
	JoinFilter(std::string name, SharedMediaFilter parent);
	~JoinFilter();

	virtual bool initialize(const Attributes &attributes);
	virtual bool shutdown(){return true;}

	virtual SharedPluginView getView();

	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);

protected:
	//IMediaFilter
	virtual StateChange onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();

	bool onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format);
	void onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format);

	void processSourceSample();
private:
	bool m_firstSample;
	unsigned int m_currentSequence;
	SharedMediaFormat m_outputFormat;
	
	SharedMediaSamples m_samples;
	boost::mutex m_samplesMutex;
	boost::condition_variable m_sampleEvent;
	boost::thread m_processThread;
};

#endif //_JoinFilter_h_
