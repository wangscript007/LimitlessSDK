#include "imageviewer.h"
#include "Base/QtPluginView.h"
#include "Media/MediaPad.h"

#include <boost/foreach.hpp>

using namespace Limitless;

ImageViewer::ImageViewer(std::string name, SharedMediaFilter parent):
MediaAutoRegister(name, parent),
m_glView(NULL),
frameCount(0)
{
}

ImageViewer::~ImageViewer()
{

}

bool ImageViewer::initialize(const Attributes &attributes)
{
	addSinkPad("[{\"mime\":\"video/raw\"}, {\"mime\":\"image/raw\"}]");
	addSourcePad("[{\"mime\":\"video/raw\"}, {\"mime\":\"image/raw\"}]");

	Strings displayModes;

	displayModes.push_back("All");
	displayModes.push_back("Single");
	addAttribute("displayMode", displayModes[0], displayModes);
	addAttribute("currentImage", 0);
	return true;
}

SharedPluginView ImageViewer::getView()
{
//	return SharedPluginView();

	if(m_view == SharedPluginView())
	{
		m_glView=new GlView();
		m_view.reset(new QtPluginView(m_glView));
	}
	return m_view;
}

bool ImageViewer::processSample(SharedMediaPad sinkPad, SharedMediaSample sample)
{
//	OutputDebugStringA("Enter ImageViewer::processSample\n");
	//	sourcePad->processSample(sample);
	frameCount++;
	if(frameCount%10 == 0)
	{
		m_timeStamps.push(sample->timestamp());
	
		double frameRate=0;
		if((frameCount != 0) && !m_timeStamps.empty())
			frameRate=((double)frameCount*1000000000.0)/(m_timeStamps.back()-m_timeStamps.front());
		m_glView->setFrameRate(frameRate);
	}

	if(m_timeStamps.size() > 10)
	{
		m_timeStamps.pop();
		frameCount-=10;
	}

	if(m_glView != NULL)
		m_glView->displaySample(sample);

	pushSample(sample);
//	OutputDebugStringA("Exit ImageViewer::processSample\n");
	return true;
}

IMediaFilter::StateChange ImageViewer::onReady()
{
	return SUCCESS;
}

IMediaFilter::StateChange ImageViewer::onPaused()
{
	return SUCCESS;
}

IMediaFilter::StateChange ImageViewer::onPlaying()
{
	return SUCCESS;
}

bool ImageViewer::onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SINK)
	{
		bool accept=false;

		if(format->exists("mime"))
		{
			std::string mimeType=format->attribute("mime")->toString();

			if(mimeType == "video/raw")
				accept=true;
			else if(mimeType == "image/raw")
				accept=true;
		}
		return accept;
	}
	else
		return true;
	return false;
}

void ImageViewer::onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format)
{
	if(pad->type() == MediaPad::SINK)
	{
		if(!format->exists("mime"))
			return;
		if((format->attribute("mime")->toString() != "video/raw") &&
			(format->attribute("mime")->toString() != "image/raw"))
			return;

		MediaFormat sourceFormat;

		sourceFormat.addAttribute("mime", format->attribute("mime")->toString());
		if(format->exists("width"))
			sourceFormat.addAttribute("width", format->attribute("width")->toString());
		if(format->exists("height"))
			sourceFormat.addAttribute("height", format->attribute("height")->toString());

		SharedMediaPads sourcePads=getSourcePads();

		BOOST_FOREACH(SharedMediaPad &sourcePad, sourcePads)
		{
			sourcePad->setFormat(sourceFormat);
		}
	}
}