// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/transform/TransformEditMode.h"
#include "qsf_editor/selection/node/NodeSelectionManager.h"

#include <qsf/component/nodes/Node.h>

#include <vector>


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
		*    Node transform edit mode class
		*
		*  @todo
		*    - TODO(co) Add support for node cloning
		*/
		class NodeTransformEditMode : public TransformEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NodeTransformEditMode" unique pluginable edit mode ID


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
			explicit NodeTransformEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodeTransformEditMode();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::TransformEditMode methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onTransformChange(const Transform& transform) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NodeSelectionManager::IdSet	mNodeIdSet;			///< IDs of the nodes selected at the point in time where the transform mode startup was performed
			std::vector<glm::vec3>		mStartupPosition;	///< The world space position of the selected nodes at the point in time the transform edit mode became active
			std::vector<Node>			mStartupNodes;		///< The nodes at the point in time the transform edit mode became active


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NodeTransformEditMode)
