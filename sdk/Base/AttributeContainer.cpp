#include "AttributeContainer.h"

//bool AttributeContainer::hasAttribute(const std::string &name)
//{
//	return m_attributes.exists(name);
//}
std::vector<std::string> AttributeContainer::keys() const
{
	return m_attributes.keys();
}

bool AttributeContainer::exists(const std::string &name) const
{	return m_attributes.exists(name);}

SharedAttribute AttributeContainer::attribute(const std::string &name) const
{
	SharedAttribute attribute;

	attribute=m_attributes.find(name);
	if(attribute != SharedAttribute())
		return m_attributes.at(name);
	return SharedAttribute();
}

//void AttributeContainer::setAttribute(std::string name, std::string value)
//{
//	SharedAttribute attribute;
//
//	attribute=m_attributes.find(name);
//	if(attribute != SharedAttribute())
//	{
//		attribute->fromString(value);
//		onAttributeChanged(name, attribute);
//	}
//}
//
//void AttributeContainer::setAttribute(std::string name, std::string value, const Strings &values)
//{
//	SharedAttribute attribute;
//
//	attribute=m_attributes.find(name);
//	if(attribute != SharedAttribute())
//	{
//		SharedAttributeStringEnum stringEnum=boost::dynamic_pointer_cast<AttributeStringEnum>(attribute);
//
//		if(stringEnum != SharedAttributeStringEnum())
//		{
//			stringEnum->fromString(value);
//			stringEnum->setEnums(values);
//			onAttributeChanged(name, attribute);
//		}
//	}
//}

void AttributeContainer::addAttribute(SharedAttribute attribute)
{
	Attribute *newAttribute=NULL;
	switch(attribute->type())
	{
	case Attribute::BOOL:
		newAttribute=new AttributeBool(*(AttributeBool *)attribute.get());
		break;
	case Attribute::INT:
		newAttribute=new AttributeInt(*(AttributeInt *)attribute.get());
		break;
	case Attribute::INT64:
		newAttribute=new AttributeInt64(*(AttributeInt64 *)attribute.get());
		break;
	case Attribute::FLOAT:
		newAttribute=new AttributeFloat(*(AttributeFloat *)attribute.get());
		break;
	case Attribute::DOUBLE:
		newAttribute=new AttributeDouble(*(AttributeDouble *)attribute.get());
		break;
	case Attribute::STRING:
		newAttribute=new AttributeString(*(AttributeString *)attribute.get());
		break;
	case Attribute::STRING_ENUM:
		newAttribute=new AttributeStringEnum(*(AttributeStringEnum *)attribute.get());
		break;
	}

	if(newAttribute != NULL)
		m_attributes[attribute->name()].reset(newAttribute);
}

//void AttributeContainer::addAttribute(std::string name, bool value)
//{
//	m_attributes[name].reset(new AttributeBool(name, value));
//}
//
//void AttributeContainer::addAttribute(std::string name, int value)
//{
//	m_attributes[name].reset(new AttributeInt(name, value));
//}
//
//void AttributeContainer::addAttribute(std::string name, __int64 value)
//{
//	m_attributes[name].reset(new AttributeInt64(name, value));
//}
//
//void AttributeContainer::addAttribute(std::string name, float value)
//{
//	m_attributes[name].reset(new AttributeFloat(name, value));
//}
//
//void AttributeContainer::addAttribute(std::string name, double value)
//{
//	m_attributes[name].reset(new AttributeDouble(name, value));
//}
//
//void AttributeContainer::addAttribute(std::string name, char *value)
//{
//	m_attributes[name].reset(new AttributeString(name, value));
//}
//
//void AttributeContainer::addAttribute(std::string name, std::string value)
//{
//	m_attributes[name].reset(new AttributeString(name, value));
//}

//void AttributeContainer::addAttribute(std::string name, std::string value, const Strings &values)
//{
//	m_attributes[name].reset(new AttributeStringEnum(name, value, values));
//}

