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

enum FilterType
{
	Unknown,
	Source,
	Filter,
	Sink
};

namespace traits
{
	template <typename T> struct type
	{	
		static FilterType get()
		{return Unknown;}
	};

	template <typename T> struct category
	{	
		static std::string get()
		{return "unknown";}
	};
}//namespace traits

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
	std::string name;
	FilterType type;
	std::string category;
};
typedef std::vector<FilterDefinition> FilterDefinitions;

class MEDIA_EXPORT MediaPluginFactory
{
	typedef IMediaFilter *(*FactoryFunc)(std::string, SharedMediaFilter);

	struct FilterDetails:public FilterDefinition
	{
		FactoryFunc factoryFunction;
	};
	typedef boost::unordered_map<std::string, FilterDetails> FilterDetailsMap;

private:
	MediaPluginFactory(){};
public:
	~MediaPluginFactory(){};

	static void loadPlugins(std::string directory);

	static std::vector<std::string> getType(std::string type);
	static bool isType(std::string className, std::string type);

	template<typename CLASS> static CLASS *createType(std::string type, std::string instance, SharedMediaFilter parent=SharedMediaFilter())
	{
		FilterDetailsMap::iterator iter=s_objects.find(type);

		if(iter != s_objects.end())
		{
			CLASS *object=dynamic_cast<CLASS *>(iter->second.factoryFunction(instance, parent));

			if(object != NULL)
			{
				SharedMediaFilter mediaFilter(object);

				s_filterInstances.push_back(mediaFilter);
				return object;
			}
		}
		return NULL;
	}

	static SharedMediaFilter create(std::string type, std::string instance, SharedMediaFilter parent)
	{
		FilterDetailsMap::iterator iter=s_objects.find(type);

		if(iter != s_objects.end())
		{
			IMediaFilter *object=iter->second.factoryFunction(instance, parent);

			if(object != NULL)
			{
				SharedMediaFilter mediaFilter(object);

				mediaFilter->initialize(Attributes());
				s_filterInstances.push_back(mediaFilter);
				return mediaFilter;
			}
		}
		return SharedMediaFilter();
	}

//Setup devices
	template<typename CLASS> static std::string registerType()
	{
		FilterDetails details;

		details.name=TypeName<CLASS>::get();
		details.type=traits::type<CLASS>::get();
		details.category=traits::category<CLASS>::get();
		details.factoryFunction=&MediaAutoRegister<CLASS, INTERFACE>::create;

		s_objects[details.name]=details;
		return details.name;

//		Log::write((boost::format("Registering Media Filter %s")%typeName).str());
//		s_objects[typeName]=factoryFunc;
//
//		return typeName;
	}

	static void removeFilter(IMediaFilter *filter);

	static FilterDefinitions registedFilters();
	static FilterDefinitions registedFiltersByType(FilterType type);
	static FilterDefinitions registedFiltersByCategory(std::string category);
	static SharedMediaFilters filterInstances(){return s_filterInstances;}

private:
//	static FactoryFunctions s_objects;
	static FilterDetailsMap s_objects;
	static SharedMediaFilters s_filterInstances;
};

template<typename CLASS, typename INTERFACE> std::string MediaAutoRegister<CLASS, INTERFACE>::s_typeName=\
MediaPluginFactory::registerType<CLASS>();

}//namespace Limitless

#pragma warning(pop)

#endif //_Limitless_MediaPluginFactory_h_