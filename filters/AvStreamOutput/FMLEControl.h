#ifndef _FMLEControl_h_
#define _FMLEControl_h_

//#include <boost/serialization/list.hpp>
//#include <boost/serialization/string.hpp>
//#include <boost/serialization/nvp.hpp>
#include "Media/MediaPluginFactory.h"
#include "Media/IMediaFilter.h"
#include "FMLEProfile.h"

#include <vector>

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

#include <boost/thread.hpp>

//class FMLEProfile
//{
//};

class FMLEControl:public MediaAutoRegister<FMLEControl, IMediaFilter>
{
public:
	FMLEControl(std::string name, SharedMediaFilter parent);
	~FMLEControl();

	//PluginObject
	virtual bool initialize(const Attributes &attributes);
	virtual bool shutdown();
	virtual SharedPluginView getView();
	
	//IMediaFilter
	virtual bool processSample(SharedMediaPad sinkPad, SharedMediaSample sample);

	std::string findInstalation();
	std::string buildOptions();

	void FMLEControl::start();
	void FMLEControl::stop();

	void FMLEControl::processThread();

	typedef std::vector<CHAR_INFO> CharInfoVector;
protected:
	//IMediaFilter
	virtual StateChange onReady();
	virtual StateChange onPaused();
	virtual StateChange onPlaying();

	bool onAcceptMediaFormat(SharedMediaPad pad, SharedMediaFormat format);
	void onLinkFormatChanged(SharedMediaPad pad, SharedMediaFormat format);

private:
	std::string converToString(const CharInfoVector &buffer, const COORD &size);
	void updateProfileAttributes();
	void updateProfile();

	std::string getProgramFolder();
	HANDLE attachConsole(HANDLE process);

	std::string m_fmleExecutable;
	
	bool m_run;
	boost::thread m_statusThread;

	HANDLE m_processHandle;

	std::string m_fmleProfilePath;
	FMLEProfile m_fmleProfile;
};

#endif //_FMLEControl_h_