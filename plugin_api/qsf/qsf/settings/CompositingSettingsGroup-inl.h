// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline CompositingSettingsGroup::CompositingSettingsGroup(SettingsGroupManager* settingsGroupManager) :
			SettingsGroup(settingsGroupManager),
			// General
			mCompositorQuality(HIGH_QUALITY),
			// Material system
			mReadMicrocodeCache(true),
			mWriteMicrocodeCache(true),
			// Shadow
			mShadows(true),
			mShadowTechnique(PSSM),
			mShadowFilter(PCF_3x3),
			mShadowDistance(130.0f),
			mShadowTextureSize(2048),	// Don't dare to go higher as default setting, it will cost to much fillrate
			mShadowFadeStart(0.8f),
			// PSSM shadows
			mPssmNumberOfSplits(PSSM_TWO),
			// CSM shadows
			mCsmShadowDistance(1200.0f),
			mCsmSplitShadowDistance(2500.0f),
			mCsmShadowTextureSize(2048),
			mCsmShadowFadeStart(1.0f),
			// Lighting, terrain, liquid, particles
			mLightingQuality(HIGH_QUALITY),
			mTerrainQuality(HIGH_QUALITY),
			mLiquidQuality(HIGH_QUALITY),
			mSoftParticles(true),
			// Post processing
			mFxaa(true),
			mDepthOfField(true)
		{
			// Nothing to do in here
		}

		inline CompositingSettingsGroup::~CompositingSettingsGroup()
		{
			// Nothing to do in here
		}

		inline CompositingSettingsGroup::Quality CompositingSettingsGroup::getCompositorQuality() const
		{
			return mCompositorQuality;
		}

		inline bool CompositingSettingsGroup::getReadMicrocodeCache() const
		{
			return mReadMicrocodeCache;
		}

		inline void CompositingSettingsGroup::setReadMicrocodeCache(bool readMicrocodeCache)
		{
			assignAndPromoteChange(mReadMicrocodeCache, readMicrocodeCache, READ_MICROCODE_CACHE);
		}

		inline bool CompositingSettingsGroup::getWriteMicrocodeCache() const
		{
			return mWriteMicrocodeCache;
		}

		inline void CompositingSettingsGroup::setWriteMicrocodeCache(bool writeMicrocodeCache)
		{
			assignAndPromoteChange(mWriteMicrocodeCache, writeMicrocodeCache, WRITE_MICROCODE_CACHE);
		}

		inline bool CompositingSettingsGroup::getShadows() const
		{
			return mShadows;
		}

		inline CompositingSettingsGroup::ShadowTechnique CompositingSettingsGroup::getShadowTechnique() const
		{
			return mShadowTechnique;
		}

		inline CompositingSettingsGroup::ShadowFilter CompositingSettingsGroup::getShadowFilter() const
		{
			return mShadowFilter;
		}

		inline float CompositingSettingsGroup::getShadowDistance() const
		{
			return mShadowDistance;
		}

		inline void CompositingSettingsGroup::setShadowDistance(float shadowDistance)
		{
			assignAndPromoteChange(mShadowDistance, shadowDistance, SHADOW_DISTANCE);
		}

		inline int CompositingSettingsGroup::getShadowTextureSize() const
		{
			return mShadowTextureSize;
		}

		inline void CompositingSettingsGroup::setShadowTextureSize(int shadowTextureSize)
		{
			assignAndPromoteChange(mShadowTextureSize, shadowTextureSize, SHADOW_TEXTURE_SIZE);
		}

		inline float CompositingSettingsGroup::getShadowFadeStart() const
		{
			return mShadowFadeStart;
		}

		inline void CompositingSettingsGroup::setShadowFadeStart(float shadowFadeStart)
		{
			assignAndPromoteChange(mShadowFadeStart, shadowFadeStart, SHADOW_FADE_START);
		}

		inline CompositingSettingsGroup::PssmNumberOfSplits CompositingSettingsGroup::getPssmNumberOfSplits() const
		{
			return mPssmNumberOfSplits;
		}

		inline void CompositingSettingsGroup::setPssmNumberOfSplits(PssmNumberOfSplits pssmNumberOfSplits)
		{
			assignAndPromoteChange(mPssmNumberOfSplits, pssmNumberOfSplits, PSSM_NUMBER_OF_SPLITS);
		}

		inline float CompositingSettingsGroup::getCsmShadowDistance() const
		{
			return mCsmShadowDistance;
		}

		inline void CompositingSettingsGroup::setCsmShadowDistance(float shadowDistance)
		{
			assignAndPromoteChange(mCsmShadowDistance, shadowDistance, CSM_SHADOW_DISTANCE);
		}

		inline float CompositingSettingsGroup::getCsmSplitShadowDistance() const
		{
			return mCsmSplitShadowDistance;
		}

		inline void CompositingSettingsGroup::setCsmSplitShadowDistance(float splitShadowDistance)
		{
			assignAndPromoteChange(mCsmSplitShadowDistance, splitShadowDistance, CSM_SPLIT_SHADOW_DISTANCE);
		}

		inline int CompositingSettingsGroup::getCsmShadowTextureSize() const
		{
			return mCsmShadowTextureSize;
		}

		inline void CompositingSettingsGroup::setCsmShadowTextureSize(int shadowTextureSize)
		{
			assignAndPromoteChange(mCsmShadowTextureSize, shadowTextureSize, CSM_SHADOW_TEXTURE_SIZE);
		}

		inline float CompositingSettingsGroup::getCsmShadowFadeStart() const
		{
			return mCsmShadowFadeStart;
		}

		inline void CompositingSettingsGroup::setCsmShadowFadeStart(float shadowFadeStart)
		{
			assignAndPromoteChange(mCsmShadowFadeStart, shadowFadeStart, CSM_SHADOW_FADE_START);
		}

		inline CompositingSettingsGroup::Quality CompositingSettingsGroup::getLightingQuality() const
		{
			return mLightingQuality;
		}

		inline CompositingSettingsGroup::Quality CompositingSettingsGroup::getTerrainQuality() const
		{
			return mTerrainQuality;
		}

		inline CompositingSettingsGroup::Quality CompositingSettingsGroup::getLiquidQuality() const
		{
			return mLiquidQuality;
		}

		inline bool CompositingSettingsGroup::getSoftParticles() const
		{
			return mSoftParticles;
		}

		inline bool CompositingSettingsGroup::getFxaa() const
		{
			return mFxaa;
		}

		inline void CompositingSettingsGroup::setFxaa(bool fxaa)
		{
			assignAndPromoteChange(mFxaa, fxaa, FXAA);
		}

		inline bool CompositingSettingsGroup::getDepthOfField() const
		{
			return mDepthOfField;
		}

		inline void CompositingSettingsGroup::setDepthOfField(bool depthOfField)
		{
			assignAndPromoteChange(mDepthOfField, depthOfField, DEPTH_OF_FIELD);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
