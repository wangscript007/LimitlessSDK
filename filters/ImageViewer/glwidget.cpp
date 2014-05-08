#include "glwidget.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "Base/Log.h"
#include <QtGui/QImage>
#include "Media/ImageSetSample.h"
#include <QtGui/QMouseEvent>

#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glu.h>

#define TEXTURE_RECTANGLE_ARB 0x84F5
#ifndef GL_BGR 
#define GL_BGR 0x80E0 
#endif //GL_BGR 
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif //GL_BGRA

using namespace boost::numeric;
using namespace Limitless;

GLWidget::GLWidget(QWidget *parent):
QGLWidget(parent),
m_displayMode(SINGLE),
m_displayModeChanged(true),
m_currentImage(0),
m_clipNear(1.0f),
m_clipFar(1000.0f),
m_transX(0.0f),
m_transY(0.0f),
m_transZ(-10.0f),
m_rotX(0.0f),
m_rotY(0.0f),
m_rotZ(0.0f),
m_init(false),
m_endThread(false),
m_readyToInit(false)
{
	setAutoBufferSwap(false);

	m_imageSampleId=MediaSampleFactory::getTypeId("ImageSample");
	m_imageSetSampleId=MediaSampleFactory::getTypeId("ImageSetSample");
	m_GPUImageSampleId=MediaSampleFactory::getTypeId("GPUImageSample");
	m_GPUImageGLSampleId=MediaSampleFactory::getTypeId("GPUImageGLSample");

	doneCurrent();
	m_thread=boost::thread(boost::bind(&GLWidget::drawThread, this));
//	m_event.notify_one();
}

GLWidget::~GLWidget()
{
	m_endThread=true;
	m_event.notify_one();
	m_thread.join();
}

bool GLWidget::initialize()
{
	QGLContext *glContext=new QGLContext(QGLFormat::defaultFormat(), this);

	setContext(glContext);
//	GLenum error;
//
//	makeCurrent();

//	if((error=glGetError()) != GL_NO_ERROR)
//	{
//		std::string errorString=(boost::format("Error in initialize: %d\n")%(int)error).str();
//		Log::write(errorString);
//		return false;
//	}

	//Set viewport and get textureType
//	const unsigned char *pExtensions=glGetString(GL_EXTENSIONS);

//	if(strstr(reinterpret_cast<const char*>(pExtensions), "GL_ARB_texture_rectangle"))
		m_textureType=TEXTURE_RECTANGLE_ARB;
//	else
	checkErrorGL();
//		m_textureType=GL_TEXTURE_2D;
	glEnable(m_textureType);
	checkErrorGL();
	
	GLboolean textureSet; 
	glGetBooleanv(m_textureType, &textureSet);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	checkErrorGL();
//	doneCurrent();

	m_init=true;
	return true;
}

void GLWidget::glInit()
{
//	QGLWidget::glInit();
}

void GLWidget::glDraw()
{
//	QGLWidget::glDraw();
	m_event.notify_one();
}

void GLWidget::initializeGL()
{
//	initialize();
//	m_event.notify_one();
//	m_readyToInit=true;
}

void GLWidget::resizeGL(int width, int height)
{
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	double ratio=(double)width/height;
//	m_fovY=25.0f*(M_PI/180.0f);
//	m_fovX=2.0f*atan(ratio*tan(m_fovY/2));
//	gluPerspective(m_fovY*(180.0f/M_PI), ratio, m_clipNear, m_clipFar);
//
//	glMatrixMode(GL_MODELVIEW);
//	checkErrorGL();

//	m_resize=true;
//	m_event.notify_one();
}

void GLWidget::paintGL()
{
//	drawImage(m_images);
//	m_event.notify_one();
}

void GLWidget::fitToScreen()
{
	float transX, transY;

	m_transX=0.0f;
	m_transY=0.0f;
	
	transX=-m_imageQuadWidth/(2.0f*tan(m_fovX/2.0f));
	transY=-m_imageQuadHeight/(2.0f*tan(m_fovY/2.0f));
	
	if(transX < transY)
		m_transZ=transX;
	else
		m_transZ=transY;
}

void GLWidget::setDisplayMode(DisplayMode mode)
{
	m_displayMode=mode;
	m_displayModeChanged=true;
}

void GLWidget::displaySample(SharedMediaSample sample)
{
	std::vector<QImage> images;
	bool initImages=true;

//	makeCurrent();
//
//	if(!m_init)
//		initialize();
//
//	if(m_resize)
//	{
//		m_resize=false;
//		glViewport(0, 0, width(), height());
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//
//		double ratio=(double)width()/height();
//		m_fovY=25.0f*(M_PI/180.0f);
//		m_fovX=2.0f*atan(ratio*tan(m_fovY/2));
//		gluPerspective(m_fovY*(180.0f/M_PI), ratio, m_clipNear, m_clipFar);
//
//		glMatrixMode(GL_MODELVIEW);
//		checkErrorGL();
//	}
//
	if(sample->type() == m_imageSampleId)
	{
		SharedImageSample imageSample=boost::dynamic_pointer_cast<ImageSample>(sample);

		if(imageSample != SharedImageSample())
		{
			if(m_images.size() > 0)
			{
				if((m_images[0].width() == imageSample->width()) && (m_images[0].width() == imageSample->width()))
					initImages=false;
			}

			if(initImages)
			{
				boost::unique_lock<boost::mutex> lock(m_mutex);

				m_images.clear();

				if(imageSample->channels() == 3)
					m_images.push_back(QImage(imageSample->width(), imageSample->height(), QImage::Format_RGB888));
				else if(imageSample->channels() == 4)
					m_images.push_back(QImage(imageSample->width(), imageSample->height(), QImage::Format_ARGB32));
			}
	
			int copySize=std::min<int>(m_images[0].byteCount(), imageSample->size());
			
			memcpy(m_images[0].bits(), imageSample->buffer(), copySize);
		}
	}
	else if(sample->type() == m_imageSetSampleId) //Handle ImageSetSamples
	{
		SharedImageSetSample imageSetSample=boost::dynamic_pointer_cast<ImageSetSample>(sample);

		if(imageSetSample != SharedImageSetSample())
		{
			unsigned int sampleCount=imageSetSample->setSize();

			IImageSample *imageSample0=imageSetSample->operator[](0);
			int imageCount=m_images.size();

			if(imageCount > 0)
			{
				if(imageCount == sampleCount)
				{
					if((m_images[0].width() == imageSample0->width()) && (m_images[0].height() == imageSample0->height()))
						initImages=false;
				}
			}

			if(initImages)
			{
				boost::unique_lock<boost::mutex> lock(m_mutex);

				m_images.clear();
				for(int i=0; i<sampleCount; ++i)
				{
					if(imageSample0->channels() == 3)
						m_images.push_back(QImage(imageSample0->width(), imageSample0->height(), QImage::Format_RGB888));
					else if(imageSample0->channels() == 4)
						m_images.push_back(QImage(imageSample0->width(), imageSample0->height(), QImage::Format_ARGB32));
				}
			}

			for(unsigned int i=0; i<sampleCount; ++i)
			{
				IImageSample *imageSample=imageSetSample->operator[](i);
				
				unsigned char *dstBuffer=m_images[i].bits();
				int dstSize=m_images[i].byteCount();
				unsigned char *srcBuffer=imageSample->buffer();
				int srcSize=imageSample->size();
				
				memcpy(dstBuffer, srcBuffer, srcSize);
//				memcpy(m_images[i].bits(), imageSample.buffer(), imageSample.size());
//				images.push_back(QImage(imageSample.buffer(), imageSample.width(), imageSample.height(), QImage::Format_RGB888));
//				images.push_back(QImage(imageSample.buffer(), imageSample.width(), imageSample.height(), QImage::Format_ARGB32));
			}
		}
	}

	m_event.notify_one();
//	if(m_displayModeChanged)
//	{
//		setupDisplayMode(m_displayMode, m_images);
//		m_currentImage=0;
//
////		m_images[0].save("test.jpg");
//	}
////	updateGL();
//	drawImage(m_images);
//
//	swapBuffers();
//	doneCurrent();
}

void GLWidget::resizeEvent(QResizeEvent *evt)
{
	m_resize=true;
	m_event.notify_one();
}

//void GLWidget::paintEvent(QPaintEvent *)
//{
//	// Handled by the GLThread.
//	m_event.notify_one();
//}

void GLWidget::closeEvent(QCloseEvent *evt)
{
	m_endThread=true;
	m_event.notify_one();
	QGLWidget::closeEvent(evt);
}

bool GLWidget::event(QEvent *e)
{
	if(e->type() == QEvent::Show)
	{
		m_event.notify_one();
	}
	else if(e->type() == QEvent::ParentChange) //The glContext will be changed, need to reinit openGl
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);

		//need to init
		m_init=false;
		bool ret=QGLWidget::event(e);
		m_event.notify_one();
		return ret;
	}
	else if(e->type() == QEvent::Resize)
	{
		return QGLWidget::event(e);
	}
	return QGLWidget::event(e);
}


void GLWidget::drawThread()
{
//	while(!m_readyToInit)
//	{
//		boost::unique_lock<boost::mutex> lock(m_mutex);
//
//		m_event.wait(lock); //wait for event
//		if(m_endThread)
//			return;
//	}
	while(true)
	{
		{
			boost::unique_lock<boost::mutex> lock(m_mutex);

			m_event.wait(lock); //wait for event

			if(m_endThread)
				break;

//			if(!m_init)
//				moveToThread(QThread * thread)

			if(!m_init)
				initialize();

			makeCurrent();
			checkErrorGL();

			if(m_resize)
			{
				m_resize=false;
				glViewport(0, 0, width(), height());
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				double ratio=(double)width()/height();
				m_fovY=25.0f*(M_PI/180.0f);
				m_fovX=2.0f*atan(ratio*tan(m_fovY/2));
				gluPerspective(m_fovY*(180.0f/M_PI), ratio, m_clipNear, m_clipFar);

				glMatrixMode(GL_MODELVIEW);
				checkErrorGL();

				fitToScreen();
			}

			if(m_displayModeChanged)
			{
				setupDisplayMode(m_displayMode, m_images);
				m_currentImage=0;

				//		m_images[0].save("test.jpg");
			}
			drawImage(m_images);

			swapBuffers();
			doneCurrent();		
		}
	}
}

void GLWidget::setupDisplayMode(DisplayMode mode, const std::vector<QImage> &images)
{
//	if(!m_displayModeChanged)
//		return;
	if(images.size() <= 0)
		return;

	switch(mode)
	{
	case SINGLE:
		setupSingleImage(images[0]);
		break;
	case MULTIPLE:
		setupleMultipleImages(images);
		break;
	case CYLINDRICAL:
		setupCylindrical();
		break;
	}
	fitToScreen();
}

void GLWidget::setupSingleImage(const QImage &image)
{
	float aspect;
	float positionX, positionY;
	int imageWidth=image.width();
	int imageHeight=image.height();

	if(imageWidth > imageHeight)
	{
		aspect=(float)imageHeight/imageWidth;
		positionX=1.0;
		positionY=positionX*aspect;
	}
	else
	{
		aspect=(float)imageWidth/imageHeight;
		positionY=1.0;
		positionX=positionY*aspect;
	}

	m_imageQuadWidth=2.0*positionX;
	m_imageQuadHeight=2.0*positionY;

	Quad imageQuad;

//	imageQuad.coords[0]=Coord(positionX, positionY, 0.0f, 0.0f, 0.0f);
//	imageQuad.coords[1]=Coord(positionX, -positionY, 0.0f, 0.0f, imageHeight);
//	imageQuad.coords[2]=Coord(-positionX, -positionY, 0.0f, imageWidth, imageHeight);
//	imageQuad.coords[3]=Coord(-positionX, positionY, 0.0f, imageWidth, 0.0f);
//	imageQuad.coords[0]=Coord(-positionX, positionY, 0.0f, 0.0f, 0.0f);
//	imageQuad.coords[1]=Coord(-positionX, -positionY, 0.0f, 0.0f, 1.0f);
//	imageQuad.coords[2]=Coord(positionX, -positionY, 0.0f, 1.0f, 1.0f);
//	imageQuad.coords[3]=Coord(positionX, positionY, 0.0f, 1.0f, 0.0f);
	imageQuad.coords[0]=Coord(-positionX, positionY, 0.0f, 0.0f, 0.0f);
	imageQuad.coords[1]=Coord(-positionX, -positionY, 0.0f, 0.0f, imageHeight);
	imageQuad.coords[2]=Coord(positionX, -positionY, 0.0f, imageWidth, imageHeight);
	imageQuad.coords[3]=Coord(positionX, positionY, 0.0f, imageWidth, 0.0f);
	
	m_imageQuads.clear();
	m_imageQuads.push_back(imageQuad);
	m_displayModeChanged=false;
}

void GLWidget::setupleMultipleImages(const std::vector<QImage> &images)
{
	int imagesX=sqrt((float)images.size());
	int imagesY;

	if(images.size() <= (imagesX*imagesX))
		imagesY=imagesX;
	else if(images.size() <= (imagesX*(imagesX+1)))
		imagesY=imagesX+1;
	else
	{
		imagesX++;
		imagesY=imagesX;
	}

	int maxWidth=0;
	int maxHeight=0;
	
	BOOST_FOREACH(const QImage &image, images)
	{
		if(image.width() > maxWidth)
			maxWidth=image.width();
		if(image.height() > maxHeight)
			maxHeight=image.height();
	}

	int totalWidth=maxWidth*imagesX;
	int totalHeight=maxHeight*imagesY;
	
	float positionX, positionY;
	float aspect;

	aspect=totalHeight/totalWidth;
	if(totalWidth > totalHeight)
	{
		positionX=-1.0f;
		positionY=-positionX/aspect;
	}
	else
	{
		positionY=1.0f;
		positionX=positionY*aspect;
	}

	m_displayModeChanged=false;
}

void GLWidget::setupCylindrical()
{
	m_displayModeChanged=false;
}

void GLWidget::drawImage(std::vector<QImage> &images)
{
//	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	glTranslatef(m_transX, m_transY, m_transZ);
	glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);

	switch(m_displayMode)
	{
	case SINGLE:
		{
			if((m_currentImage >= 0) && (m_currentImage < images.size()))
				drawSingleImage(images[m_currentImage]);
		}
		break;
	case MULTIPLE:
		break;
	case CYLINDRICAL:
		break;
	}

//	glBegin(GL_TRIANGLES);
//	{
//		glColor3f(1.0f, 0.0f, 0.0f);
//		glVertex3f(-2.0f, 2.0f, -1.0f);
//		glColor3f(0.0f, 1.0f, 0.0f);
//		glVertex3f(-2.0f, -2.0f, -1.0f);
//		glColor3f(0.0f, 0.0f, 1.0f);
//		glVertex3f(2.0f, -2.0f, -1.0f);
//	}
//	glEnd();

//	swapBuffers();
//	doneCurrent();
}

void GLWidget::drawSingleImage(QImage &image)
{
	if((image.width() <= 0) || (image.height() <= 0))
		return;

	int width, height;
	unsigned char *imageData;

	imageData=image.bits();
	width=image.width();
	height=image.height();

//	image.save("test.jpg");

	if(imageData == NULL)
		return;

	if(m_textures.size() < 1)
		allocateTextures(1);

	checkErrorGL();
	glBindTexture(m_textureType, m_textures[0]);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	checkErrorGL();
//	if(m_textureType == TEXTURE_RECTANGLE_ARB)
	glColor4f(1.0, 1.0, 1.0, 1.0);

	if(image.format() == QImage::Format_RGB888)
		glTexImage2D(m_textureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); 
	else if(image.format() == QImage::Format_ARGB32)
		glTexImage2D(m_textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); 
	else
		return;
	glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//		glTexImage2D(m_textureType, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData); 
	checkErrorGL();
//		glTexImage2D(m_textureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); 
//	else
//		gluBuild2DMipmaps(m_textureType, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	checkErrorGL();

	glBegin(GL_QUADS);
	{
		for(Quads::iterator iter=m_imageQuads.begin(); iter!=m_imageQuads.end(); ++iter)
		{
			for(int i=0; i<4; ++i)
			{
				Coord &coord=iter->coords[i];
				
				glTexCoord2f(coord.texture.x, coord.texture.y);
				glVertex3f(coord.point.x, coord.point.y, coord.point.z);
			}
		}
	}
	glEnd();
	checkErrorGL();
}

void GLWidget::allocateTextures(int textureCount)
{
	if(m_textures.size() < textureCount)
	{
		if(!m_textures.empty())
			glDeleteTextures(m_textures.size(), &m_textures[0]);
		m_textures.clear();
	}

	m_textures.resize(textureCount);
	glGenTextures(textureCount, &m_textures[0]);
	checkErrorGL();
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
	Qt::MouseButton button=event->button();

	if(Qt::LeftButton == button)
		m_leftClick=event->globalPos();
	if(Qt::RightButton == button)
		m_rightClick=event->globalPos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	this->setCursor(QCursor(Qt::OpenHandCursor));
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	Qt::MouseButtons buttons=event->buttons();

	if(Qt::LeftButton&buttons)
	{
		bool rightButton=Qt::RightButton&buttons;

		if(!rightButton)
		{
			Qt::KeyboardModifiers keys=event->modifiers();

			if(Qt::AltModifier&keys)
			{
			}
			else
			{
				m_rotY += (float)(event->globalX()-m_leftClick.x())*(360.0f/width());
				m_rotX += (float)(m_leftClick.y()-event->globalY())*(360.0f/height());
//				updateGL();
				m_event.notify_one();
			}
		}
		m_leftClick = event->globalPos();
	}
}

void GLWidget::checkErrorGL()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString=gluErrorString(errCode);
		
		std::string error=(boost::format("GL error: %s\n")%errString).str();
		
		OutputDebugStringA((LPSTR)error.c_str());
	}
}