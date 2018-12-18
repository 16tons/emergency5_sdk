// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/UniqueInstance.h"
#include "qsf/settings/SettingsGroup.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Material;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF compositing settings group implementation
		*/
		class QSF_API_EXPORT CompositingSettingsGroup : public SettingsGroup, public UniqueInstance<CompositingSettingsGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;				///< "qsf::compositing::CompositingSettingsGroup" unique pluginable settings group ID
			// General
			static const uint32 COMPOSITOR_QUALITY;			///< "CompositorQuality" unique class property ID inside the class
			// Material system
			static const uint32 READ_MICROCODE_CACHE;		///< "ReadMicrocodeCache" unique class property ID inside the class
			static const uint32 WRITE_MICROCODE_CACHE;		///< "WriteMicrocodeCache" unique class property ID inside the class
			// Shadow
			static const uint32 SHADOWS;					///< "Shadows" unique class property ID inside the class
			static const uint32 SHADOW_TECHNIQUE;			///< "ShadowTechnique" unique class property ID inside the class
			static const uint32 SHADOW_FILTER;				///< "ShadowFilter" unique class property ID inside the class
			static const uint32 SHADOW_DISTANCE;			///< "ShadowDistance" unique class property ID inside the class
			static const uint32 SHADOW_TEXTURE_SIZE;		///< "ShadowTextureSize" unique class property ID inside the class
			static const uint32 SHADOW_FADE_START;			///< "ShadowFadeStart" unique class property ID inside the class
			// PSSM shadows
			static const uint32 PSSM_NUMBER_OF_SPLITS;		///< "PssmNumberOfSplits" unique class property ID inside the class
			// CSM shadows
			static const uint32 CSM_SHADOW_DISTANCE;		///< "CsmShadowDistance" unique class property ID inside the class
			static const uint32 CSM_SPLIT_SHADOW_DISTANCE;	///< "CsmSplitShadowDistance" unique class property ID inside the class
			static const uint32 CSM_SHADOW_TEXTURE_SIZE;	///< "CsmShadowTextureSize" unique class property ID inside the class
			static const uint32 CSM_SHADOW_FADE_START;		///< "CsmShadowFadeStart" unique class property ID inside the class
			// Lighting, terrain, liquid, particles
			static const uint32 LIGHTING_QUALITY;			///< "LightingQuality" unique class property ID inside the class
			static const uint32 TERRAIN_QUALITY;			///< "TerrainQuality" unique class property ID inside the class
			static const uint32 LIQUID_QUALITY;				///< "LiquidQuality" unique class property ID inside the class
			static const uint32 SOFT_PARTICLES;				///< "SoftParticles" unique class property ID inside the class
			// Post processing
			static const uint32 FXAA;						///< "Fxaa" unique class property ID inside the class
			static const uint32 DEPTH_OF_FIELD;				///< "DepthOfField" unique class property ID inside the class

			/**
			*  @brief
			*    Quality setting
			*/
			enum Quality
			{
				ULTRA_QUALITY  = 0,	///< Ultra quality
				HIGH_QUALITY   = 1,	///< High quality
				MEDIUM_QUALITY = 2,	///< Medium quality
				LOW_QUALITY	   = 3,	///< Low quality
				DEBUG_QUALITY  = 4	///< Debug quality: Meant for debugging with the simplest possible rendering
			};

			/**
			*  @brief
			*    Shadow technique
			*/
			enum ShadowTechnique
			{
				FOCUSED = 0,	///< Focused shadow mapping, one shadow map (performance)
				PSSM    = 1		///< Parallel Split Shadow Map (PSSM), three shadow map (quality)
			//	CSM	    = 2		///< Cascaded Shadow Map (CSM), four shadow maps (best quality)		// TODO(co) Port to OGRE 2.0
			};

			/**
			*  @brief
			*    Shadow filter
			*/
			enum ShadowFilter
			{
				NONE,		///< No filter
				PCF_2x2,	///< Standard quality. Very fast.
				PCF_3x3,	///< Good quality. Still quite fast on most modern hardware.
				PCF_4x4		///< High quality. Very slow in old hardware (e.g. DX10 level hardware and below).
			};

			/**
			*  @brief
			*    PSSM number of splits
			*/
			enum PssmNumberOfSplits
			{
				PSSM_TWO = 0,
				PSSM_THREE
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] settingsGroupManager
			*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
			*/
			inline explicit CompositingSettingsGroup(SettingsGroupManager* settingsGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CompositingSettingsGroup();

			//[-------------------------------------------------------]
			//[ General                                               ]
			//[-------------------------------------------------------]
			inline Quality getCompositorQuality() const;
			void setCompositorQuality(Quality quality);
			void applyCompositorQualityToMaterial(Material& material) const;

			//[-------------------------------------------------------]
			//[ Material system                                       ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Control reading of shader microcode from the material system cache folder
			*
			*  @note
			*    - Microcode is machine independent and loads very fast compared to regular compilation
			*    - The availability of this feature depends on the platform
			*/
			inline bool getReadMicrocodeCache() const;
			inline void setReadMicrocodeCache(bool readMicrocodeCache);
			inline bool getWriteMicrocodeCache() const;
			inline void setWriteMicrocodeCache(bool writeMicrocodeCache);

			//[-------------------------------------------------------]
			//[ Shadow                                                ]
			//[-------------------------------------------------------]
			inline bool getShadows() const;
			void setShadows(bool shadows);
			inline ShadowTechnique getShadowTechnique() const;
			void setShadowTechnique(ShadowTechnique shadowTechnique);
			inline ShadowFilter getShadowFilter() const;
			void setShadowFilter(ShadowFilter shadowFilter);
			inline float getShadowDistance() const;
			inline void setShadowDistance(float shadowDistance);
			inline int getShadowTextureSize() const;
			inline void setShadowTextureSize(int shadowTextureSize);
			inline float getShadowFadeStart() const;
			inline void setShadowFadeStart(float shadowFadeStart);
			// PSSM
			inline PssmNumberOfSplits getPssmNumberOfSplits() const;
			inline void setPssmNumberOfSplits(PssmNumberOfSplits pssmNumberOfSplits);
			// CSM
			inline float getCsmShadowDistance() const;
			inline void setCsmShadowDistance(float shadowDistance);
			inline float getCsmSplitShadowDistance() const;
			inline void setCsmSplitShadowDistance(float splitShadowDistance);
			inline int getCsmShadowTextureSize() const;
			inline void setCsmShadowTextureSize(int shadowTextureSize);
			inline float getCsmShadowFadeStart() const;
			inline void setCsmShadowFadeStart(float shadowFadeStart);

			//[-------------------------------------------------------]
			//[ Lighting, terrain, liquid, particles                  ]
			//[-------------------------------------------------------]
			inline Quality getLightingQuality() const;
			void setLightingQuality(Quality quality);
			void applyLightingQualityToMaterial(Material& material) const;
			inline Quality getTerrainQuality() const;
			void setTerrainQuality(Quality terrainQuality);
			void applyTerrainQualityToMaterial(Material& material) const;
			inline Quality getLiquidQuality() const;
			void setLiquidQuality(Quality liquidQuality);
			void applyLiquidQualityToMaterial(Material& material) const;
			inline bool getSoftParticles() const;
			void setSoftParticles(bool softParticles);
			void applySoftParticlesToMaterial(Material& material) const;

			//[-------------------------------------------------------]
			//[ Post processing                                       ]
			//[-------------------------------------------------------]
			inline bool getFxaa() const;
			inline void setFxaa(bool fxaa);
			inline bool getDepthOfField() const;
			inline void setDepthOfField(bool depthOfField);

			//[-------------------------------------------------------]
			//[ Internal                                              ]
			//[-------------------------------------------------------]
			void applyMaterialSettings() const;
			void applyCompositingMaterialSettings() const;
			void applyLightingMaterialSettings() const;
			void applyParticlesMaterialSettings() const;
			void applyShadowMaterialSettings() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			// General
			Quality mCompositorQuality;
			// Material system
			bool mReadMicrocodeCache;
			bool mWriteMicrocodeCache;
			// Shadow
			bool			mShadows;
			ShadowTechnique mShadowTechnique;
			ShadowFilter	mShadowFilter;
			float			mShadowDistance;
			int				mShadowTextureSize;
			float			mShadowFadeStart;
			// PSSM shadows
			PssmNumberOfSplits mPssmNumberOfSplits;
			// CSM shadows
			float mCsmShadowDistance;
			float mCsmSplitShadowDistance;
			int	  mCsmShadowTextureSize;
			float mCsmShadowFadeStart;
			// Lighting, terrain, liquid, particles
			Quality mLightingQuality;
			Quality mTerrainQuality;
			Quality mLiquidQuality;
			bool	mSoftParticles;
			// Post processing
			bool mFxaa;
			bool mDepthOfField;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/settings/CompositingSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::CompositingSettingsGroup)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::CompositingSettingsGroup::Quality)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::CompositingSettingsGroup::ShadowTechnique)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::CompositingSettingsGroup::ShadowFilter)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::CompositingSettingsGroup::PssmNumberOfSplits)
