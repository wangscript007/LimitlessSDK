#ifndef _Attributes_h_
#define _Attributes_h_

#include "common.h"
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "base_define.h"

#pragma warning(push)
#pragma warning(disable:4800)
#include <boost/lexical_cast.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4251)

template<typename classType> class AttributeTemplate;

namespace Limitless
{

class BASE_EXPORT Attribute
{
public:
	enum Type
	{
		UNKNOWN,
		BOOL,
		INT,
		INT64,
		FLOAT,
		DOUBLE,
		STRING,
		STRING_ENUM
	};

	Attribute(Type type, std::string name):m_type(type), m_name(name){}
	virtual ~Attribute(){};

	Type type(){return m_type;}
	std::string name(){return m_name;}

	template <typename T> void from(T value){AttributeTemplate<T> *d=dynamic_cast<AttributeTemplate<T> *>(this);d?d->from(value):T();}
	template <typename T> T to() const {const AttributeTemplate<T> *d=dynamic_cast<const AttributeTemplate<T> *>(this);return d?d->to<T>(value):T();}

	virtual void fromString(std::string value)=0;
	virtual void fromInt(int value)=0;
	virtual void fromInt64(__int64 value)=0;
	virtual void fromFloat(float value)=0;
	virtual void fromDouble(double value)=0;
	virtual void fromBool(bool value)=0;

	virtual std::string toString() const=0;
	virtual int toInt() const=0;
	virtual __int64 toInt64() const=0;
	virtual float toFloat() const=0;
	virtual double toDouble() const=0;
	virtual bool toBool() const=0;

	virtual bool operator==(const Attribute &attribute) const{return (toString()==attribute.toString());}
	virtual bool equal(boost::shared_ptr<Attribute> attribute) const{return (toString()==attribute->toString());}

private:
	Type m_type;
	std::string m_name;
};
typedef boost::shared_ptr<Attribute> SharedAttribute;
typedef boost::unordered_map<std::string, SharedAttribute> SharedAttributes;

template<typename classType>
class AttributeTemplate:public Attribute
{
public:
	AttributeTemplate(const std::string name):Attribute(Attribute::UNKNOWN, name){};
	AttributeTemplate(const std::string name, classType value):Attribute(Attribute::UNKNOWN, name), m_value(value){};
	virtual ~AttributeTemplate(){};

	classType value(){return m_value;}

	template<typename T> void from(const T &value){m_value=boost::lexical_cast<classType>(value);}
	template<typename T> void to() const{boost::lexical_cast<T>(m_value);}

	virtual void fromString(std::string value){m_value=boost::lexical_cast<classType>(value);}
	virtual void fromInt(int value){m_value=boost::lexical_cast<classType>(value);}
	virtual void fromInt64(__int64 value){m_value=boost::lexical_cast<classType>(value);}
	virtual void fromFloat(float value){m_value=boost::lexical_cast<classType>(value);}
	virtual void fromDouble(double value){m_value=boost::lexical_cast<classType>(value);}
	virtual void fromBool(bool value){m_value=boost::lexical_cast<classType>(value);}

	virtual std::string toString() const{return boost::lexical_cast<std::string>(m_value);}
	virtual int toInt() const{return boost::lexical_cast<int>(m_value);}
	virtual __int64 toInt64() const{return boost::lexical_cast<__int64>(m_value);}
	virtual float toFloat() const{return boost::lexical_cast<float>(m_value);}
	virtual double toDouble() const{return boost::lexical_cast<double>(m_value);}
	virtual bool toBool() const{return boost::lexical_cast<bool>(m_value);}

private:
	classType m_value;
};

template<> BASE_EXPORT AttributeTemplate<bool>::AttributeTemplate(const std::string name):Attribute(Attribute::BOOL, name), m_value(false){};
template<> BASE_EXPORT AttributeTemplate<bool>::AttributeTemplate(const std::string name, bool value):Attribute(Attribute::BOOL, name), m_value(value){};
typedef AttributeTemplate<bool> AttributeBool;

template<> BASE_EXPORT AttributeTemplate<int>::AttributeTemplate(const std::string name):Attribute(Attribute::INT, name), m_value(0){};
template<> BASE_EXPORT AttributeTemplate<int>::AttributeTemplate(const std::string name, int value):Attribute(Attribute::INT, name), m_value(value){};
typedef AttributeTemplate<int> AttributeInt;

template<> BASE_EXPORT AttributeTemplate<__int64>::AttributeTemplate(const std::string name):Attribute(Attribute::INT64, name), m_value(0){};
template<> BASE_EXPORT AttributeTemplate<__int64>::AttributeTemplate(const std::string name, __int64 value):Attribute(Attribute::INT64, name), m_value(value){};
typedef AttributeTemplate<__int64> AttributeInt64;

template<> BASE_EXPORT AttributeTemplate<float>::AttributeTemplate(const std::string name):Attribute(Attribute::FLOAT, name), m_value(0.0f){};
template<> BASE_EXPORT AttributeTemplate<float>::AttributeTemplate(const std::string name, float value):Attribute(Attribute::FLOAT, name), m_value(value){};
typedef AttributeTemplate<float> AttributeFloat;

template<> BASE_EXPORT AttributeTemplate<double>::AttributeTemplate(const std::string name):Attribute(Attribute::DOUBLE, name), m_value(0.0){};
template<> BASE_EXPORT AttributeTemplate<double>::AttributeTemplate(const std::string name, double value):Attribute(Attribute::DOUBLE, name), m_value(value){};
typedef AttributeTemplate<double> AttributeDouble;

template<> BASE_EXPORT AttributeTemplate<std::string>::AttributeTemplate(const std::string name):Attribute(Attribute::STRING, name), m_value(""){};
template<> BASE_EXPORT AttributeTemplate<std::string>::AttributeTemplate(const std::string name, std::string value):Attribute(Attribute::STRING, name), m_value(value){};
typedef AttributeTemplate<std::string> AttributeString;

template<typename classType>
class BASE_EXPORT AttributeEnum:public AttributeTemplate<classType>
{
public:
	AttributeEnum(const std::string name):AttributeTemplate(name){};
	AttributeEnum(const std::string name, const classType value):AttributeTemplate(name, value){};
	AttributeEnum(const std::string name, const classType  value, const std::vector<classType> values):AttributeTemplate(name, value), m_values(values){};
	virtual ~AttributeEnum(){};

	std::vector<classType> enums() const{return m_values;}
	void setEnums(std::vector<classType> enumValues){m_values=enumValues;}

private:
	std::vector<classType> m_values;
};

template AttributeEnum<int>;
typedef AttributeEnum<int> AttributeIntEnum;

template AttributeEnum<float>;
typedef AttributeEnum<float> AttributeFloatEnum;

template AttributeEnum<std::string>;
typedef AttributeEnum<std::string> AttributeStringEnum;
typedef boost::shared_ptr<AttributeStringEnum> SharedAttributeStringEnum;

class BASE_EXPORT Attributes
{
public:
	void addAttribute(SharedAttribute attribute){m_attributes[attribute->name()]=attribute;}

	typedef SharedAttributes::value_type value_type;
	typedef SharedAttributes::iterator iterator;
	typedef SharedAttributes::const_iterator const_iterator;

	size_t size() const{return m_attributes.size();}
	std::vector<std::string> keys() const;
	bool exists(std::string name) const{return (m_attributes.find(name) != m_attributes.end());}
	SharedAttribute find(const char *key) const;
	SharedAttribute find(const std::string &key) const;
	SharedAttribute &operator[](const char *key){const std::string constKey(key); return m_attributes[constKey];}
	SharedAttribute &operator[](const std::string &key){return m_attributes[key];}

	SharedAttribute const&at(const std::string key) const{return m_attributes.at(key);}

	iterator begin(){return m_attributes.begin();}
    const_iterator begin() const{return m_attributes.begin();}
    iterator end(){return m_attributes.end();}
    const_iterator end() const{return m_attributes.end();}

	iterator erase(const_iterator position){return m_attributes.erase(position);}
private:
	SharedAttributes m_attributes;
};

#pragma warning(pop)

void BASE_EXPORT merge(Attributes &dst, const Attributes &src);

}//namespace Limitless

#endif //_Attributes_h_