// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/effect/GroundSpotComponent.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/math/Color3.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireComponent;
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
	*    Fire drone scan component class
	*/
	class EM5_API_EXPORT FireDroneScanComponent : public GroundSpotComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::FireDroneScanComponent" unique component ID


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
		explicit FireDroneScanComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireDroneScanComponent();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ActivateTime, qsf::Time, mActivateTime)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FadeInDuration, qsf::Time, mFadeInDuration)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FadeOutDuration, qsf::Time, mFadeOutDuration)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WaitDuration, qsf::Time, mWaitDuration)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BaseColor, const qsf::Color3&, mBaseColor)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WarningColor, const qsf::Color3&, mWarningColor)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HighlightColor, const qsf::Color3&, mHighlightColor)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GroundTexture, const qsf::AssetProxy&, mGroundTexture)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TextureIntensity, float, mTextureIntensity)


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;
		virtual void onSetActive(bool active) override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateAnimation(const qsf::JobArguments& jobArguments);

		void destroyEffect();
		void collectFireEntitiesInRange(const glm::vec3& effectPosition);
		bool isFireComponentInDistance(const glm::vec3& drone2dPosition, const std::vector<FireComponent*>& fireComponents);
		qsf::Color3 calcHighLightColorForFireComponents(const std::vector<FireComponent*>& fireComponents);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time		mActivateTime;
		qsf::Time		mFadeInDuration;
		qsf::Time		mFadeOutDuration;
		qsf::Time		mWaitDuration;
		qsf::Color3		mBaseColor;
		qsf::Color3		mHighlightColor;
		qsf::Color3		mWarningColor;
		qsf::AssetProxy mGroundTexture;
		float			mTextureIntensity;

		// Internal
		boost::container::flat_map<uint64, qsf::WeakPtr<qsf::Entity>> mFoundObjects;

		// Job proxies for regular update
		qsf::JobProxy mSimulationJobProxy;	///< Regular simulation job proxy; for updates once a game tick
		qsf::JobProxy mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireDroneScanComponent)
