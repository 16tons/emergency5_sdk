// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/equipment/weapon/WeaponConfiguration.h"

#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PhysicsWorldComponent;
}


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
	*    EMERGENCY 5 hunt and shoot action
	*
	*  @note
	*    - This action is pushed to entities to start shooting at other targets, pushes other actions like move, shoot, play animation
	*/
	class EM5_API_EXPORT HuntAndShootAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::HuntAndShootAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HuntAndShootAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HuntAndShootAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(qsf::Entity& targetEntity, const weapon::WeaponConfiguration& weaponConfiguration);

		/**
		*  @brief
		*    Return shoot range in meters
		*/
		float getShootRange() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool isTargetValid() const;
		void playStandUpAnimation();
		void showOutOfRangeHint(const glm::vec3& position) const;
		bool hasFreeSightlineTo(qsf::PhysicsWorldComponent* physicsWorldComponent, const qsf::TransformComponent& startTransform, const qsf::TransformComponent& targetTransform, qsf::Entity* targetEntity);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_GOTOTARGET,
			STATE_SHOOT,
			STATE_END
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State						mCurrentState;
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		uint32						mNumberOfShots;
		weapon::WeaponConfiguration	mWeaponConfiguration;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HuntAndShootAction)
