#include "logs.h"

#include <QDir> // for CreateDir()

/**
static bool g_bInited = false;
static std::shared_ptr<spdlog::sinks::qt_sink_mt> s_qtSink = nullptr;

// Create directory, return 0 if success.
static int CreateDir(const char* dirPath)
{
    QDir dir;
    bool bSuccess = dir.mkpath(dirPath);
    if (!bSuccess) {
        return -1;
    }

    return 0;
}

int initLog(const char *filename)
{
    // If inited, just return;
    if (g_bInited) {
        return 0;
    }

    // Create directory
    // Now can only support create one directory, if it's
    //     ./dir/nonexist1/nonexist2/aaa.log, then it will fail.
    do {
        std::string strFilename = filename;
        // replace all '\' with '/'
        for (auto it = strFilename.begin(); it != strFilename.end(); ++it) {
            if (*it == '\\') {
                *it = '/';
            }
        }
        auto pos = strFilename.find_last_of('/');
        if (std::string::npos == pos) {
            break;
        }
        std::string strDirPath = strFilename.substr(0, pos);
        CreateDir(strDirPath.c_str());
    } while (0);

    try {
        spdlog::level::level_enum level = spdlog::level::info;
#ifdef _DEBUG
        level = spdlog::level::trace;
#endif

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        //console_sink->set_level(level);
        console_sink->set_pattern("[%T.%e] [%^%L%$] [%t] [%s:%#;%!] %v");

        auto daily_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filename, 0, 0);
        //daily_file_sink->set_level(level);
        daily_file_sink->set_pattern("[%T.%e] [%^%L%$] [%t] [%s:%#;%!] %v");

        auto logger = std::make_shared<spdlog::logger>("multi_sink",
                                                       spdlog::sinks_init_list({ console_sink, daily_file_sink }));
        logger->flush_on(spdlog::level::trace);
        logger->set_level(level);
        spdlog::set_default_logger(logger);
    } catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }

    g_bInited = true;

    return 0;
}

void SetLogLevel(int level)
{
    if (level < spdlog::level::trace || level > spdlog::level::err) {
        L_ERROR("Invalid log level: {}", level);
        return;
    }

    spdlog::default_logger()->set_level(spdlog::level::level_enum(level));
}
**/

/**
template<typename T>
void log_error(T t)
{

}
template<typename T, typename... Args>
void log_error(T t, Args... args)
{

}

template<typename T>
static void log_info(T t)
{

}
template<typename T, typename... Args>
static void log_info(T t, Args... args)
{

}

template<typename T>
void log_trace(T t)
{

}
template<typename T, typename... Args>
void log_trace(T t, Args... args)
{

}



template<typename T>
void log_warning(T t)
{

}
template<typename T, typename... Args>
void log_warning(T t, Args... args)
{

}
**/
