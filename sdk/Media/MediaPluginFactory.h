#ifndef _Limitless_MediaPluginFactory_h_
#define _Limitless_MediaPluginFactory_h_

#include "Media/media_define.h"

#include "Base/PluginFactory.h"
#include "Base/Log.h"
#include "Media/IMediaFilter.h"

#include <boost/format.hpp>

#pragma warning(push)
#pragma warning(disable:4251)

namespace Limitless
{

template<typename CLASS, typename INTERFACE>
class MediaAutoRegister:public INTERFACE
{
public:
	MediaAutoRegister(std::string instance, SharedMediaFilter parent):INTERFACE(instance, parent){&s_typeName;}

	static IMediaFilter *create(std::string instance, SharedMediaFilter parent){return dynamic_cast<IMediaFilter *>(new CLASS(instance, parent));}

	virtual std::string typeName(){return s_typeName;}
private:
	static std::string s_typeName;
};

class MEDIA_EXPORT FilterDefinition
{
public:
	FilterDefinition(std::string name):name(name){};
	~FilterDefinition(){};

	std::string name;
};
typedef std::vector<FilterDefinition> FilterDefinitions;

class MEDIA_EXPORT MediaPluginFactory
{
	typedef IMediaFilter *(*FactoryFunc)(std::string, SharedMediaFilter);
	typedef boost::unordered_map<std::string, FactoryFunc> FactoryFunctions;

private:
	MediaPluginFactory(){};
public:
	~MediaPluginFactory(){};

	static MediaPluginFactory &instance();
	
	void loadPlugins(std::string directory);

	std::vector<std::string> getType(std::string type);
	bool isType(std::string className, std::string type);

	template<typename CLASS> CLASS *createType(std::string type, std::string instance, SharedMediaFilter parent=SharedMediaFilter())
	{
		FactoryFunctions::iterator iter=m_objects.find(type);

		if(iter != m_objects.end())
		{
			CLASS *object=dynamic_cast<CLASS *>(iter->second(instance, parent));

			if(object != NULL)
			{
				SharedMediaFilter mediaFilter(object);

				m_filterInstances.push_back(mediaFilter);
				return object;
			}
		}
		return NULL;
	}

	SharedMediaFilter create(std::string type, std::string instance, SharedMediaFilter parent)
	{
		FactoryFunctions::iterator iter=m_objects.find(type);

		if(iter != m_objects.end())
		{
			IMediaFilter *object=iter->second(instance, parent);

			if(object != NULL)
			{
				SharedMediaFilter mediaFilter(object);

				mediaFilter->initialize(Attributes());
				m_filterInstances.push_back(mediaFilter);
				return mediaFilter;
			}
		}
		return SharedMediaFilter();
	}

//Setup devices
	std::string registerType(std::string typeName, FactoryFunc factoryFunc)
	{
		Log::write((boost::format("Registering Media Filter %s")%typeName).str());
		m_objects[typeName]=factoryFunc;

		return typeName;
	}

	void removeFilter(IMediaFilter *filter);

	FilterDefinitions registedFilters();
	SharedMediaFilters filterInstances(){return m_filterInstances;}

private:
	FactoryFunctions m_objects;

	SharedMediaFilters m_filterInstances;
};

//template <typename T> struct TypeName
//{	
//	static std::string get()
//	{
//		const char *fullName=typeid(T).name();
//		const char *name=strstr(fullName, "class");
//		return (name)? name+6 : fullName;
//	}
//};

template<typename CLASS, typename INTERFACE> std::string MediaAutoRegister<CLASS, INTERFACE>::s_typeName=\
MediaPluginFactory::instance().registerType(TypeName<CLASS>::get(), &MediaAutoRegister<CLASS, INTERFACE>::create);

}//namespace Limitless

#pragma warning(pop)

#endif //_Limitless_MediaPluginFactory_h_