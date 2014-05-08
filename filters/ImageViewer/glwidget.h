#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QtGui/QImage>
//#include <QtCore/QEvent>
//#include <boost/atomic.hpp>
#include "Media/MediaSample.h"
#include <boost/thread.hpp>

class Vector2f
{
public:
	Vector2f():x(0.0), y(0.0){};
	Vector2f(float x, float y):x(x), y(y){};
	~Vector2f(){};

	float x, y;
};

class Vector3f
{
public:
	Vector3f():x(0.0), y(0.0), z(0.0){};
	Vector3f(float x, float y, float z):x(x), y(y), z(z){};
	~Vector3f(){};

	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float r, g, b;
		};
	};
};

class Coord
{
public:
	Coord(){};
	Coord(float x, float y, float z, float tx, float ty){point=Vector3f(x, y, z);texture=Vector2f(tx, ty);}
	Coord(Vector3f point, Vector2f texture):point(point), texture(texture){};

	Vector3f point;
	Vector2f texture;
};

class Quad
{
public:
	Quad(){}
	Quad(Coord coord1, Coord coord2, Coord coord3, Coord coord4){coords[0]=coord1; coords[1]=coord2; coords[2]=coord3; coords[3]=coord4;}
	Quad(std::vector<Coord> coords)
	{
		for(int i=0; i<coords.size() && i<4; ++i) 
			coords[i]=coords[i];
	};
	~Quad(){};

	Coord coords[4];
};
typedef std::vector<Quad> Quads;


class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent);
	~GLWidget();

	enum DisplayMode
	{
		SINGLE,
		MULTIPLE,
		CYLINDRICAL
	};

	bool initialize();
	void setDisplayMode(DisplayMode mode);

	void displaySample(Limitless::SharedMediaSample sample);

	void drawThread();
protected:
	bool event(QEvent *e);

	virtual void glInit();
	virtual void glDraw();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	void resizeEvent(QResizeEvent *evt);
//	void paintEvent(QPaintEvent *);
	void closeEvent(QCloseEvent *evt);

private:
	void checkErrorGL();

	void setupDisplayMode(DisplayMode mode, const std::vector<QImage> &images);
	void setupSingleImage(const QImage &image);
	void setupleMultipleImages(const std::vector<QImage> &images);
	void setupCylindrical();

	void fitToScreen();

	void drawImage(std::vector<QImage> &images);
	void drawSingleImage(QImage &image);

	void allocateTextures(int textureCount);

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

	boost::thread m_thread;
	boost::condition_variable m_event;
	boost::mutex m_mutex;
	bool m_endThread;

	bool m_readyToInit;
	bool m_init;
	bool m_resize;
	DisplayMode m_displayMode;
//	boost::atomic<bool> m_displayModeChanged;
	bool m_displayModeChanged;
	std::vector<QImage> m_images;
	int m_currentImage;

	Quads m_imageQuads;

	GLenum m_textureType;
	std::vector<unsigned int> m_textures;

	size_t m_imageSampleId;
	size_t m_imageSetSampleId;
	size_t m_GPUImageSampleId;
	size_t m_GPUImageGLSampleId;

	float m_fovX;
	float m_fovY;
	float m_clipNear;
	float m_clipFar;

	float m_imageQuadWidth;
	float m_imageQuadHeight;

	QPoint m_leftClick;
	QPoint m_rightClick;

	float m_transX;
	float m_transY;
	float m_transZ;
	float m_rotX;
	float m_rotY;
	float m_rotZ;
};

#endif // GLWIDGET_H
