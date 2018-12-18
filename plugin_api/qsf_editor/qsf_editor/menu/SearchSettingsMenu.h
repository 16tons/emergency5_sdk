// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qmenu.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class SearchWidget;
		class FilterConfiguration;
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
		*    Search settings menu
		*/
		class SearchSettingsMenu : public QMenu
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			enum ActionType
			{
				FILTER_PREFAB_ASSET_ACTION = 2,
				FILTER_MESH_ASSET_ACTION,
				FILTER_PARTICLES_ASSET_ACTION,
				FILTER_NAME_ACTION,
				FILTER_ID_ACTION,
				FILTER_COMPONENT_ACTION,
				FILTER_TAG_ACTION,
				FILTER_LAYER_ACTION,
				FILTER_DESCRIPTION_ACTION,
				FILTER_BASEMESH_NAME_ACTION,
				SELECT_ALL,
				DESELECT_ALL
			};
			typedef std::vector<QAction*> CheckableQActions;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			SearchSettingsMenu(SearchWidget& searchWidget, FilterConfiguration& filterConfiguration);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SearchSettingsMenu();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void executeQAction(QAction& qAction);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SearchWidget&		 mSearchWidget;
			FilterConfiguration& mFilterConfiguration;
			CheckableQActions	 mCheckableQActions;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
