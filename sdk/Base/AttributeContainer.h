#ifndef _AttributeContainer_h_
#define _AttributeContainer_h_

#include "Base/base_define.h"
#include "Base/Attributes.h"

class BASE_EXPORT AttributeContainer
{
public:
	AttributeContainer(){}
	virtual ~AttributeContainer(){}

//	bool hasAttribute(const std::string &name);
	std::vector<std::string> keys() const;
	bool exists(const std::string &name) const;
	SharedAttribute attribute(const std::string &name) const;
	
//	void setAttribute(std::string name, std::string value);
//	void setAttribute(std::string name, std::string value, const Strings &values);
	template<typename T> void setAttribute(std::string name, T value)
	{
		SharedAttribute attribute;

		attribute=m_attributes.find(name);
		if(attribute != SharedAttribute())
		{
			attribute->from(value);
			onAttributeChanged(name, attribute);
		}
	}
	template<typename T> void setAttribute(std::string name, T value, const std::vector<T> &values);

	Attributes &attributes(){return m_attributes;}

	void addAttribute(SharedAttribute attribute);
	template <typename T> void addAttribute(std::string name, T value){m_attributes[name].reset(new AttributeTemplate<T>(name, value));}
	template<> void addAttribute<const char *>(std::string name, const char * value){addAttribute(name, std::string(value));}
	template<> void addAttribute<char *>(std::string name, char * value){addAttribute(name, std::string(value));}
//	void addAttribute(std::string name, bool value);
//	void addAttribute(std::string name, int value);
//	void addAttribute(std::string name, __int64 value);
//	void addAttribute(std::string name, float value);
//	void addAttribute(std::string name, double value);
//	void addAttribute(std::string name, char *value);
//	void addAttribute(std::string name, std::string value);
//	void addAttribute(std::string name, std::string value, const Strings &values);
	template <typename T> void addAttribute(std::string name, T value, const std::vector<T> &values){m_attributes[name].reset(new AttributeEnum<T>(name, value, values));}

protected:
	virtual void onAttributeChanged(std::string name, SharedAttribute attribute){};

private:
	Attributes m_attributes;
};

#endif //_AttributeContainer_h_