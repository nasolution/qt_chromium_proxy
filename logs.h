#ifndef LOGS_H
#define LOGS_H

// Under DEBUG mode, we will log all from trace to error.
// Under RELEASE mode, we will only log from info to error.
#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE  // Now use trace compile under release.
#endif



#include "KVVersion.h"
#include <memory>
#include <QString>
#include <QStringList>
#include <QDebug>

/**
// Must be called before any logging.
// @param[in] path log file name prefix.
int initLog(const char *filename);

// Set current log level.
// 0: trace; 1: debug; 2: info; 3: warn; 4: error.
void SetLogLevel(int level);
**/

// Qt Logging Framework:
// https://www.kdab.com/wp-content/uploads/stories/slides/Day2/KaiKoehne_Qt%20Logging%20Framework%2016_9_0.pdf

template<typename T>
static void log_warning(T t)
{
    Q_UNUSED(t);
#ifdef LOG_WARNING
    qDebug() << t;
#endif
}


template<typename T>
static void log_error(T t)
{
    Q_UNUSED(t);
#ifdef LOG_ERRORS
    qDebug() << t;
#endif
}


template<typename T>
static void log_info(T t)
{
Q_UNUSED(t)
#ifdef LOG_INFOS
    qDebug() << t;
#endif
}


template <typename T>
static void log_trace(T t)
{
Q_UNUSED(t)
#ifdef LOG_TRACE
    qDebug() << t;
#endif
}



/**
#if !defined(SPD_NO_LOG)
#define L_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define L_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define L_INFO(...)  SPDLOG_INFO(__VA_ARGS__)
#define L_WARN(...)  SPDLOG_WARN(__VA_ARGS__)
#define L_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define L_FATAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
**/

#endif // LOGS_H
