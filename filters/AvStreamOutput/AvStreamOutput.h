#ifndef _AvStreamOutput_h_
#define _AvStreamOutput_h_

#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"
//#include "imageviewer_global.h"
#include "AvStream/AvStreamServer.h"

class AvStreamOutput:public MediaAutoRegister<AvStreamOutput, IMediaFilter>
{
public:
	AvStreamOutput(std::string name, SharedMediaFilter parent);
	~AvStreamOutput();

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

private:
	AvStreamServer m_streamServer;

	int m_width;
	int m_height;

	unsigned int m_imageSampleId;
	unsigned int m_imageSetSampleId;

	std::string m_fmleName;
};

#endif // _AvStreamOutput_h_
