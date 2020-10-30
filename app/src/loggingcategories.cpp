#include "loggingcategories.h"

Q_LOGGING_CATEGORY(logDebug,    "Debug")
Q_LOGGING_CATEGORY(logInfo,     "Info")
Q_LOGGING_CATEGORY(logWarning,  "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")
Q_LOGGING_CATEGORY(log_short_Info, "short_Info")

//#define logDebug    QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).debug
//#define logInfo     QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).info
//#define logWarning  QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).warning
//#define logCritical QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).critical
//#define logFatal    QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).fatal
