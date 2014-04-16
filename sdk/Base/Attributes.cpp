#include "Attributes.h"
#include <boost/foreach.hpp>
#include <QString>

//operator Attribute::QString() const
//{
//	return QString::fromStdString(value);
//}
std::vector<std::string> Attributes::keys() const
{
	std::vector<std::string> localKeys;

	for(SharedAttributes::const_iterator iter=m_attributes.begin(); iter!=m_attributes.end(); ++iter)
		localKeys.push_back(iter->first);
	return localKeys;
}

void merge(Attributes &dst, const Attributes &src)
{
	BOOST_FOREACH(const Attributes::value_type &value, src)
	{
		dst[value.first]=value.second;
	}
}

SharedAttribute Attributes::find(const char *key) const
{
	const std::string stringKey(key); 
	
	return find(stringKey);
}

SharedAttribute Attributes::find(const std::string &key) const
{
	SharedAttributes::const_iterator iter=m_attributes.find(key);

	if(iter != m_attributes.end())
		return iter->second;
	return SharedAttribute();
}

//template<> AttributeTemplate<bool>::AttributeTemplate(const std::string name):Attribute(Attribute::BOOL, name){};
//template<> AttributeTemplate<bool>::AttributeTemplate(const std::string name, bool value):Attribute(Attribute::BOOL, name), m_value(value){};
//
//template<> AttributeTemplate<int>::AttributeTemplate(const std::string name):Attribute(Attribute::INT, name){};
//template<> AttributeTemplate<int>::AttributeTemplate(const std::string name, int value):Attribute(Attribute::INT, name), m_value(value){};
//
//template<> AttributeTemplate<__int64>::AttributeTemplate(const std::string name):Attribute(Attribute::INT64, name){};
//template<> AttributeTemplate<__int64>::AttributeTemplate(const std::string name, __int64 value):Attribute(Attribute::INT64, name), m_value(value){};
//
//template<> AttributeTemplate<float>::AttributeTemplate(const std::string name):Attribute(Attribute::FLOAT, name){};
//template<> AttributeTemplate<float>::AttributeTemplate(const std::string name, float value):Attribute(Attribute::FLOAT, name), m_value(value){};
//
//template<> AttributeTemplate<double>::AttributeTemplate(const std::string name):Attribute(Attribute::DOUBLE, name){};
//template<> AttributeTemplate<double>::AttributeTemplate(const std::string name, double value):Attribute(Attribute::DOUBLE, name), m_value(value){};
//
//template<> AttributeTemplate<std::string>::AttributeTemplate(const std::string name):Attribute(Attribute::STRING, name){};
//template<> AttributeTemplate<std::string>::AttributeTemplate(const std::string name, std::string value):Attribute(Attribute::STRING, name), m_value(value){};
