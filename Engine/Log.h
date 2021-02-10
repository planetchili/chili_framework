#pragma once
#include<string>
#include<filesystem>
#include<memory>

#if _DEBUG
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace penguin2D
{
	/// this class is used to construct a spdlogger with a multi threaded sink to stdout
	/// 
	/// the constructor of this class does all the heavy lifting and is responsible for the actual construction of the loggers.. since construction
	/// of an object is always single threaded.. it was reasoned that this is a good time as ever to go ahead and create the spdlog' logger to be used..
	class consoleLog
	{
	public:
		///a constructor respnsible for constructing  the spdlog::logger
		/// 
		/// @params logName this is the name of the logger being created by this constructor. this name is useful as it is displayed on every log message published by the logger and can be used for things like classification
		/// 
		/// @note this constructor also initializes spdlog if it is not already initialized.. 
		/// @attention there is no fear of a race condition while initializing spdlog and the synchronization to initialize the library only once is handled internally by the constructor in question
		/// 
		/// this constructor uses the factory function provided by spdlog to create its logger.. the exact details of construction are hence dependent on the library.
		/// however only one sink to stdout is guarenteed when the object is initialized by this constructor.
		consoleLog(const std::string& logName);

	public:
		/// a reference to the logger constructed by spdlog 
		/// 
		/// a reference to the spdlog logger is held so as to avoid the lookup call which has to be made to spdlog library every time this instance is used to log something..
		std::shared_ptr<spdlog::logger> m_log_handle = nullptr;
	};

	/// this class is used to construct a spdlogger with multi threaded sinks that write to different files.
	/// 
	/// the constructor of this class does all the heavy lifting and is responsible for the actual construction of the loggers.. since construction
	/// of an object is always single threaded.. it was reasoned that this is a good time as ever to go ahead and create the spdlog' logger to be used..
	/// 
	/// @attention every call to write using this object results in the message being written in atleast two files. (only two when the doccumentation was written.) 
	/// @note one file is the record all messages written by this instance.. the file is saved in a folder with the logName in the log directory..
	/// @note the second file only records messages of that particular level.. ex- all warning level messages are recorded in a seperate file so that it is easy to look at only those if someone so wishes..
	class fileLog
	{
	public:
		///a constructor respnsible for constructing all the spdlog::logger's associated with this class.
		/// 
		/// @params logName this is the name of the logger being created by this constructor. this name is useful as it is displayed on every log message published by the logger and can be used for things like classification
		/// 
		/// @note this constructor also initializes spdlog if it is not already initialized.. 
		/// @attention there is no fear of a race condition while initializing spdlog and the synchronization to initialize the library only once is handled internally by the constructor in question
		/// 
		/// @attention this constructor does not use the factory functions provided by spdlog and as such at the very least are not registered in the global lookup provided by spdlog to the best of my knowledge.
		/// @attention every logger contained inside this class is guarenteed to be atleast associated to two sinks if this method is used to initialize the object..
		fileLog(const std::string& logName);
	
	public:

		/// a reference to the logger constructed by spdlog for this object
		/// a reference to the spdlog logger is held as they are not registered to the lookup provided by spdlog and one can avoid the call to lookup everytime this object is used..
		/// @attention the trace_handle suggests that this logger also holds a sink where only trace messages are recorded.
		/// @note using this logger writes to the record all sink associated with this object..
		std::shared_ptr<spdlog::logger> m_log_trace_handle		= nullptr;

		/// a reference to the logger constructed by spdlog for this object
		/// a reference to the spdlog logger is held as they are not registered to the lookup provided by spdlog and one can avoid the call to lookup everytime this object is used..
		/// @attention the info_handle suggests that this logger also holds a sink where only info messages are recorded.
		/// @note using this logger writes to the record all sink associated with this object..
		std::shared_ptr<spdlog::logger> m_log_info_handle		= nullptr;

		/// a reference to the logger constructed by spdlog for this object
		/// a reference to the spdlog logger is held as they are not registered to the lookup provided by spdlog and one can avoid the call to lookup everytime this object is used..
		/// @attention the warn_handle suggests that this logger also holds a sink where only warn messages are recorded.
		/// @note using this logger writes to the record all sink associated with this object..
		std::shared_ptr<spdlog::logger> m_log_warn_handle		= nullptr;

		/// a reference to the logger constructed by spdlog for this object
		/// a reference to the spdlog logger is held as they are not registered to the lookup provided by spdlog and one can avoid the call to lookup everytime this object is used..
		/// @attention the error_handle suggests that this logger also holds a sink where only error messages are recorded.
		/// @note using this logger writes to the record all sink associated with this object..
		std::shared_ptr<spdlog::logger> m_log_error_handle		= nullptr;

		/// a reference to the logger constructed by spdlog for this object
		/// a reference to the spdlog logger is held as they are not registered to the lookup provided by spdlog and one can avoid the call to lookup everytime this object is used..
		/// @attention the critical_handle suggests that this logger also holds a sink where only critical messages are recorded.
		/// @note using this logger writes to the record all sink associated with this object..
		std::shared_ptr<spdlog::logger> m_log_critical_handle	= nullptr;
	};

	/// this class is basically a lookup class to aquire whichever logger that is required  
	/// 
	/// @warning do not ever make direct calls to the class, it is unnecessary. use the provided macros..at the very least they will get stripped out when the code is being built for shipping increasing performance.
	/// @warning the provided macros dynamically decide which function to call in this class and record an appropriate as to from where this log write has originated.. if you make the wrong call. then the log will be written wrong causing that much more headache while debugging.   
	class log
	{
	public:
		///this function returns an object which can be used to log to the stdout.. the signature is engine..
		///@warning the signature engine states that engine is the one logging no matter who logs using the object provided by this function..
		///@returns an object which can be used to safely log to stdout in a multi-threaded environment with the signature engine.  
		///@see getConsoleLoggerEditor() getConsoleLoggerExecutable() getConsoleLoggerTest() getConsoleLoggerExample()
		static consoleLog getConsoleLoggerEngine();
		
		///this function returns an object which can be used to log to relevant files.. the signature is engine..
		///@warning the signature engine states that engine is the one logging no matter who logs using the object provided by this function..
		///@returns an object which can be used to safely log to stdout in a multi-threaded environment with the signature engine.  
		///@see getFileLoggerEditor() getFileLoggerExecutable() getFileLoggerTest() getFileLoggerExample() 
		static fileLog getFileLoggerEngine();
	};
}
#endif

#if _DEBUG
	#define logConsoleTrace(...)	penguin2D::log::getConsoleLoggerEngine().m_log_handle->trace	(__VA_ARGS__);
	#define logConsoleInfo(...)		penguin2D::log::getConsoleLoggerEngine().m_log_handle->info		(__VA_ARGS__);
	#define logConsoleWarn(...)		penguin2D::log::getConsoleLoggerEngine().m_log_handle->warn		(__VA_ARGS__);
	#define logConsoleError(...)	penguin2D::log::getConsoleLoggerEngine().m_log_handle->error	(__VA_ARGS__);
	#define logConsoleCritical(...)	penguin2D::log::getConsoleLoggerEngine().m_log_handle->critical (__VA_ARGS__);


	#define logFileTrace(...)		penguin2D::log::getFileLoggerEngine().m_log_trace_handle->trace(__VA_ARGS__);
	#define logFileInfo(...)		penguin2D::log::getFileLoggerEngine().m_log_info_handle->info(__VA_ARGS__);
	#define logFileWarn(...)		penguin2D::log::getFileLoggerEngine().m_log_warn_handle->warn(__VA_ARGS__);
	#define logFileError(...)		penguin2D::log::getFileLoggerEngine().m_log_error_handle->error(__VA_ARGS__);
	#define logFileCritical(...)	penguin2D::log::getFileLoggerEngine().m_log_critical_handle->critical(__VA_ARGS__);


	#define logTrace(...)			logConsoleTrace(__VA_ARGS__);\
									logFileTrace(__VA_ARGS__);
									

	#define logInfo(...)			logConsoleInfo(__VA_ARGS__);\
									logFileInfo(__VA_ARGS__);

	#define logWarn(...)			logConsoleWarn(__VA_ARGS__);\
									logFileWarn(__VA_ARGS__);

	#define logError(...)			logConsoleError(__VA_ARGS__);\
									logFileError(__VA_ARGS__);

	#define logCritical(...)		logConsoleCritical(__VA_ARGS__);\
									logFileCritical(__VA_ARGS__);
												
#else
#define logConsoleTrace(...)	
#define logConsoleInfo(...)		
#define logConsoleWarn(...)		
#define logConsoleError(...)	
#define logConsoleCritical(...)	


#define logFileTrace(...)		
#define logFileInfo(...)		
#define logFileWarn(...)		
#define logFileError(...)		
#define logFileCritical(...)	


#define logTrace(...)			
								


#define logInfo(...)			
								

#define logWarn(...)			
								

#define logError(...)			
								

#define logCritical(...)		
								
#endif 