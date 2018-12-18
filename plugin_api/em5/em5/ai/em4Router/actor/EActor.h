// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/em4Router/actor/base/EActorBase.h"

#include <qsf_ai/navigation/em4Router/wrapper/actor/EActor.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Emergency 5 implementation of EActor wrapper class for a QSF entity
	*/
	class EActor : public qsf::ai::EActor, public EActorBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		virtual qsf::Entity& getQsfEntity() override  { return mEntity; }
		virtual bool hasTag(const qsf::StringHash& tag) const override;
		virtual float GetBoundingRadius() override	{ return 0.0f; }				// TODO(co) Implement me
		virtual float GetBoundingRadiusXZ() const override	{ return 0.0f; }		// TODO(co) Implement me
		virtual float GetBoundingRadiusHeight() const override { return 0.0f; }
		virtual glm::vec3 GetBBMidPoint() const override	{ return glm::vec3(); }	// TODO(co) Implement me

		virtual void initOwnerPlayer() override { mOwnerPlayer = EActorBase::initOwnerPlayer(); }


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		EActor(qsf::ai::EActorType actorType, qsf::Entity& entity);
		virtual ~EActor();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
