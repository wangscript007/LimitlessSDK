#ifndef _DIRECTSHOWCAPTURE_H_
#define _DIRECTSHOWCAPTURE_H_

#include <AtlBase.h>
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__

//#include <Qedit.h>
#include <windows.h>
#include <dshow.h>
#include "dshowutil.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QImage>
#include <QtCore/QThread>
#include "ICaptureDevice.h"
#include <vector>

class SampleGrabberCB;
class DirectShowCapture;

//////////////////////////////////////////////////////////////////////
// DirectShowCaptureEvent Declaration/Implementation
//////////////////////////////////////////////////////////////////////
class DSCaptureEventThread:public QThread
{
public:
	DSCaptureEventThread(DirectShowCapture *parent):m_parent(parent),m_run(true){};

	void stop(){m_run=false;};
private:
	virtual void run();

	DirectShowCapture *m_parent;
	bool volatile m_run;
};

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
class DirectShowCapture:public MediaAutoRegister<DirectShowCapture, IMediaSource>
{
public:
//	DirectShowCapture(const QString &device, const QString &uid);
	DirectShowCapture(std::string name, SharedMediaFilter parent);
	virtual ~DirectShowCapture();

	//PluginObject
	virtual bool initialize(const Attributes &attributes);
	virtual bool shutdown();

	//IMediaFilter
	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);

	//IMediaSource
	virtual MediaDevices devices();
	virtual bool connect(MediaDevice mediaDevice);
	virtual bool disconnect();
	virtual bool connected();

	virtual bool isAvailable(){return true;}
	virtual std::string name(){return m_device;}
	virtual std::string uid(){return m_uid;}

//	virtual bool open();
//	virtual bool close();

//	virtual int start();
//	virtual int stop();

	typedef int(*ISourceCallback)(QImage &, QImage &);
	int setCallback(ISourceCallback callback);

	//////////////////////////////////////////////////////////////////////
	void grabberCallback(std::string input, double SampleTime, QImage frame);

protected:
	virtual void onPinOpen(std::string pinName);

	std::string m_device;
	std::string m_uid;

private:
	HRESULT getDevice(const std::string &device, const std::string &uid, IBaseFilter **filter);

	int eventHandler();
	int buffer(QImage &color, QImage &ir);
	
	friend SampleGrabberCB;
	friend DSCaptureEventThread;

	DSCaptureEventThread m_eventThread;
	IGraphBuilder *m_graph;
	IMediaControl *m_control;
	IMediaSeeking *m_seeking;
	IMediaEvent *m_events;
	IBaseFilter *m_source;
//	ISampleGrabber *m_grabber;
	IBaseFilter *m_nullRenderer;
	SampleGrabberCB *m_callback;
};


#endif //_DIRECTSHOWCAPTURE_H_