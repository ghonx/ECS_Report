#ifndef QLIB_GLOBAL_H
#define QLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLIB_LIBRARY)
#  define QLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QLIB_GLOBAL_H
