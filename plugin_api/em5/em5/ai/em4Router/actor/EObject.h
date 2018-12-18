// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/em4Router/actor/base/EGenericObjectBase.h"

#include <qsf_ai/navigation/em4Router/wrapper/actor/EObject.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	 *   @brief
	 *     Emergency 5 implementation of EObject wrapper class for a QSF entity
	 */
	class EObject : public qsf::ai::EObject, public EGenericObjectBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		EObject(qsf::Entity& entity);
		virtual ~EObject();

		// ::EActor
		virtual qsf::Entity& getQsfEntity() override  { return mEntity; }
		virtual bool hasTag(const qsf::StringHash& tag) const override;
		virtual void initOwnerPlayer() override { mOwnerPlayer = EActorBase::initOwnerPlayer(); }

		// ::EGenericObject
		virtual const glm::vec3& GetPosition() override			{ return EGenericObjectBase::GetPosition(); }
		virtual const glm::mat3x3& GetRotationMatrix() override	{ return EGenericObjectBase::GetRotationMatrix(); }
		virtual bool IsWalkable() override						{ return EActorBase::IsWalkable(); }
		virtual qsf::ai::ETerrain* GetTerrain() override				{ return EGenericObjectBase::GetTerrain(); }
		virtual qsf::ai::ETerrain* GetTerrain(int index) override		{ return EGenericObjectBase::GetTerrain(index); }
		virtual qsf::ai::EObjectTerrainClass GetTerrainClass() override;
		virtual qsf::ai::EFloorPlacement GetPlacement() override		{ return EGenericObjectBase::GetPlacement(); }
		virtual void UpdatePlacement(glm::vec3& position_, glm::mat3x3& rotation_)  override	{ EGenericObjectBase::UpdatePlacement(position_, rotation_); }
		virtual int GetCarriedPersonID() const override			{ return EGenericObjectBase::GetCarriedPersonID(); }


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
