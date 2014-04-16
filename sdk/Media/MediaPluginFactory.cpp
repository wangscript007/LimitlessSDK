#include "Media/MediaPluginFactory.h"
#include <QDir>
#include <QStringList>
#include <QString>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif //WIN32

MediaPluginFactory &MediaPluginFactory::instance()
{
	static MediaPluginFactory s_instance;

	return s_instance;
}

void MediaPluginFactory::loadPlugins(std::string path)
{
	QDir directory(QString::fromStdString(path));
	
	QStringList  directories=directory.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

	foreach(const QString &subDirectory, directories)
	{
		QString qPath=QString("%1%2%3").arg(QString::fromStdString(path)).arg(subDirectory).arg(QDir::separator());
		//load subfolders
		loadPlugins(qPath.toStdString());
	}

	QStringList filters;
	
	filters<<"*.dll";
	directory.setNameFilters(filters);

	QStringList files=directory.entryList();

#ifdef WIN32
	SetDllDirectoryA(path.c_str());
#endif //WIN32
	foreach(const QString &file, files)
	{
		QString fullPath=QString("%1%2").arg(QString::fromStdString(path)).arg(file);

#ifdef WIN32
		HINSTANCE instance=LoadLibrary(fullPath.toStdString().c_str()); //Plugins should autoregister

		if(instance == NULL)
		{
			LPVOID lpMsgBuf;
			
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);

			OutputDebugString((LPCTSTR)lpMsgBuf);
			LocalFree( lpMsgBuf );
		}

#endif //WIN32
	}
#ifdef WIN32
	SetDllDirectoryA(NULL);
#endif //WIN32
}

std::vector<std::string> MediaPluginFactory::getType(std::string type)
{
	std::vector<std::string> classes;

	for(FactoryFunctions::iterator iter=m_objects.begin(); iter!=m_objects.end(); ++iter)
	{
		PluginObject *object=iter->second("getType", SharedMediaFilter());

		if(object->isType(type))
			classes.push_back(iter->first);
		delete object;
	}
	return classes;
}

bool MediaPluginFactory::isType(std::string className, std::string type)
{
	FactoryFunctions::iterator iter=m_objects.find(className);

	if(iter != m_objects.end())
	{
		PluginObject *object=iter->second("isType", SharedMediaFilter());
		bool returnValue=object->isType(type);
		
		delete object;
		return returnValue;
	}
	return false;
}

void MediaPluginFactory::removeFilter(IMediaFilter *filter)
{
	for(SharedMediaFilters::iterator iter=m_filterInstances.begin(); iter!=m_filterInstances.end(); ++iter)
	{
		if(iter->get() == filter)
		{
			m_filterInstances.erase(iter);
			break;
		}
	}
}

FilterDefinitions MediaPluginFactory::registedFilters()
{
	FilterDefinitions filterDefinitions;
	FactoryFunctions::iterator iter;

	for(iter=m_objects.begin(); iter!=m_objects.end(); ++iter)
		filterDefinitions.push_back(FilterDefinition(iter->first));
	return filterDefinitions;
}