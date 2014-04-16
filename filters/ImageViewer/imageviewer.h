#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"
//#include "imageviewer_global.h"
#include "glView.h"

#include <queue>

class ImageViewer:public MediaAutoRegister<ImageViewer, IMediaFilter>
{
public:
	ImageViewer(std::string name, SharedMediaFilter parent);
	~ImageViewer();

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
	SharedPluginView m_view;
	GlView *m_glView;

	std::queue<MediaTime> m_timeStamps;
	int frameCount;

};

#endif // IMAGEVIEWER_H
