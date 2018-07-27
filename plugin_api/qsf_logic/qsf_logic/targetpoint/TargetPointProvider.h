// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"
#include "qsf_logic/targetpoint/TargetPoint.h"

#include <qsf/plugin/pluginable/Pluginable.h>
#include <qsf/math/Transform.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace logic
	{
		class TargetPointManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A target point provider manages constraints for a type of interaction between entities.
		*    It returns actual target points that are spatial configurations which allow the interaction given the entities current setup.
		*    Since a target point is relative to the interacting as well as the interacted on entity it may be dynamic and is often invalidated as soon as one of the entities moves.
		*/
		class QSF_LOGIC_API_EXPORT TargetPointProvider : public Pluginable<TargetPointManager>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit TargetPointProvider(TargetPointManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TargetPointProvider();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::TargetPointProvider methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Deliver target points based on the two entities' current state.
			*
			*  @param[in] caller
			*    The caller entity, i.e. the entity requesting a target point; is possibly ignored when calculating the target point
			*  @param[in] target
			*    The target entity, i.e. the one to deliver a target point for
			*  @param[out] targetPoints
			*    The target point options that should be used for an interaction between these two entities.
			*/
			void calculateTargetPoints(Entity& caller, Entity& target, std::vector<TargetPoint>& targetPoints) const;

			/**
			*  @brief
			*    Deliver target points based on the two entities' current state but assuming a specific caller and target transform.
			*    This is used for future projections but also by the simple call providing the current state as parameters here.
			*
			*  @param[in] caller
			*    The caller entity, i.e. the entity requesting a target point; is possibly ignored when calculating the target point
			*  @param[in] target
			*    The target entity, i.e. the one to deliver a target point for
			*  @param[in] callerTransform
			*    The transform information to assume for the caller instead of the values defined inside its current transform.
			*  @param[out] targetPoints
			*    The target point options that should be used for an interaction between these two entities.
			*  @param[in] targetTransform
			*    The transform information to assume for the target entity instead of the values defined inside its current transform.
			*/
			virtual void calculateTargetPoints(Entity& caller, const Transform& callerTransform, Entity& target, const Transform& targetTransform, std::vector<TargetPoint>& targetPoints) const = 0;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::TargetPointProvider)
