#ifndef IMAGEVIEWER_GLOBAL_H
#define IMAGEVIEWER_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef IMAGEVIEWER_LIB
# define IMAGEVIEWER_EXPORT Q_DECL_EXPORT
#else
# define IMAGEVIEWER_EXPORT Q_DECL_IMPORT
#endif

#endif // IMAGEVIEWER_GLOBAL_H
