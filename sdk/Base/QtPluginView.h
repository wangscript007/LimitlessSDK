#ifndef _IIMSDK_QtPluginView_h_
#define _IIMSDK_QtPluginView_h_

#include "Base/PluginView.h"
#include <QtWidgets/QWidget>

class QtPluginView:public PluginView
{
public:
	QtPluginView(QWidget *widget):widget(widget){};
	virtual ~QtPluginView(){};

	virtual Type getType(){return PluginView::QT;}
	
	QWidget *widget;
};
typedef boost::shared_ptr<QtPluginView> SharedQtPluginView;
#endif //_IIMSDK_QtPluginView_h_