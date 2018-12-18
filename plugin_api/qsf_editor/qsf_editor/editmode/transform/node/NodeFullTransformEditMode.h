// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/transform/node/NodeTransformEditMode.h"
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
		*    Node full transform edit mode class. It's like the NodeTransformEditMode, but supports
		*    changing the whole transform (i.e. per-node rotation and scale) too.
		*
		*  @todo
		*    - TODO(co) Add support for node cloning
		*/
		class NodeFullTransformEditMode : public NodeTransformEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NodeFullTransformEditMode" unique pluginable edit mode ID


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
			explicit NodeFullTransformEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodeFullTransformEditMode();


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
			std::vector<Transform>		mStartupTransform;	///< The world space position of the selected nodes at the point in time the transform edit mode became active
			std::vector<Node>			mStartupNodes;		///< The nodes at the point in time the transform edit mode became active
			glm::mat4x4					mWorldSpaceIntoObjectSpace;	///< Transformation matrix to bring a world-space node transform into local node-space of the edited component

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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NodeFullTransformEditMode)
