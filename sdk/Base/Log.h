#ifndef _Log_h_
#define _Log_h_

#include <boost/shared_ptr.hpp>
//#include <QString>
#include <vector>
#include "base_define.h"

class BASE_EXPORT ILogCallback
{
public:
	ILogCallback(){};
	virtual ~ILogCallback(){};

	virtual void write(std::string entry)=0;
};
typedef boost::shared_ptr<ILogCallback> SharedLogCallback;

class BASE_EXPORT Log
{
public:
	Log(){};
	~Log(){};

	static void attachCallback(SharedLogCallback callback);
	static void dettachCallback(SharedLogCallback callback);

	static void write(std::string entry);
//	static void write(QString entry);

private:
	static std::vector<SharedLogCallback> m_callbacks;
};

#endif //_Log_h_