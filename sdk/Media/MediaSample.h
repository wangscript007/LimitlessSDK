#ifndef _MediaSample_h_
#define _MediaSample_h_

//#include <QImage>
#include "Media/media_define.h"

#include "Media/MediaClock.h"
#include <boost/shared_ptr.hpp>
#include <vector>

//template<typename CLASS, typename INTERFACE>
//class AutoRegister:public INTERFACE
//{
//public:
//	AutoRegister(std::string instance):INTERFACE(instance){&s_typeName;}
//
//	static PluginObject *create(std::string instance){return new CLASS(instance);}
//private:
//	static std::string s_typeName;
//};

class MEDIA_EXPORT MediaSample
{
public:
	MediaSample():m_type(-1), m_typeName(""), m_timestamp(0), m_flags(0), m_sequenceNumber(0){m_uniqueId=m_uniqueIdCount++;};
	virtual ~MediaSample(){};

	void setAllocBin(unsigned __int64 bin){m_bin=bin;}
	unsigned __int64 allocBin(){return m_bin;}

	unsigned int type() const{return m_type;}
	std::string typeName() const{return m_typeName;}

	virtual void copyHeader(boost::shared_ptr<MediaSample> sample);

	virtual unsigned char *buffer()=0;
	virtual size_t size() const=0;

	virtual MediaTime timestamp() const{return m_timestamp;}
	virtual void setTimestamp(MediaTime timestamp){m_timestamp=timestamp;}

	unsigned int flags() const{return m_flags;}
	void setFlags(unsigned int flags){m_flags=flags;}
	void addFlags(unsigned int flags){m_flags|=flags;}
	void removeFlags(unsigned int flags){m_flags=m_flags&(~flags);}
	bool hasFlags(unsigned int flags){return (m_flags&flags);}

	unsigned int sequenceNumber() const{return m_sequenceNumber;}
	void setSequenceNumber(unsigned int sequenceNumber){m_sequenceNumber=sequenceNumber;}

	unsigned int mediaIndex() const{return m_mediaIndex;}
	void setMediaIndex(unsigned int mediaIndex){m_mediaIndex=mediaIndex;}

	unsigned int uniqueId(){return m_uniqueId;}
private:
	template<typename CLASS, typename INTERFACE> friend class AutoRegisterMediaSample;

	void setType(unsigned int type, std::string typeName){m_type=type; m_typeName=typeName;}

	unsigned __int64 m_bin;
	static unsigned int m_uniqueIdCount;
	unsigned int m_uniqueId;

	unsigned int m_type;
	std::string m_typeName;
	MediaTime m_timestamp;
	unsigned int m_flags;
	unsigned int m_sequenceNumber;
	unsigned int m_mediaIndex;
};
typedef boost::shared_ptr<MediaSample> SharedMediaSample;
typedef std::vector<SharedMediaSample> SharedMediaSamples;
typedef std::vector<MediaSample *> MediaSamples;

//class MEDIA_EXPORT MediaSample
//{
//public:
//	MediaSample():m_timestamp(-1){};
//	MediaSample(QImage image, __int64 timestamp):m_image(image), m_timestamp(timestamp){};
//	virtual ~MediaSample(){};
//
////	QImage &image(){return m_image;}
////	void setImage(QImage image){m_image=image;}
//	__int64 timestamp(){return m_timestamp;}
//	void setTimestamp(__int64 timestamp){m_timestamp=timestamp;}
//
//private:
//	QImage m_image;
//	__int64 m_timestamp;
//};
//typedef boost::shared_ptr<MediaSample> SharedMediaSample;

#endif //_MediaSample_h_