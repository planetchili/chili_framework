#include "Log.h"

#if _DEBUG
#include "spdlog/sinks/basic_file_sink.h"

namespace penguin2D
{
	static std::once_flag logInitializerFlag;
	static void initializeLog()
	{
		spdlog::set_pattern("%^[%T] %n : %v%$");
	}

	consoleLog::consoleLog(const std::string& logName)
	{
		//call the initialize function of the library only once at the start of the program.. 
		std::call_once(logInitializerFlag, []() {
			initializeLog();
			});

		m_log_handle = spdlog::stdout_color_mt(logName);
		m_log_handle->set_level(spdlog::level::trace);
	}

	fileLog::fileLog(const std::string& logName)
	{
		//call the initialize function of the library only once at the start of the program..
		std::call_once(logInitializerFlag, []() {
			initializeLog();
			});

		//set filepaths
		std::filesystem::path mainPath	  (std::string(std::string("log/") + logName + std::string("/") + std::string("log.txt")));

		std::filesystem::path tracePath	  (std::string(std::string("log/") + logName + std::string("/") + std::string("traceLog.txt")));
		std::filesystem::path infoPath	  (std::string(std::string("log/") + logName + std::string("/") + std::string("infoLog.txt")));
		std::filesystem::path warnPath	  (std::string(std::string("log/") + logName + std::string("/") + std::string("warnLog.txt")));
		std::filesystem::path errorPath	  (std::string(std::string("log/") + logName + std::string("/") + std::string("errorLog.txt")));
		std::filesystem::path criticalPath(std::string(std::string("log/") + logName + std::string("/") + std::string("criticalLog.txt")));

		//creating sinks
		spdlog::sink_ptr mainLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(mainPath.u8string());

		spdlog::sink_ptr traceLogSink	 = std::make_shared<spdlog::sinks::basic_file_sink_mt>(tracePath.u8string());
		spdlog::sink_ptr infoLogSink	 = std::make_shared<spdlog::sinks::basic_file_sink_mt>(infoPath.u8string());
		spdlog::sink_ptr warnLogSink	 = std::make_shared<spdlog::sinks::basic_file_sink_mt>(warnPath.u8string());
		spdlog::sink_ptr errorLogSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(errorPath.u8string());
		spdlog::sink_ptr criticalLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(criticalPath.u8string());

		//create spdlog loggers
		std::vector<spdlog::sink_ptr> loggerSinks;
		loggerSinks.emplace_back(mainLogSink);
		loggerSinks.emplace_back(traceLogSink);
		m_log_trace_handle = std::make_shared<spdlog::logger>(logName + std::string("TraceLogger"),loggerSinks.begin(),loggerSinks.end());
		m_log_trace_handle->set_level(spdlog::level::trace);
		loggerSinks.clear();

		loggerSinks.emplace_back(mainLogSink);
		loggerSinks.emplace_back(infoLogSink);
		m_log_info_handle = std::make_shared<spdlog::logger>(logName + std::string("InfoLogger"), loggerSinks.begin(),loggerSinks.end());
		m_log_info_handle->set_level(spdlog::level::info);
		loggerSinks.clear();

		loggerSinks.emplace_back(mainLogSink);
		loggerSinks.emplace_back(warnLogSink);
		m_log_warn_handle = std::make_shared<spdlog::logger>(logName + std::string("WarnLogger"), loggerSinks.begin(), loggerSinks.end());
		m_log_warn_handle->set_level(spdlog::level::warn);
		loggerSinks.clear();

		loggerSinks.emplace_back(mainLogSink);
		loggerSinks.emplace_back(errorLogSink);
		m_log_error_handle = std::make_shared<spdlog::logger>(logName + std::string("ErrorLogger"), loggerSinks.begin(),loggerSinks.end());
		m_log_error_handle->set_level(spdlog::level::err);
		loggerSinks.clear();

		loggerSinks.emplace_back(mainLogSink);
		loggerSinks.emplace_back(criticalLogSink);
		m_log_critical_handle = std::make_shared<spdlog::logger>(logName + std::string("CriticalLogger"), loggerSinks.begin(),loggerSinks.end());
		m_log_critical_handle->set_level(spdlog::level::trace);
		loggerSinks.clear();
	}

	consoleLog log::getConsoleLoggerEngine()
	{
		static consoleLog logObject("engine");
		return logObject;
	}
	fileLog log::getFileLoggerEngine()
	{
		static fileLog logObject("engine");
		return logObject;
	}
}
#endif