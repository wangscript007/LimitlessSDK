#ifndef _IIMSDK_PluginView_h_
#define _IIMSDK_PluginView_h_

#include "boost\shared_ptr.hpp"

class PluginView
{
public:
	PluginView(){};
	virtual ~PluginView(){};

	enum Type
	{
		QT
	};

	virtual Type getType()=0;
};

typedef boost::shared_ptr<PluginView> SharedPluginView;
#endif //_IIMSDK_PluginView_h_