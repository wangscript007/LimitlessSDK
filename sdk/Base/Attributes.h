#ifndef _Attributes_h_
#define _Attributes_h_

#include "common.h"
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include "base_define.h"

#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4800)

template<typename classType> class AttributeTemplate;

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

//	Type type(){return m_type;}
//	std::string name(){return m_name;}
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
//	Type m_type;
//	std::string m_name;
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
	//	AttributeStringEnum():Attribute(Attribute::STRING_ENUM, ""), m_value(""){};
	AttributeEnum(const std::string name):AttributeTemplate(name){};
	AttributeEnum(const std::string name, const classType value):AttributeTemplate(name, value){};
	AttributeEnum(const std::string name, const classType  value, const std::vector<classType> values):AttributeTemplate(name, value), m_values(values){};
	virtual ~AttributeEnum(){};

	std::vector<classType> enums() const{return m_values;}
	void setEnums(std::vector<classType> enumValues){m_values=enumValues;}

private:
	std::vector<classType> m_values;
};

//template<> AttributeEnum<std::string>::AttributeEnum(const std::string name):AttributeTemplate<std::string>(name){};
//template<> AttributeEnum<std::string>::AttributeEnum(const std::string name, std::string value):AttributeTemplate(name, value){};
//template<> AttributeEnum<std::string>::AttributeEnum(const std::string name, std::string value, const std::vector<std::string> values):AttributeTemplate(name, value), m_values(values){};
template AttributeEnum<int>;
typedef AttributeEnum<int> AttributeIntEnum;

template AttributeEnum<float>;
typedef AttributeEnum<float> AttributeFloatEnum;

template AttributeEnum<std::string>;
typedef AttributeEnum<std::string> AttributeStringEnum;
typedef boost::shared_ptr<AttributeStringEnum> SharedAttributeStringEnum;

/*
class BASE_EXPORT Attribute
{
public:
	enum Type
	{
		BOOL,
		INT,
		INT64,
		FLOAT,
		DOUBLE,
		STRING,
		STRING_ENUM
	};

//	Attribute(const Type type):m_type(type), m_name(""){};
	Attribute(const Type type, const std::string name):m_type(type), m_name(name){};
//	Attribute(const std::string name, const std::string value):name(name), value(value){};
//	Attribute(const std::string name, const Strings &values, const std::string value):name(name), values(values), value(value){};
	~Attribute(){};

	Type type(){return m_type;}
	std::string name(){return m_name;}

//	virtual bool operator==(const Attribute &rhs) const{return (m_name==rhs.m_name);}
//	virtual bool operator==(const std::string &rhs) const{return (m_value==rhs);}
//	virtual bool operator==(const QString &rhs) const{return (value==rhs.toStdString());}
//	virtual bool operator!=(const Attribute &rhs) const{return (m_name!=rhs.m_name);}
//	virtual bool operator!=(const char *rhs) const{return (m_name!=rhs);}
//	virtual bool operator!=(const std::string &rhs) const{return (value!=rhs);}
//	virtual bool operator!=(const QString &rhs) const{return (value!=rhs.toStdString());}

	virtual void fromString(std::string value)=0;//{return value;}
	virtual void fromInt(int value)=0;
	virtual void fromInt64(__int64 value)=0;
	virtual void fromFloat(float value)=0;
	virtual void fromDouble(double value)=0;
	virtual void fromBool(bool value)=0;

	virtual std::string toString() const=0;//{return value;}
	virtual int toInt() const=0;
	virtual __int64 toInt64() const=0;
	virtual float toFloat() const=0;
	virtual double toDouble() const=0;
	virtual bool toBool() const=0;
	
//	virtual bool operator==(const Attribute &mediaAttribute) const=0;
	virtual bool operator==(const Attribute &mediaAttribute) const{return (toString()==mediaAttribute.toString());}
	virtual bool equal(boost::shared_ptr<Attribute> mediaAttribute) const{return (toString()==mediaAttribute->toString());}

private:
	Type m_type;
	std::string m_name;
//	std::string value;
//	Strings values;
};
typedef boost::shared_ptr<Attribute> SharedAttribute;
typedef boost::unordered_map<std::string, SharedAttribute> SharedAttributes;

class AttributeInt:public Attribute
{
public:
//	AttributeInt():Attribute(Attribute::INT, ""), m_value(0){};
	AttributeInt(const std::string name):Attribute(Attribute::INT, name), m_value(0){};
	AttributeInt(const std::string name, const int value):Attribute(Attribute::INT, name), m_value(value){};
	virtual ~AttributeInt(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toInt());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toInt());}
	AttributeInt &operator=(const int &value){m_value=value; return *this;}
	operator int() const{return m_value;}
	
	virtual void fromString(std::string value){m_value=boost::lexical_cast<int>(value);}
	virtual void fromInt(int value){m_value=value;}
	virtual void fromInt64(__int64 value){m_value=(int)value;}
	virtual void fromFloat(float value){m_value=(int)value;}
	virtual void fromDouble(double value){m_value=(int)value;}
	virtual void fromBool(bool value){m_value=(int)value;}

	virtual std::string toString() const{return boost::lexical_cast<std::string>(m_value);}
	virtual int toInt() const{return m_value;}
	virtual __int64 toInt64() const{return (__int64)m_value;}
	virtual float toFloat() const{return (float)m_value;}
	virtual double toDouble() const{return (double)m_value;}
	virtual bool toBool() const{return (m_value != 0);}

private:
	int m_value;
};

class AttributeInt64:public Attribute
{
public:
	//	AttributeInt():Attribute(Attribute::INT, ""), m_value(0){};
	AttributeInt64(const std::string name):Attribute(Attribute::INT64, name), m_value(0){};
	AttributeInt64(const std::string name, const __int64 value):Attribute(Attribute::INT64, name), m_value(value){};
	virtual ~AttributeInt64(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toInt64());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toInt64());}
	AttributeInt64 &operator=(const __int64 &value){m_value=value; return *this;}
	operator __int64() const{return m_value;}

	virtual void fromString(std::string value){m_value=boost::lexical_cast<__int64>(value);}
	virtual void fromInt(int value){m_value=(__int64)value;}
	virtual void fromInt64(__int64 value){m_value=value;}
	virtual void fromFloat(float value){m_value=(__int64)value;}
	virtual void fromDouble(double value){m_value=(__int64)value;}
	virtual void fromBool(bool value){m_value=(__int64)value;}

	virtual std::string toString() const{return boost::lexical_cast<std::string>(m_value);}
	virtual int toInt() const{return (int)m_value;}
	virtual __int64 toInt64() const{return m_value;}
	virtual float toFloat() const{return (float)m_value;}
	virtual double toDouble() const{return (double)m_value;}
	virtual bool toBool() const{return (m_value != 0);}

private:
	__int64 m_value;
};

class AttributeFloat:public Attribute
{
public:
//	AttributeFloat():Attribute(Attribute::FLOAT, ""), m_value(0.0f){};
	AttributeFloat(const std::string name):Attribute(Attribute::FLOAT, name), m_value(0.0f){};
	AttributeFloat(const std::string name, const float value):Attribute(Attribute::FLOAT, name), m_value(value){};
	virtual ~AttributeFloat(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toFloat());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toFloat());}
	AttributeFloat &operator=(const float &value){m_value=value; return *this;}
	operator float() const{return m_value;}
	
	virtual void fromString(std::string value){m_value=boost::lexical_cast<float>(value);}
	virtual void fromInt(int value){m_value=(float)value;}
	virtual void fromInt64(__int64 value){m_value=(float)value;}
	virtual void fromFloat(float value){m_value=value;}
	virtual void fromDouble(double value){m_value=(float)value;}
	virtual void fromBool(bool value){m_value=(float)value;}

	virtual std::string toString() const{return boost::lexical_cast<std::string>(m_value);}
	virtual int toInt() const{return (int)m_value;}
	virtual __int64 toInt64() const{return (__int64)m_value;}
	virtual float toFloat() const{return m_value;}
	virtual double toDouble() const{return (double)m_value;}
	virtual bool toBool() const{return (m_value != 0.0f);}

private:
	float m_value;
};

class AttributeDouble:public Attribute
{
public:
	//	AttributeFloat():Attribute(Attribute::FLOAT, ""), m_value(0.0f){};
	AttributeDouble(const std::string name):Attribute(Attribute::DOUBLE, name), m_value(0.0){};
	AttributeDouble(const std::string name, const double value):Attribute(Attribute::FLOAT, name), m_value(value){};
	virtual ~AttributeDouble(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toFloat());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toFloat());}
	AttributeDouble &operator=(const double &value){m_value=value; return *this;}
	operator double() const{return m_value;}

	virtual void fromString(std::string value){m_value=boost::lexical_cast<double>(value);}
	virtual void fromInt(int value){m_value=(double)value;}
	virtual void fromInt64(__int64 value){m_value=(double)value;}
	virtual void fromFloat(float value){m_value=(double)value;}
	virtual void fromDouble(double value){m_value=value;}
	virtual void fromBool(bool value){m_value=(double)value;}

	virtual std::string toString() const{return boost::lexical_cast<std::string>(m_value);}
	virtual int toInt() const{return (int)m_value;}
	virtual __int64 toInt64() const{return (__int64)m_value;}
	virtual float toFloat() const{return (float)m_value;}
	virtual double toDouble() const{return m_value;}
	virtual bool toBool() const{return (m_value != 0.0);}

private:
	double m_value;
};

class AttributeBool:public Attribute
{
public:
//	AttributeBool():Attribute(Attribute::BOOL, ""), m_value(false){};
	AttributeBool(const std::string name):Attribute(Attribute::BOOL, name), m_value(false){};
	AttributeBool(const std::string name, const bool value):Attribute(Attribute::BOOL, name), m_value(value){};
	virtual ~AttributeBool(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toBool());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toBool());}
	AttributeBool &operator=(const bool &value){m_value=value; return *this;}
	operator bool() const{return m_value;}
	
	virtual void fromString(std::string value){m_value=boost::lexical_cast<bool>(value);}
	virtual void fromInt(int value){m_value=(bool)value;}
	virtual void fromInt64(__int64 value){m_value=(bool)value;}
	virtual void fromFloat(float value){m_value=(bool)value;}
	virtual void fromDouble(double value){m_value=(bool)value;}
	virtual void fromBool(bool value){m_value=value;}

	virtual std::string toString() const{return boost::lexical_cast<std::string>(m_value);}
	virtual int toInt() const{return (int)m_value;}
	virtual __int64 toInt64() const{return (__int64)m_value;}
	virtual float toFloat() const{return (float)m_value;}
	virtual double toDouble() const{return (double)m_value;}
	virtual bool toBool() const{return m_value;}

private:
	bool m_value;
};

class AttributeString:public Attribute
{
public:
//	AttributeString():Attribute(Attribute::STRING, ""), m_value(""){};
	AttributeString(const std::string name):Attribute(Attribute::STRING, name), m_value(""){};
	AttributeString(const std::string name, const std::string value):Attribute(Attribute::STRING, name), m_value(value){};
	virtual ~AttributeString(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toString());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toString());}
	AttributeString &operator=(const bool &value){m_value=value; return *this;}
	operator std::string() const{return m_value;}
	
	virtual void fromString(std::string value){m_value=value;}
	virtual void fromInt(int value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromInt64(__int64 value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromFloat(float value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromDouble(double value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromBool(bool value){m_value=boost::lexical_cast<std::string>(value);}

	virtual std::string toString() const{return m_value;}
	virtual int toInt() const{return  boost::lexical_cast<int>(m_value);}
	virtual __int64 toInt64() const{return  boost::lexical_cast<__int64>(m_value);}
	virtual float toFloat() const{return boost::lexical_cast<float>(m_value);}
	virtual double toDouble() const{return boost::lexical_cast<double>(m_value);}
	virtual bool toBool() const{return boost::lexical_cast<bool>(m_value);}

private:
	std::string m_value;
};

class AttributeStringEnum:public Attribute
{
public:
//	AttributeStringEnum():Attribute(Attribute::STRING_ENUM, ""), m_value(""){};
	AttributeStringEnum(const std::string name):Attribute(Attribute::STRING_ENUM, name), m_value(""){};
	AttributeStringEnum(const std::string name, const std::string value):Attribute(Attribute::STRING_ENUM, name), m_value(value){};
	AttributeStringEnum(const std::string name, const std::string value, const Strings values):Attribute(Attribute::STRING_ENUM, name), m_value(value), m_values(values){};
	virtual ~AttributeStringEnum(){};

	virtual bool operator==(const Attribute &mediaAttribute) const{return (m_value==mediaAttribute.toString());}
	virtual bool equal(SharedAttribute mediaAttribute) const{return (m_value==mediaAttribute->toString());}
	AttributeStringEnum &operator=(const std::string &value){m_value=value; return *this;}
	operator std::string() const{return m_value;}
	Strings enums() const{return m_values;}
	void setEnums(Strings enumValues){m_values=enumValues;}
	
	virtual void fromString(std::string value){m_value=value;}
	virtual void fromInt(int value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromInt64(__int64 value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromFloat(float value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromDouble(double value){m_value=boost::lexical_cast<std::string>(value);}
	virtual void fromBool(bool value){m_value=boost::lexical_cast<std::string>(value);}

	virtual std::string toString() const{return m_value;}
	virtual int toInt() const{return  boost::lexical_cast<int>(m_value);}
	virtual __int64 toInt64() const{return  boost::lexical_cast<__int64>(m_value);}
	virtual float toFloat() const{return boost::lexical_cast<float>(m_value);}
	virtual double toDouble() const{return boost::lexical_cast<double>(m_value);}
	virtual bool toBool() const{return boost::lexical_cast<bool>(m_value);}

private:
	std::string m_value;
	Strings m_values;
};
typedef boost::shared_ptr<AttributeStringEnum> SharedAttributeStringEnum;
*/
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
//	iterator find(const char *key){const std::string constKey(key); return boost::unordered_map<std::string, Attribute>::find(constKey);}
//	const_iterator find(const char *key)const {const std::string constKey(key); return boost::unordered_map<std::string, Attribute>::find(constKey);}
private:
	SharedAttributes m_attributes;
};

void BASE_EXPORT merge(Attributes &dst, const Attributes &src);

#pragma warning(pop)
#endif //_Attributes_h_