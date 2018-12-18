// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf_ai/navigation/Path.h>

#include <qsf/component/nodes/PathComponent.h>
#include <qsf/logic/action/ActionPriority.h>
#include <qsf/logic/action/ActionInsertionMode.h>

#include <memory>


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
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Encapsulates a static path definition that can easily be converted into a movement path for the AI
		*
		*  @remarks
		*    It currently only creates very simple paths.
		*    It does not support turning radius and curves but only straight segments.
		*    It doesn't use area or node ids but always considers the moved terrain illegal.
		*    It doesn't support moving backwards and always moves on the primary map (which doesn't matter much without area ids).
		*/
		class QSF_GAME_API_EXPORT AiPathComponent : public PathComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::AiPathComponent" unique component ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			inline explicit AiPathComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~AiPathComponent();

			/**
			*  @brief
			*    Convert the contained node structure into a movement path usable by the AI
			*/
			std::auto_ptr<ai::Path> createAiPath() const;

			/**
			*  @brief
			*    Convert the contained node structure into a movement path usable by the AI
			*    It smoothes the path according the radius values of the nodes
			*/
			std::auto_ptr<ai::Path> createAiPathSmoothed() const;

			/**
			*  @brief
			*     Comfort version for letting the entity move along the path
			*
			*  @remarks
			*     The action is pushed with the parameters passed here. See ActionComponent.pushAction() for the details.
			*/
			void makeEntityMoveAlongPath(Entity& entity, action::Priority priority, action::InsertionMode insertionMode = action::REPLACE_WHOLE_PLAN) const;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/movement/AiPathComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::AiPathComponent)
