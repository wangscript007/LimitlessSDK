#ifndef _MediaSampleFactory_h_
#define _MediaSampleFactory_h_

#include "Media/media_define.h"
#include "Base/PluginFactory.h"
#include "Media/MediaSample.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Limitless
{

template<typename CLASS, typename INTERFACE>
class AutoRegisterMediaSample:public INTERFACE
{
public:
	AutoRegisterMediaSample():INTERFACE(){&s_typeName;}

	static MediaSample *create(unsigned int type){CLASS *newClass=new CLASS(); newClass->setType(type, s_typeName); return newClass;}
private:
	static std::string s_typeName;
};

class MEDIA_EXPORT MediaSampleFactory
{
	typedef MediaSample *(*FactoryFunc)(unsigned int type);
	class MediaSampleNode
	{
	public:
		MediaSampleNode(unsigned int type, std::string typeName, FactoryFunc factoryFunction):
			m_type(type), m_typeName(typeName), m_factoryFunction(factoryFunction){};

		size_t type(){return m_type;}
		std::string typeName(){return m_typeName;}
		MediaSample *factoryFunction(){return m_factoryFunction(m_type);}

	private:
		unsigned int m_type;
		std::string m_typeName;
		FactoryFunc m_factoryFunction;
	};
	typedef std::vector<MediaSampleNode> MediaSampleNodes;

public:
	MediaSampleFactory(){}
	~MediaSampleFactory(){}

public:
	static size_t getTypeId(std::string typeName);
//	static std::vector<std::string> getType(std::string type);
	static SharedMediaSample createType(std::string typeName);
	static SharedMediaSample createType(unsigned int type);

//Setup devices
	static std::string registerType(std::string typeName, FactoryFunc factoryFunc)
	{
		s_mediaSampleNodes.push_back(MediaSampleNode(s_sampleTypeIndex, typeName, factoryFunc));
		s_sampleTypeIndex++;
		return typeName;
	}

private:
	static MediaSampleNodes s_mediaSampleNodes;
	static unsigned int s_sampleTypeIndex;
};

template<typename CLASS, typename INTERFACE> std::string AutoRegisterMediaSample<CLASS, INTERFACE>::s_typeName=\
MediaSampleFactory::registerType(TypeName<CLASS>::get(), &AutoRegisterMediaSample<CLASS, INTERFACE>::create);

}//namespace Limitless

#pragma warning(pop)

#endif //_MediaSampleFactory_h_