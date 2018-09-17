#pragma once

#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include <io.h>
#include <algorithm>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <share.h>
#include "../../core/coredefs.hpp"

#define	MC_LOG_LINE_MAX		(4096)
#define MC_LOG_HEADER_MAX	(256)
#define MC_LOG_BYTES_MAX	(1024*1024*100)


#define MC_LOG_INFO		(0x1)
#define MC_LOG_DEBUG	(0x2)
#define MC_LOG_WARN		(0x3)
#define MC_LOG_ERROR	(0x4)
#define MC_LOG_FATAL	(0x5)

#define MF_LOG_NOHDR	(0x40000000)
#define MF_LOG_SYSERR	(0x80000000)

const char* GetLastSystemErrorMessage(char* messageBuffer,size_t bufferSize);

template <typename LOCK>
class SyncronizedLocalTextLog
{
public: //defines
	typedef typename LOCK locktype;
	enum EnumModeAtFull
	{
		eNoopOnFull = 0,
		eLoopOnFull = 1,
	};



public:
	SyncronizedLocalTextLog(const char * dir, const char * file, char * header);
	virtual ~SyncronizedLocalTextLog();

public: //state
	bool writable() const;
	bool error() const;
	bool uninitialized() const;

public: //control
	size_t setMaxLines(size_t v) { int s = m_max_lines; m_max_lines = v; return s;}
	size_t setMaxSize(size_t v) { int s = m_max_bytes; m_max_bytes = v; return s;}
	EnumModeAtFull  setAtFullMode(EnumModeAtFull e) {EnumModeAtFull s = m_at_full; m_at_full = e; return s;}
	size_t setIntervalSecsToSync(size_t v) { int s = m_intervalSecsToSync; m_intervalSecsToSync = v; return s;}
	size_t setIntervalLinesToSync(size_t v) { int s = m_intervalLinesToSync; m_intervalLinesToSync = v; return s;}

public: //op
	int open();
	void close();
	void log(int flag, const char* fmt, ...);
	void logNLBegin(int flag, const char* fmt, ...);
	void logNLEnd(int flag, const char* fmt, ...);

	
protected:
	void _processLimits();
	int _checkSize() const;
	int _checkLines() const;
	int _formatString(std::string& strDest);
	void _stringReplace(std::string &strBase, const std::string& strSrc, const std::string& strDes) const;
	void _close();
	int _open();
	void _parseLogPriority(int flag);
	void _parseSysError(int flag);
	void _parseHeader(int flag);
	void _composeFilePathName(std::string& strFilePathTemp);
	bool _writable() const;
	bool _error() const;
	bool _uninitialized() const;
protected:

	time_t		m_last_time;
	size_t		m_max_bytes;
	size_t		m_max_lines;
	EnumModeAtFull			m_at_full;

	FILE*	m_fp;
	size_t		m_lines;
	size_t		m_intervalSecsToSync;
	size_t		m_intervalLinesToSync;
	time_t		m_lastSyncEpochTime;

	std::string	m_dir;
	std::string	m_name;
	std::string	m_namePattern;
	std::string m_header;

	mutable locktype	m_lock;
};

//constructor and destructors
template <typename LOCK>
SyncronizedLocalTextLog<LOCK>::SyncronizedLocalTextLog(const char* dir, const char* namePattern, char * header)
{
	m_last_time = 0;
	m_max_bytes = (size_t) 0;
	m_max_lines = (size_t) 0;
	m_at_full = eNoopOnFull;

	m_dir = dir;
	m_namePattern = namePattern;
	m_header = (header == nullptr)? "" : header;
	m_fp = nullptr;
	m_lines = 0;
	m_intervalSecsToSync = 0;
	m_intervalLinesToSync = 1;
	m_lastSyncEpochTime = 0;

}

template <typename LOCK>
SyncronizedLocalTextLog<LOCK>::~SyncronizedLocalTextLog()
{
	m_lock.lock();

	try {
		_close();
	} catch(...)
	{
	}	
	m_lock.unlock();
}
//end of constructor and destructors

//public member functions
template <typename LOCK>
int SyncronizedLocalTextLog<LOCK>::open()
{
	//std::lock_guard<locktype> guard(m_lock);	
	return _open();
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::close()
{
	//std::lock_guard<locktype> guard(m_lock);
	_close();
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::log(int flag, const char* fmt, ...)
{
	//std::lock_guard<locktype> guard(m_lock);

	if (this->_uninitialized())
	{
		int ret = _open();
		if (ret != 0)
		{
			return;
		}
	}
	else
	{
		if (this->_error())
		{
			return;
		}
	}	

	std::string strTmpName;
	_composeFilePathName(strTmpName);

	bool bReOpen = false;
	if (strTmpName.compare(m_name) == 0)
	{

	}
	else
	{		
		bReOpen = true;

	}

	if (bReOpen)
	{
		_close();
		if (0 != _open())
		{
			return;
		}
	}


	_processLimits();

	_parseLogPriority(flag);

	_parseSysError(flag);

	_parseHeader(flag);

	va_list ap;
	va_start(ap, fmt);
	vfprintf_s(m_fp, fmt, ap);
	va_end(ap);

	m_lines++;

	fputc('\n', m_fp);

	if (m_intervalLinesToSync !=0) {
		if (m_lines % m_intervalLinesToSync == 0) {
			fflush(m_fp);
		}
	}

	if (m_intervalSecsToSync !=0 ) {
		time_t t;
		time(&t);
		if (t - m_lastSyncEpochTime > m_intervalSecsToSync) {
			fflush(m_fp);
			m_lastSyncEpochTime = t; 
		}
	}	
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::logNLBegin(int flag, const char* fmt, ...)
{
	m_lock.lock();
	
	if ( this->_uninitialized())
	{
		int ret = _open();
		if (ret != 0)
		{
			return;
		}
	}
	else
	{
		if (this->_error())
		{
			return;
		}
	}

	std::string strTmpName;
	_composeFilePathName(strTmpName);

	bool bReOpen = false;
	if (strTmpName.compare(m_name) == 0)
	{
		;
	}
	else
	{		
		bReOpen = true;
	}

	if (bReOpen)
	{
		_close();
		_open();
	}

	_processLimits();

	_parseLogPriority(flag);

	_parseSysError(flag);

	_parseHeader(flag);	

	va_list ap;
	va_start(ap, fmt);
	vfprintf_s(m_fp, fmt, ap);
	va_end(ap);
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::logNLEnd(int flag, const char* fmt, ...)
{
	if (this->_uninitialized())
	{
		int ret = _open();
		if (ret != 0)
		{
			return;
		}
	}
	else
	{
		if (this->_error())
		{
			return;
		}
	}

	va_list ap;
	va_start(ap, fmt);
	vfprintf_s(m_fp, fmt, ap);
	va_end(ap);

	m_lines++;

	fputc('\n', m_fp);

	if (m_intervalLinesToSync !=0) {
		if (m_lines % m_intervalLinesToSync == 0) {
			fflush(m_fp);
		}
	}

	if (m_intervalSecsToSync !=0 ) {
		time_t t;
		time(&t);
		if (t - m_lastSyncEpochTime > m_intervalSecsToSync) {
			fflush(m_fp);
			m_lastSyncEpochTime = t; 
		}
	}

	m_lock.unlock();
	
}

//end of public member functions
template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_processLimits()
{
	if (m_at_full == eNoopOnFull) {
		return;
	}

	bool bFull = false;
	if (_checkSize() == 1) {
		bFull = true;
	}

	if (bFull == false) {
		if (_checkLines() == 1)
		{
			bFull = true;
		}		
	}

	if (! bFull) {
		return;
	}

	if (m_at_full == eLoopOnFull) {
		_close();
		fopen_s(&m_fp, m_name.c_str(),"a");		
		_close();
		_open();
		m_lines = 0;
		::SetLastError(0);
	}
}

template <typename LOCK>
int SyncronizedLocalTextLog<LOCK>::_checkSize() const
{
	if (m_max_bytes != (size_t) 0) {
		struct stat buf;
		stat(m_name.c_str(), &buf);
		if (buf.st_size >= (_off_t)m_max_bytes) {
			return 1;
		} 
	}

	return 0;
}

template <typename LOCK>
int SyncronizedLocalTextLog<LOCK>::_checkLines() const
{
	if (m_max_lines != (size_t) 0) {
		if (m_lines >= m_max_lines) {
			return 1;
		} 
	}

	return 0;
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_composeFilePathName(std::string& strFilePathTemp)
{
	strFilePathTemp = m_dir;
	strFilePathTemp += "\\";
	strFilePathTemp += m_namePattern;	
	_formatString(strFilePathTemp);
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_parseHeader(int flag)
{
	if ((flag & MF_LOG_NOHDR))
	{
		return;
	}

	std::string header = "";

	if (m_header.length() > 2)
	{
		header = m_header;
		_formatString(header);
	}

	if (header.length() > 0)
	{
		fprintf_s(m_fp, "%s\t", header.c_str());
	}
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_parseSysError(int flag)
{
	char szBuffer[1024] = {0};
	if (flag & MF_LOG_SYSERR)
	{
		pilo::get_os_error_msg(szBuffer,sizeof(szBuffer));
		if (szBuffer[0] != 0)
		{
			fprintf_s(m_fp,"(reason:%s)\t" ,szBuffer);
		}	
	}	
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_parseLogPriority(int fullflag)
{
	int flag = (fullflag & (0x000000FF));
	if (flag != 0)
	{
		if (flag == MC_LOG_INFO)
		{
			fprintf_s(m_fp, "[Info]\t");
		}
		else if (flag == MC_LOG_DEBUG)
		{
			fprintf_s(m_fp, "[DBG]\t");
		}
		else if (flag == MC_LOG_WARN)
		{
			fprintf_s(m_fp, "[Warn]\t");
		}
		else if (flag == MC_LOG_ERROR)
		{
			fprintf_s(m_fp, "[ERR]\t");
		}
		else if (flag == MC_LOG_FATAL)
		{
			fprintf_s(m_fp, "[Fatal]\t");
		}
	}
}

template <typename LOCK>
int SyncronizedLocalTextLog<LOCK>::_open()
{
	if (this->_writable())
	{
		_close();
	}
	if (this->_error())
	{
		_close();
	}
	_composeFilePathName(m_name);

	// Open a file.
	int fd= -1;
	int err = _sopen_s( &fd, m_name.c_str(), _O_RDWR | _O_APPEND | _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE   );
	if( err!=0 )
	{
		return -2;
	}

	if( (m_fp = _fdopen( fd, "a" )) == NULL )
	{
		return -3;
	}

	return 0;
}

template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_close()
{
	if (nullptr != m_fp)
	{
		fflush(m_fp);
		fclose(m_fp);
		m_fp = nullptr;
	}	
}



template <typename LOCK>
void SyncronizedLocalTextLog<LOCK>::_stringReplace(std::string &strBase, const std::string& strSrc, const std::string& strDes) const
{
	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
}

template <typename LOCK>
int SyncronizedLocalTextLog<LOCK>::_formatString(std::string& strDest)
{
	char szName[1024];
	GetModuleFileName(NULL, szName, sizeof(szName));
	char* pName = strrchr(szName, '\\');
	if (pName == nullptr)
	{
		pName = szName;
	}
	else
	{
		pName++;
	}
	char* pTmpName = 0;
	if ((pTmpName = strrchr(pName, '.')) != NULL)
	{
		*pTmpName = 0;
	}
	pTmpName = pName;

	_stringReplace(strDest, "%EXECNAME", pTmpName);

	char szTmpTimeStr[32];
	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%u", ::GetTickCount());
	_stringReplace(strDest, "%TICK", szTmpTimeStr);

	struct tm tmpTm;
	time_t tmpTime = time(0);
	localtime_s(&tmpTm, &tmpTime);

	
	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%04d", tmpTm.tm_year+1900);
	_stringReplace(strDest, "%YYYY", szTmpTimeStr);
	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%02d", tmpTm.tm_year + 1900-2000);
	_stringReplace(strDest, "%YY", szTmpTimeStr);
	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%02d", tmpTm.tm_mon + 1);
	_stringReplace(strDest, "%MM", szTmpTimeStr);

	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%02d", tmpTm.tm_mday);
	_stringReplace(strDest, "%DD", szTmpTimeStr);

	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%02d", tmpTm.tm_hour);
	_stringReplace(strDest, "%hh", szTmpTimeStr);
	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%02d", tmpTm.tm_min);
	_stringReplace(strDest, "%mm", szTmpTimeStr);
	sprintf_s(szTmpTimeStr, sizeof(szTmpTimeStr), "%02d", tmpTm.tm_sec);
	_stringReplace(strDest, "%ss", szTmpTimeStr);

	return 0;
}

template <typename LOCK>
bool SyncronizedLocalTextLog<LOCK>::_uninitialized() const
{
	if (nullptr == m_fp)
	{
		return true;
	}
	return false;
}

template <typename LOCK>
bool SyncronizedLocalTextLog<LOCK>::uninitialized() const
{
	std::lock_guard<locktype> guard(m_lock);
	return this->_uninitialized();
}

template <typename LOCK>
bool SyncronizedLocalTextLog<LOCK>::_error() const
{
	if (nullptr==m_fp)
	{
		return false;
	}
	if (::ferror(m_fp) == 0)
	{
		return false;
	}
	return true;

}

template <typename LOCK>
bool SyncronizedLocalTextLog<LOCK>::error() const
{
	std::lock_guard<locktype> guard(m_lock);
	return this->_error();
}

template <typename LOCK>
bool SyncronizedLocalTextLog<LOCK>::_writable() const
{
	if (nullptr == m_fp)
	{
		return false;
	}
	if (::ferror(m_fp) != 0)
	{
		return false;
	}
	return true;
}

template <typename LOCK>
bool SyncronizedLocalTextLog<LOCK>::writable() const
{
	std::lock_guard<locktype> guard(m_lock);
	return this->_writable();
}
