#ifndef _SerializableObject_h_
#define _SerializableObject_h_

#include <QObject>
#include <QVariantMap>
#include "base_define.h"

namespace Limitless
{

class BASE_EXPORT SerializableObject:public QObject
{
	Q_OBJECT
public:
	SerializableObject(){};
	virtual ~SerializableObject(){};

	QVariantMap serialize();
	void deserialization(const QVariantMap &map);
};

}//namespace Limitless

#endif //_SerializableObject_h_