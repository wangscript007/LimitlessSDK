#include "Log.h"
#include <boost/foreach.hpp>

std::vector<SharedLogCallback> Log::m_callbacks;

void Log::attachCallback(SharedLogCallback callback)
{	m_callbacks.push_back(callback);}

void Log::dettachCallback(SharedLogCallback callback)
{
	std::vector<SharedLogCallback>::iterator iter=std::find(m_callbacks.begin(), m_callbacks.end(), callback);

	if(iter != m_callbacks.end())
		m_callbacks.erase(iter);
}

void Log::write(std::string entry)
{
	BOOST_FOREACH(SharedLogCallback &callback, m_callbacks)
	{
		callback->write(entry);
	}
}

//void Log::write(QString entry)
//{
//	BOOST_FOREACH(SharedLogCallback &callback, m_callbacks)
//	{
//		callback->write(entry.toStdString());
//	}
//}