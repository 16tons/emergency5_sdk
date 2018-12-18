// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/em4Router/actor/base/EActorBase.h"

#include <qsf_ai/navigation/em4Router/wrapper/actor/EVirtualObject.h>

#include <qsf/map/Entity.h>


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
	*     Emergency 5 implementation of EVirtualObject wrapper class for a QSF entity
	*/
	class EVirtualObject : public qsf::ai::EVirtualObject, public EActorBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		EVirtualObject(qsf::Entity& entity);
		virtual ~EVirtualObject();

		// ::EActor
		virtual qsf::Entity& getQsfEntity() override  { return mEntity; }
		virtual bool hasTag(const qsf::StringHash& tag) const override;
		virtual float GetBoundingRadius() override	{ return 0.0f; }				// TODO(co) Implement me
		virtual float GetBoundingRadiusXZ() const override	{ return 0.0f; }		// TODO(co) Implement me
		virtual float GetBoundingRadiusHeight() const override	{ return 0.0f; }		// TODO(co) Implement me
		virtual glm::vec3 GetBBMidPoint() const override	{ return glm::vec3(); }	// TODO(co) Implement me

		// ::EVirtualObject
		virtual qsf::PolygonComponent& getPolygonComponent() const override;
		virtual bool IsWalkable() override			{ return EActorBase::IsWalkable(); }
		virtual qsf::ai::ETerrain* GetTerrain() override		{ return EActorBase::GetTerrain(); }
		virtual qsf::ai::ETerrain* GetTerrain(int) override  { return nullptr; }				// TODO(fw) Implement me
		virtual void initOwnerPlayer() override { mOwnerPlayer = EActorBase::initOwnerPlayer(); }


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::PolygonComponent> mPolygonComponent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
