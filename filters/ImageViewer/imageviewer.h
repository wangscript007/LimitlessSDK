#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"
//#include "imageviewer_global.h"
#include "glView.h"

#include <queue>

class ImageViewer:public Limitless::MediaAutoRegister<ImageViewer, Limitless::IMediaFilter>
{
public:
	ImageViewer(std::string name, Limitless::SharedMediaFilter parent);
	~ImageViewer();

	virtual bool initialize(const Limitless::Attributes &attributes);
	virtual bool shutdown(){return true;}

	virtual Limitless::SharedPluginView getView();

	virtual bool processSample(Limitless::SharedMediaPad sinkPad, Limitless::SharedMediaSample sample);

protected:
	//IMediaFilter
	virtual StateChange onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();

	bool onAcceptMediaFormat(Limitless::SharedMediaPad pad, Limitless::SharedMediaFormat format);
	void onLinkFormatChanged(Limitless::SharedMediaPad pad, Limitless::SharedMediaFormat format);

private:
	Limitless::SharedPluginView m_view;
	GlView *m_glView;

	std::queue<MediaTime> m_timeStamps;
	int frameCount;

};

namespace Limitless{namespace traits
{
	template<> struct category<ImageViewer>
	{
		static std::string get()
		{return "sink";}
	};
}}//Limitless::traits

#endif // IMAGEVIEWER_H
