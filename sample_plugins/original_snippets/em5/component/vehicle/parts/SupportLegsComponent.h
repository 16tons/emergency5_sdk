// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		class SupportLegData;
	}
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
	*  @remarks
	*    EMERGENCY 5 Support legs component
	*
	*  @brief
	*    Special emergency 5 units can must deploy support legs for starting work (To support optical stability for heavy working, eg lifting an vehicle).
	*    In case the deploy them self they can't move.
	*/
	class EM5_API_EXPORT SupportLegsComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class multiplayer::SupportLegData; // Needed for multiplay


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::SupportLegsComponent" unique component ID

		enum SupportLegsState
		{
			SUPPORTLEGSSTATE_HIDDEN,
			SUPPORTLEGSSTATE_EXTENDING_ANIMATION,
			SUPPORTLEGSSTATE_EXTENDED,
			SUPPORTLEGSSTATE_HIDDING_ANIMATION
		};


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
		inline explicit SupportLegsComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SupportLegsComponent();

		inline const qsf::AssetProxy& getAnimationExtend() const;
		inline void setAnimationExtend(const qsf::AssetProxy& skeleton);

		void startExtending();
		void hideExtending();

		bool isAnimationPlaying();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void deactivateJob();
		void updateJob(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;	///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SupportLegsState	mSupportLegsState;
		EntityIdArray		mSupportLegIds;			///< List of the entity ids for the support legs
		qsf::AssetProxy		mAnimationExtend;

		// Internal
		qsf::JobProxy		mUpdateJobProxy;
		AudioProxy			mAudioProxy;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, SupportLegsComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface SupportLegsArray;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/parts/SupportLegsComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SupportLegsComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::SupportLegsComponent::EntityIdArrayInterface)
