// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/transform/TransformEditMode.h"

#include <qsf/math/Ray.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
		*    Abstract vertex snap edit mode base class
		*/
		class VertexSnapEditMode : public TransformEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::VertexSnapEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~VertexSnapEditMode();


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
			explicit VertexSnapEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Return the ray to use for vertex snapping
			*
			*  @return
			*    World space ray used for vertex snapping
			*/
			Ray getRay() const;

			/**
			*  @brief
			*    Find the nearest vertex position for a given ray
			*
			*  @param[in] ray
			*    World space ray to find the nearest world space vertex position for
			*  @param[in] entity
			*    Entity to check the vertices from
			*  @param[in/out] nearestDistance
			*    Current nearest distance
			*  @param[in/out] nearestPosition
			*    Current nearest position
			*
			*  @return
			*    "true" if the nearest position was updated, else "false"
			*/
			bool findNearestEntityVertex(const Ray& ray, const Entity& entity, float& nearestDistance, glm::vec3& nearestPosition);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void keyReleaseEvent(QKeyEvent& qEvent) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::VertexSnapEditMode)
