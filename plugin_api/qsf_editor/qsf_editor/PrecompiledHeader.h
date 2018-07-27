// Copyright (C) 2012-2017 Promotion Software GmbH


// Do never ever include this optional precompiled header within another public header!
// The precompiled header is only used to speed up compilation, it's no replacement for
// a decent header design. Be clever, don't use precompiled headers like a sledgehammer
// or you increase instead of decrease the compile time faster as you can say "bad idea".


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
// We can't use pragma once here since precompiled headers get a special treatment on Mac OS X that is incompatible with #pragma once
#ifndef _QSF_EDITOR_PRECOMPILED_HEADER_H_
#define _QSF_EDITOR_PRECOMPILED_HEADER_H_


// This here should recognize when you try to include multiple precompiled headers.
// Use this block without modification of "_QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_"
// in all precompiled headers of your project.
#ifdef _QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_
	#error Multiple precompiled headers included. The reason could be inclusion of a precompiled header in a public header file.
#else
	#define _QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// QSF (stable)
#ifdef _MSC_VER
	#include <qsf/platform/msvc_no_sillywarnings_please.h> // Disables nasty VisualStudio warnings
#endif
#include <qsf/base/Memory.h>
#include <qsf/base/profiler/Profiler.h>

// Include this before it's included by other headers
#ifdef _MSC_VER
	// Include this before it's included by other headers
	#include <xutility>
#endif

// Qt (e.g. "QtWidgets/QLabel" instead of "QtWidgets/qlabel.h" preferred in here because Qt UI will generate headers using the "QtWidgets/QLabel variant)
#include <QtWidgets/QLabel>
#include <QtWidgets/QAction>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWhatsThis>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QUndoStack>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QFont>
#include <QtGui/QColor>
#include <QtGui/qevent.h>
#include <QtGui/QClipboard>
#include <QtGui/QKeySequence>
#include <QtGui/QGuiApplication>
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include <QtCore/QSize>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QSettings>
#include <QtCore/QMimeData>
#include <QtCore/QSettings>
#include <QtCore/qglobal.h>
#include <QtCore/QMetaType>
#include <QtCore/QFileInfo>
#include <QtCore/QByteArray>
#include <QtCore/QTranslator>
#include <QtCore/QStringList>
#include <QtCore/qnamespace.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QAbstractProxyModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QAbstractItemModel>

// CAMP
#include <camp/class.hpp>

// GLM
#include <glm/glm.hpp>

// Boost
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/nowide/convert.hpp>

// C++ standard headers
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <deque>
#include <vector>
#include <string>
#include <limits>
#include <memory>

#endif // _QSF_EDITOR_PRECOMPILED_HEADER_H_
