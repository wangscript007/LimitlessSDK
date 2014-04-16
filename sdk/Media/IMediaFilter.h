#ifndef _IIMediaSDK_IMediaFilter_h_
#define _IIMediaSDK_IMediaFilter_h_

#include "Media/media_define.h"

#include "Base/PluginObject.h"
#include "Media/MediaSample.h"
#include "Media/MediaFormat.h"
#include "Media/MediaPad.h"
#include "Media/MediaSourcePad.h"
#include "Media/MediaSinkPad.h"
#include "Media/MediaClock.h"

#include "boost/enable_shared_from_this.hpp"
//class IMediaFilter;
//typedef boost::shared_ptr<IMediaFilter> SharedMediaFilter;

class MEDIA_EXPORT IMediaFilter:public boost::enable_shared_from_this<IMediaFilter>, public PluginObject
{
public:
	IMediaFilter(std::string instance, boost::shared_ptr<IMediaFilter> parent);
//	IMediaFilter(std::string instance, boost::shared_ptr<IMediaFilter> parent);
	virtual ~IMediaFilter(){};

//Filter pad/connections
	virtual SharedMediaPads getSourcePads();
	virtual SharedMediaPads getSinkPads();

//	bool accept(boost::shared_ptr<IMediaFilter> filter)
//	bool accept(SharedMediaPad filterPad, SharedMediaFormat mediaFormat);
//	bool accept(SharedMediaFormat mediaFormat);
//	bool accept(SharedMediaPad mediaPad, SharedMediaFormat mediaFormat);
	
	virtual bool link(boost::shared_ptr<IMediaFilter> sinkFilter);
	
//	virtual SharedMediaFormat accept(SharedMediaFormat format=SharedMediaFormat());
//	bool accept(SharedMediaFilter sink, SharedMediaFormat format);
//	virtual SharedMediaFormat accept(boost::shared_ptr<IMediaFilter> filter, SharedMediaFormat format=SharedMediaFormat());
//	bool link(boost::shared_ptr<IMediaFilter> filter, SharedMediaFormat format=SharedMediaFormat());
//	virtual SharedMediaFormat linkPad(boost::shared_ptr<IMediaFilter> filter, SharedMediaPad pad, SharedMediaFormat format=SharedMediaFormat());
//	virtual SharedMediaFormat connect(boost::shared_ptr<IMediaFilter> filter, SharedMediaFormat format=SharedMediaFormat());
	
	virtual void disconnect(boost::shared_ptr<IMediaFilter> sink);
//	virtual bool processSample(SharedMediaSample sample)=0;
//	onPushSample(SharedMediaSample sample);
	virtual bool inStream(boost::shared_ptr<IMediaFilter> filter);
	virtual bool upStream(boost::shared_ptr<IMediaFilter> filter);
	virtual bool downStream(boost::shared_ptr<IMediaFilter> filter);

	virtual std::vector<boost::shared_ptr<IMediaFilter> > upStreamFilters();
	virtual std::vector<boost::shared_ptr<IMediaFilter> > downStreamFilters();

	virtual SharedMediaSample newSample(unsigned int type);
	virtual SharedMediaSample newSample(unsigned int type, unsigned __int64 allocBin);
	virtual void deleteSample(SharedMediaSample sample);

	MediaTime getTime();

	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample)=0;

//Filter state
	enum FilterState
	{
		INIT,
		READY,
		PAUSED,
		PLAYING
	};
	enum StateChange
	{
		FAILED,
		ASYNC,
		SUCCESS
	};
	IMediaFilter::FilterState getState() const;
	StateChange setState(IMediaFilter::FilterState state);
	StateChange ready();
	StateChange pause();
	StateChange play();

protected:
//Pad creation
	void addSourcePad(SharedMediaPad mediaPad);
	void addSourcePad(const char *jsonFormat);
	void addSourcePad(const std::string &jsonFormat);
	void addSourcePad(SharedMediaFormat format);
	void addSinkPad(SharedMediaPad mediaPad);
	void addSinkPad(const char *jsonFormat);
	void addSinkPad(const std::string &jsonFormat);
	void addSinkPad(SharedMediaFormat format);


//Pad Control
//	bool connectLinkedPads();
	bool reLinkPads();
	virtual bool onAcceptMediaFormat(SharedMediaPad Pad, SharedMediaFormat format);
//	virtual bool linkSourcePad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format);
//	virtual bool linkSinkPad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format);
//	virtual SharedMediaFormat connectSourcePad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format);
//	virtual SharedMediaFormat connectSinkPad(SharedMediaPad sourcePad, SharedMediaPad sinkPad, SharedMediaFormat format);
	friend MediaPad;

	virtual void onLinked(SharedMediaPad pad, SharedMediaPad filterPad){}
	virtual void onDisconnected(SharedMediaPad pad, SharedMediaPad filterPad){}

//Filter transition functions
	virtual StateChange onReady(){return SUCCESS;}
	virtual StateChange onPaused(){return SUCCESS;}
	virtual StateChange onPlaying(){return SUCCESS;}

	virtual void onLinkFormatChanged(SharedMediaPad mediaPad, SharedMediaFormat format);

	void pushSample(SharedMediaSample sample);
private:
	FilterState m_state;

//	SharedMediaFilter m_parent;
	boost::shared_ptr<IMediaFilter> m_parent;
	SharedMediaPads m_mediaSourcePads;
	SharedMediaPads m_mediaSinkPads;

	SharedMediaClock m_mediaClock;
};
typedef boost::shared_ptr<IMediaFilter> SharedMediaFilter;
typedef std::vector<SharedMediaFilter> SharedMediaFilters;

#endif //_IIMediaSDK_IMediaFilter_h_