// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/em4Router/actor/base/EGenericObjectBase.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/em4Router/wrapper/actor/EPerson.h>


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
	*    Emergency 5 implementation of EPerson wrapper class for a QSF entity
	*/
	class EPerson : public qsf::ai::EPerson, public EGenericObjectBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		EPerson(qsf::Entity& entity);
		virtual ~EPerson();

		// ::EGenericObject
		virtual const glm::vec3& GetPosition() override			{ return EGenericObjectBase::GetPosition(); }
		virtual const glm::mat3x3& GetRotationMatrix() override	{ return EGenericObjectBase::GetRotationMatrix(); }
		virtual bool IsWalkable() override						{ return EActorBase::IsWalkable(); }
		virtual qsf::ai::ETerrain* GetTerrain() override					{ return EGenericObjectBase::GetTerrain(); }
		virtual qsf::ai::ETerrain* GetTerrain(int index) override		{ return EGenericObjectBase::GetTerrain(index); }
		virtual qsf::ai::EObjectTerrainClass GetTerrainClass() override;
		virtual qsf::ai::EFloorPlacement GetPlacement() override		{ return EGenericObjectBase::GetPlacement(); }
		virtual void UpdatePlacement(glm::vec3& position_, glm::mat3x3& rotation_)  override	{ EGenericObjectBase::UpdatePlacement(position_, rotation_); }
		virtual int GetCarriedPersonID() const override			{ return EGenericObjectBase::GetCarriedPersonID(); }
		virtual void initOwnerPlayer() override { mOwnerPlayer = EActorBase::initOwnerPlayer(); }

		// ::EPerson
		virtual bool IsArrested() const override;
		virtual bool IsDiver() const override;
		virtual bool IsDrowning() const override;
		virtual bool IsJumping() const override;
		virtual bool IsCollisionIgnored(EActor& actor) const override;
		virtual bool CanRunFromUnitCar() const override;
		virtual int GetIsFollowing() const override;
		virtual int GetLinkedPersonID() const override;
		virtual glm::vec3 GetRelLinkPosition() const override;
		virtual const glm::vec3& getBoundingBoxCenter() override  { return mBoundingBoxCenter; }
		virtual const glm::vec3& getBoundingBoxExtents() override  { return mBoundingBoxExtents; }
		void setBoundingBox(const glm::vec3& center, const glm::vec3& extents);
		virtual qsf::ai::EPersonRole GetRole() const override;

		// ::EActor
		virtual qsf::Entity& getQsfEntity() override  { return mEntity; }
		virtual bool hasTag(const qsf::StringHash& tag) const override;

		void updateRole();
		void updateTerrainClass();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Caching of values for performance reasons
		qsf::ai::EPersonRole		 mRole;
		qsf::ai::EObjectTerrainClass mTerrainClass;
		EntityHelper				 mEntityHelper;
		glm::vec3					 mBoundingBoxCenter;
		glm::vec3					 mBoundingBoxExtents;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
