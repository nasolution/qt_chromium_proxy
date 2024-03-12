#ifndef KVVERSION_H
#define KVVERSION_H
#include <QString>
#include <QDir>

static const QString KV_MAINTENANCE_URL = "https://mt.aleph-archives.com/";
static const QString TOKEN_HEADER = "X-MT-TOKEN";
static const QString  TOKEN_HEADER_VALUE = "123";
static const QString USER_AGENT = "Mozilla/5.0 kv_";
static const QString  LINUX_UPDATE_INSTALL_PATH = QDir::homePath() + "/AlephArchives/kv/";
#define KV_CHECK_UPDATE   6 // in hours
#define KV_VERSION_MAJOR  3
#define KV_VERSION_MINOR  1
#define KV_VERSION_BUILD  1

#define UPDATE_NOTES_WINDOW_WIDTH 600
#define UPDATE_NOTES_WINDOW_HEIGHT 450

#define DEBUG_WINDOW_WIDTH  1024
#define DEBUG_WINDOW_HEIGHT 768

#define DEBUG_HIDE_INTERCEPTED_URL

static const QString ARCHIVE_NAME_DEFAULT="https://www.aleph-archives.com/";
static const QString ARCHIVE_TITLE_DEFAULT="Archives Debugger";
static const QString ARCHIVE_UUID_DEFAULT="e4f59d56-adf1-4001-e17d-468a020046c6";
static const QString ARCHIVE_DATE_DEFAULT="2023-10-23 09:00:00";

//#define DEBUG_NO_INTERCEPT
//#define DEBUG_SHOW_SUBWINDOW_ONLOAD
//#define DEBUG_JAKE

//#define LOG_WARNINGS
#define LOG_INFOS

//#define NANO_MODE

#endif // KVVERSION_H
