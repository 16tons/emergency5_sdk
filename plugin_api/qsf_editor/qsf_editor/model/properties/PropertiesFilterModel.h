// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qsortfilterproxymodel.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class EditorSettingsGroup;
	}
}


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
		*    Object property Qt filter model
		*
		*  @note
		*    - This class filters components and properties from the user view
		*    - See http://qt-project.org/doc/qt-4.8/qsortfilterproxymodel.html as reference
		*/
		class PropertiesFilterModel : public QSortFilterProxyModel
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
			explicit PropertiesFilterModel(QObject* parent);
			virtual ~PropertiesFilterModel();


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EditorSettingsGroup& mEditorSettingsGroup;	///< Editor settings group instance; The filter is using it quite often, so we cache the reference


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
