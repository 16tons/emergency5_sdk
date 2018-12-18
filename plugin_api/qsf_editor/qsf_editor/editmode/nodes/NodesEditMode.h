// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/EditMode.h"

#include <qsf/component/nodes/Node.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class NodesComponent;
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
		*    Abstract nodes edit mode base class
		*/
		class NodesEditMode : public EditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NodesEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodesEditMode();

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
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit NodesEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Return the used nodes component instance
			*
			*  @return
			*    The used nodes component instance, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			NodesComponent* getNodesComponent() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void focusInEvent(QFocusEvent& qFocusEvent) override;
			virtual void focusOutEvent(QFocusEvent& qFocusEvent) override;


		//[-------------------------------------------------------]
		//[ Protected Qt slots (MOC)                              ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			void onTriggeredSnapToGround();
			void onTriggeredSnapToObject();
			void onTriggeredSnapToVertex();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			float mDefaultNodeRadius;	///< Default node radius for newly created nodes


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NodesEditMode)
