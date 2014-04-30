#include "glview.h"

GlView::GlView(QWidget *parent):
QWidget(parent)
{
	ui.setupUi(this);

	connect(this, SIGNAL(setFrameRateSignal(double)), this, SLOT(onFrameRate(double)), Qt::QueuedConnection);
}

GlView::~GlView()
{

}

void GlView::setDisplayMode(GLWidget::DisplayMode displayMode)
{
	ui.openglWidget->setDisplayMode(displayMode);
}

void GlView::displaySample(Limitless::SharedMediaSample sample)
{
	ui.openglWidget->displaySample(sample);
}

void GlView::setFrameRate(double frameRate)
{
	emit(setFrameRateSignal(frameRate));
}

void GlView::onFrameRate(double frameRate)
{
	ui.frameRate->setText(QString("%1").arg(frameRate));
}