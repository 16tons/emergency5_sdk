// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qfilesystemmodel.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF file system model with improved read-only mode
		*/
		class FileSystemModel: public QFileSystemModel
		{


			//[-------------------------------------------------------]
			//[ Qt definitions (MOC)                                  ]
			//[-------------------------------------------------------]
			Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
						// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				explicit FileSystemModel(QObject* parent = 0);


			//[-------------------------------------------------------]
			//[ Protected virtual QAbstractItemModel methods          ]
			//[-------------------------------------------------------]
			protected:
				virtual Qt::DropActions supportedDragActions() const override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
