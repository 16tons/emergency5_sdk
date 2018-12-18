// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/map/ObjectEditMode.h"
#include "qsf_editor/operation/street/StreetOperationHelper.h"


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
		*    Modify street network edit mode class
		*
		*  @note
		*    - Not derived from "qsf::editor::StreetEditMode" by intent because we need the same features as available inside the object edit mode and network is big picture anyway
		*/
		class StreetNetworkEditMode : public ObjectEditMode
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::StreetNetworkEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit StreetNetworkEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~StreetNetworkEditMode();

			/**
			*  @brief
			*    Get the default node radius for newly created nodes
			*
			*  @return
			*    The default node radius for newly created nodes
			*/
			float getDefaultNodeRadius() const;

			/**
			*  @brief
			*    Set default node radius for newly created nodes
			*
			*  @param[in] defaultNodeRadius
			*    Default node radius for newly created nodes
			*/
			void setDefaultNodeRadius(float defaultNodeRadius);

			/**
			*  @brief
			*    Create a new node instance with the given local space node position and the default node radius
			*
			*  @param[in] position
			*    Local space node position
			*/
			Node createNode(const glm::vec3& position) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QObject methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void timerEvent(QTimerEvent* qTimerEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*   Enable/disable edit mode of all street crossing component instances
			*
			*  @param[in] enabled
			*    "true" if edit mode is enabled, else "false"
			*  @param[in] forceInstantUpdate
			*    "true" to force instant update, else "false" for delayed update
			*/
			void setEditMode(bool editMode, bool forceInstantUpdate = false);

			void createDrawRequest();
			void clearDrawRequest();

			/**
			*  @brief
			*   Return the currently hot selected gateway, but only in case it's unoccupied
			*
			*  @return
			*    The currently hot selected gateway, null pointer on error (no hot selection or the gateway is already occupied)
			*/
			StreetOperationHelper::Gateway* getUnoccupiedHotSelectedGateway() const;

			/**
			*  @brief
			*   Return whether or not two hot selected street gateways are interconnected
			*
			*  @return
			*    "true" if two hot selected street gateways are interconnected, else "false"
			*/
			bool areHotSelectedGatewaysInterconnected() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			float											mDefaultNodeRadius;			///< Default node radius for newly created nodes
			std::vector<unsigned int>						mDebugDrawRequestIds;		///< Debug draw request IDs
			std::vector<StreetOperationHelper::Gateway>		mGateways;					///< World space gateways
			std::vector<StreetOperationHelper::Gateway*>	mGatewayHotSelected;		///< TODO(co) Review this (pointers are dangerous in here)
			StreetOperationHelper::Gateway*					mGatewaySelect;				///< TODO(co) Review this (pointers are dangerous in here)
			int												mUpdateEditModeQtTimerId;	///< Update edit mode Qt timer ID, "0" means no timer
			int												mUpdateEditMode;			///< New edit mode state to be set when the Qt timer fires


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::StreetNetworkEditMode)
