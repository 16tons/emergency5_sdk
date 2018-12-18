// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/reflection/type/CampQsfTime.h>
#include <qsf/reflection/type/CampQsfColor4.h>
#include <qsf/base/PropertyHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Weather state class
		*
		*  @remarks
		*    Defines a weather state.
		*/
		class QSF_GAME_API_EXPORT WeatherState : public Object
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit WeatherState(uint32 key = 0);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~WeatherState();

			const std::string& getName() const;
			void setName(const std::string& name);

			uint32 getWeatherId() const;
			void setWeatherId(uint32 id);

			const std::string& getSource() const;
			void setSource(const std::string& source);

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ChangeDuration,  Time, mChangeDuration)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MinimumDuration, Time, mMinimumDuration)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MaximumDuration, Time, mMaximumDuration)

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SunColor,			  const Color4&, mSunColor)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AmbientColor,		  const Color4&, mAmbientColor)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CloudColor,			  const Color4&, mCloudColor)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CloudDensity,		  float, mCloudDensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FogDiffuseColor,	  const Color4&, mFogDiffuseColor)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AtmosphereFogDensity, float, mAtmosphereFogDensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GroundFogDensity,	  float, mGroundFogDensity)

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HdrExposureIntensity,		   float, mHdrExposureIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DepthGradingNearColor,		   const Color4&, mDepthGradingNearColor)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DepthGradingFarColor,		   const Color4&, mDepthGradingFarColor)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ColorGradingBlend,			   float, mColorGradingBlend)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GlobalReflectionSaturation,	   float, mGlobalReflectionSaturation)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GlobalReflectionMap0Intensity, float, mGlobalReflectionMap0Intensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GlobalReflectionMap1Intensity, float, mGlobalReflectionMap1Intensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CloudsIntensity,			   float, mCloudsIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BloomIntensity,				   float, mBloomIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WetSurfacesIntensity,		   float, mWetSurfacesIntensity)

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RainIntensity,				   float, mRainIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(StormIntensity,				   float, mStormIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WindIntensity,				   float, mWindIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FlashFrequency,				   float, mFlashFrequency)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SnowIntensity,				   float, mSnowIntensity)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AshFallIntensity,			   float, mAshFallIntensity)

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FireAnimationSpeed,				  float, mFireAnimationSpeed);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WaterAnimationSpeed,			  float, mWaterAnimationSpeed);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VegetationAnimationSpeed,		  float, mVegetationAnimationSpeed);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VegetationAnimationFrequency,	  float, mVegetationAnimationFrequency);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VegetationAnimationWeight,		  float, mVegetationAnimationWeight);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VegetationAnimationBending,		  float, mVegetationAnimationBending);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VegetationAnimationIndividuality, float, mVegetationAnimationIndividuality);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VegetationAnimationCrosswind,	  float, mVegetationAnimationCrosswind);

			//[-------------------------------------------------------]
			//[ Serialization                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Serialize this weather instance
			*
			*  @param[in] serializer
			*    The serializer, which can be either in read or write mode
			*/
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string	mName;				///< Name of this weather state
			uint32		mWeatherId;			///< ID of this weather state
			std::string	mSource;			///< Source of this weather state, usually an UTF-8 virtual file name

			Time		mChangeDuration;	///< Time how long it takes to fade to the next weather
			Time		mMinimumDuration;	///< Minimum time the weather is present
			Time		mMaximumDuration;	///< Maximum time the weather is present

			Color4		mSunColor;				///< Sun color (RGB) and lerp factor of this property (A)
			Color4		mAmbientColor;			///< Ambient color (RGB) and lerp factor of this property (A)
			Color4		mCloudColor;			///< Cloud color (RGB) and lerp factor of this property (A)
			float		mCloudDensity;			///< Cloud density factor
			Color4		mFogDiffuseColor;		///< Fog diffuse color (RGB) and lerp factor of this property (A)
			float		mAtmosphereFogDensity;	///< Atmosphere fog density factor
			float		mGroundFogDensity;		///< Ground fog density factor

			float		mHdrExposureIntensity;
			Color4		mDepthGradingNearColor;
			Color4		mDepthGradingFarColor;
			float		mColorGradingBlend;
			float		mGlobalReflectionSaturation;
			float		mGlobalReflectionMap0Intensity;
			float		mGlobalReflectionMap1Intensity;
			float		mCloudsIntensity;
			float		mBloomIntensity;
			float		mWetSurfacesIntensity;

			float		mRainIntensity;		///< Rain intensity between 0 and 1
			float		mStormIntensity;	///< Storm intensity between 0 and 1
			float		mWindIntensity;		///< Wind intensity between 0 and 1
			float		mFlashFrequency;	///< Lightning flash frequency between 0 and 1
			float		mSnowIntensity;		///< Snow intensity between 0 and 1
			float		mAshFallIntensity;	///< Ash fall intensity between 0 and 1

			float		mFireAnimationSpeed;
			float		mWaterAnimationSpeed;
			float		mVegetationAnimationSpeed;
			float		mVegetationAnimationFrequency;
			float		mVegetationAnimationWeight;
			float		mVegetationAnimationBending;
			float		mVegetationAnimationIndividuality;
			float		mVegetationAnimationCrosswind;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::WeatherState)
