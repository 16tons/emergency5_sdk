// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/objects/ObjectDamageComponent.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>

#include <vector>


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
	*    EMERGENCY 5 ebox component
	*/
	class EM5_API_EXPORT EboxComponent : public ObjectDamageComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::EboxComponent" unique component ID


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
		inline explicit EboxComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~EboxComponent();

		void setEboxDefective();
		void setEboxDefectiveWithGaping(float gapeChance, float reactionRadius);
		void setEboxIntact();

		inline bool isDefect() const;

		inline const qsf::AssetProxy& getDefectMesh() const;
		inline void setDefectMesh(const qsf::AssetProxy& mesh);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FlashWaitTime, const glm::vec2&, mFlashWaitTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SparksWaitTime, const glm::vec2&, mSparksWaitTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MaxInjuredPersons, int, mMaxInjuredPersons);


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool isReadyForCleanup() const override;
		virtual void cleanup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetDamageState(DamageState damageState) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments& jobArguments);
		void updateSparks(const qsf::Time& timePassed);
		void updateGapersAndShocks(const qsf::Time& timePassed);

		void setMeshToDefect(bool defect);
		void activateFireComponents(bool active);
		void injurePerson(qsf::Entity& entity) const;
		void showSparksParticles();
		void showFlashParticles();
		void registerUpdateProxy();
		void makePersonGaper(qsf::Entity& entity);
		void makePersonSnooper(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;		///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		EntityIdArray mFlashParticlesIds;		///< Array of particles entity IDs
		EntityIdArray mSparksParticlesIds;		///< Array of particles entity IDs

		glm::vec2 mFlashWaitTime;		///< Minimum and maximum waiting time between particle effects
		glm::vec2 mSparksWaitTime;		///< Minimum and maximum waiting time between particle effects

		// Internal
		bool			mIsDefect;
		qsf::JobProxy	mUpdateJobProxy;
		float			mGapeChance;
		float			mReactionRadius;

		std::vector<uint64> mSnoopingPersons;
		std::vector<uint64> mAffectedPersons;	///< All person which were affected by the ebox, also the ones who are not gaping

		qsf::Time mSecondsToNextFlash;			///< Waiting time till the next particle effect is triggered
		qsf::Time mSecondsToNextSparks;			///< Waiting time till the next particle effect is triggered

		qsf::AssetProxy mIntactMesh;
		qsf::AssetProxy mDamageMesh;

		int	mMaxInjuredPersons;
		int mCurrentInjuredPersons;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, EboxComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface FlashParticlesArray;		///< Array of the particles for the flash
		EntityIdArrayInterface SparksParticlesArray;	///< Array of the particles for the sparks


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/objects/EboxComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::EboxComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::EboxComponent::EntityIdArrayInterface)
