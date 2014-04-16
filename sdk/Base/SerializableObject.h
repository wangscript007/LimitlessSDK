#ifndef _SerializableObject_h_
#define _SerializableObject_h_

#include <QObject>
#include <QVariantMap>
#include "base_define.h"

class BASE_EXPORT SerializableObject:public QObject
{
	Q_OBJECT
public:
	SerializableObject(){};
	virtual ~SerializableObject(){};

	QVariantMap serialize();
	void deserialization(const QVariantMap &map);
};

#endif //_SerializableObject_h_