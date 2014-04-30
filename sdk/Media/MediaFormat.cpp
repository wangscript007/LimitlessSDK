#include "MediaFormat.h"
#include "Base/json.h"
#include <QtCore/QStringList>
#include <boost/foreach.hpp>

using namespace Limitless;

MediaAttribute::MediaAttribute(std::string name, std::string value, Flags flags):
AttributeString(name, value),
m_flags(flags)
{
	//	m_attribute=attribute;
}

//MediaAttribute::MediaAttribute(SharedAttributeString attribute, Flags flags):
//AttributeString(*attribute)
//{
////	m_attribute=attribute;
//	m_flags=flags;
//}

void MediaFormat::addMediaAttribute(SharedMediaAttribute mediaAttribute)
{
//	m_mediaAttributes[mediaAttribute->name()]=mediaAttribute;
	*attribute(mediaAttribute->name())=*mediaAttribute;
}

void MediaFormat::addMediaAttributes(std::string jsonAttributes)
{
	QVariant jsonVariant=QtJson::parse(QString::fromStdString(jsonAttributes));

	if(jsonVariant.type() != QVariant::Map)
		return;

	addMediaAttributes(&jsonVariant);
}

void MediaFormat::addMediaAttributes(QVariant *jsonVariant)
{
	QVariantMap attributes=jsonVariant->toMap();
	QStringList keys=attributes.keys();

	foreach(const QString &key, keys)
	{
		QVariant &value=attributes[key];
		Attribute *attribute=NULL;

		switch(value.type())
		{
		case QVariant::Bool:
			attribute=new AttributeBool(key.toStdString(), value.toBool());
			break;
		case QVariant::Int:
			attribute=new AttributeInt(key.toStdString(), value.toInt());
			break;
		case QVariant::Double:
			attribute=new AttributeFloat(key.toStdString(), value.toFloat());
			break;
		case QVariant::String:
			attribute=new AttributeString(key.toStdString(), value.toString().toStdString());
			break;
		case QVariant::StringList:
			{
				QStringList stringList=value.toStringList();
				Strings values;

				foreach(const QString &item, stringList)
					values.push_back(item.toStdString());

				if(values.size() > 0)
				{
					attribute=new AttributeStringEnum(key.toStdString(), values[0], values);
				}
			}
			break;
		case QVariant::Map:
			break;
		}

		if(attribute != NULL)
		{
//			SharedAttribute sharedAttribute(attribute);
//			SharedMediaAttribute mediaAttribute(new MediaAttribute(sharedAttribute));
			SharedMediaAttribute mediaAttribute(new MediaAttribute(attribute->name(), attribute->toString()));
			
			addAttribute(mediaAttribute);
			delete attribute;
//			m_mediaAttributes[mediaAttribute->name()]=mediaAttribute;
		}
	}
}

//bool MediaFormat::exists(std::string name) const
//{
//	return (m_mediaAttributes.find(name) != m_mediaAttributes.end());
//}
//
//SharedMediaAttribute MediaFormat::attribute(std::string name) const
//{
//	return m_mediaAttributes.at(name);
//}
//
//Strings MediaFormat::attributeKeys() const
//{	
//	Strings keys;
//
//	for(SharedMediaAttributes::const_iterator iter=m_mediaAttributes.begin(); iter != m_mediaAttributes.end(); ++iter)
//		keys.push_back(iter->first);
//	return keys;
//}

bool MediaFormat::match(SharedMediaFormat mediaFormat)
{
	Strings keys=mediaFormat->keys();
	bool intersectMatched=false;

	BOOST_FOREACH(std::string &key, keys)
	{
//		if(!exists(key))
//			return false;

//		if(*(m_mediaAttributes.at(key).get()) == *(mediaFormat->attribute(key).get()))
		if(!exists(key))
			continue;

		if(*attribute(key) == *mediaFormat->attribute(key))
			intersectMatched=true;
	}
	return intersectMatched;
}

bool MediaFormat::operator==(const MediaFormat &mediaFormat) const
{
	Strings keys=mediaFormat.keys();

	BOOST_FOREACH(std::string &key, keys)
	{
		if(!exists(key))
			return false;

//		if(*m_mediaAttributes.at(key) == *mediaFormat.attribute(key))
		if(*attribute(key) == *mediaFormat.attribute(key))
			return true;
	}
	return false;
}