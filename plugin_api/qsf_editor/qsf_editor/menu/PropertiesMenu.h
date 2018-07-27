// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/BaseModel.h"
#include "qsf_editor/menu/asset/AssetMenu.h"


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
		*    Properties menu
		*/
		class PropertiesMenu : public AssetMenu
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
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] indexData
			*    Base model index data this context menu operates on, can be a null pointer
			*/
			explicit PropertiesMenu(const BaseModel::IndexData* indexData);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PropertiesMenu();


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			const BaseModel::IndexData* mIndexData;	///< Base model index data this context menu operates on, can be a null pointer, do not destroy the instance


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredResetToDefault();
			void onTriggeredMarkAsChanged();
			void onTriggeredCopy();
			void onTriggeredPaste();
			void onTriggeredSelectEntity();
			void onTriggeredFlyToEntity();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Return the ID of the entity the currently selected property is assigned with
			*
			*  @return
			*    The ID of the entity the currently selected property is assigned with
			*/
			uint64 getPropertyEntityId() const;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
