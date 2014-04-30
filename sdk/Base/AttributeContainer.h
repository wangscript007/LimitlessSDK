#ifndef _AttributeContainer_h_
#define _AttributeContainer_h_

#include "Base/base_define.h"
#include "Base/Attributes.h"

namespace Limitless
{

class BASE_EXPORT AttributeContainer
{
public:
	AttributeContainer(){}
	virtual ~AttributeContainer(){}

	std::vector<std::string> keys() const;
	bool exists(const std::string &name) const;
	SharedAttribute attribute(const std::string &name) const;
	
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
	template <typename T> void addAttribute(std::string name, T value, const std::vector<T> &values){m_attributes[name].reset(new AttributeEnum<T>(name, value, values));}

protected:
	virtual void onAttributeChanged(std::string name, SharedAttribute attribute){};

private:
	Attributes m_attributes;
};

}//namespace Limitless

#endif //_AttributeContainer_h_