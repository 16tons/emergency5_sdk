// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/polygon/PolygonComponent.h"
#include "qsf/asset/AssetProxy.h"
#include "qsf/reflection/type/CampQsfTime.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AmbientAudioManagementComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Ambient polygon component class
	*
	*  @remarks
	*    Defines a volume from which ambient sound is emitted. All sound emitting ambient polygons are managed by the core component ambient audio management component.
	*/
	class QSF_API_EXPORT AmbientPolygonComponent : public PolygonComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::AmbientPolygonComponent" unique component ID
		class ComplexPolygonAudioEmitter;	///< Forward declaration for hidden implementation


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
		inline explicit AmbientPolygonComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AmbientPolygonComponent();

		/**
		*  @brief
		*    Returns a proxy for the sound asset which is played on this ambient polygon
		*/
		inline const AssetProxy& getAmbientSoundAsset() const;

		/**
		*  @brief
		*    Changes the sound asset which is played on this polygon
		*/
		void setAmbientSoundAsset(const AssetProxy& soundAssetProxy);

		inline float getVolume() const;
		inline void setVolume(float newVolume);

		inline float getMaximumVolume() const;
		inline void setMaximumVolume(float newMaximumVolume);

		inline const Time& getOnTime() const;
		inline void setOnTime(const qsf::Time& time);

		inline const Time& getOffTime() const;
		inline void setOffTime(const qsf::Time& time);

		float getCurrentVolume() const;

		void updateDayTime(const qsf::Time& currentDayTime);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PolygonComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual const Color4& getPolygonDebugColor() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetProxy						 mAmbientSoundAssetProxy;	///< Sound which is played from this ambient polygon
		AmbientAudioManagementComponent* mAmbientAudioManagement;
		ComplexPolygonAudioEmitter*		 mEmitter;					///< We have to destroy the instance if we no longer need it
		float							 mVolume;					///< Volume of this ambient sound
		float							 mMaximumVolume;			///< The maximum volume level the ambient sound can reach
		float							 mCurrentVolume;			///< Volume for this actual time of day
		Time							 mOnTime;					///< The time of day when the ambient polygon gets active
		Time							 mOffTime;					///< The time of day when the ambient polygon gets inactive again


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/polygon/AmbientPolygonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AmbientPolygonComponent)
