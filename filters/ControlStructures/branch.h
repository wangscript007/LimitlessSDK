#ifndef _BranchFilter_h
#define _BranchFilter_h

#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"

#include <queue>
#include <boost/thread.hpp>
#include <boost/function.hpp>

class BranchFilter:public MediaAutoRegister<BranchFilter, IMediaFilter>
{
public:
	BranchFilter(std::string name, SharedMediaFilter parent);
	~BranchFilter();

	virtual bool initialize(const Attributes &attributes);
	virtual bool shutdown(){return true;}

	virtual SharedPluginView getView();

	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);

protected:
	//IMediaFilter
	virtual StateChange onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();

	virtual bool onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format);
	virtual void onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format);

	virtual void onLinked(SharedMediaPad pad, SharedMediaPad filterPad);
private:
	void processSourceSample(SharedMediaPad sourcePad);

	class PadThread
	{
	public:
		PadThread(SharedMediaPad pad, boost::function<void (SharedMediaPad pad)> threadFunction):pad(pad), thread(new boost::thread(threadFunction, pad)){};
		~PadThread(){}
		
		bool operator==(const SharedMediaPad &thatPad) const{return (pad == thatPad);}
		SharedMediaPad pad; 
		boost::shared_ptr<boost::thread> thread;
	};
	typedef std::vector<PadThread> PadThreads;

	SharedMediaFormat m_outputFormat;

	std::queue<SharedMediaSample> m_sampleQueue;
	boost::mutex m_sampleQueueMutex;
	boost::condition_variable m_sampleEvent;
	boost::condition_variable m_sampleProcessEvent;
	PadThreads m_threads;

	unsigned int m_lastSequence;
};

#endif // IMAGEVIEWER_H
