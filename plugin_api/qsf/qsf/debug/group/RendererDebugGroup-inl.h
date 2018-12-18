// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RendererDebugGroup::RendererDebugGroup(DebugGroupManager* debugGroupManager) :
		DebugGroup(debugGroupManager),
		mShowVertexNormals(false),
		mShowWireframes(false),
		mShowBones(false),
		mShowDebugMeshes(false),
		mShowDebugBoxes(false),
		mShowDebugBoxesFilled(true),
		mShowDebugBoxesDistance(30.0f),
		mShowDebugTexts(false),
		mShowParticlePlayback(true),
		mParticleFastForward(false),
		mDisableParticles(false),
		mDisableMultiThreadedParticlesUpdate(false)
	{
		// Nothing to do in here
	}

	inline RendererDebugGroup::~RendererDebugGroup()
	{
		// Nothing to do in here
	}

	inline bool RendererDebugGroup::getShowWireframes() const
	{
		return mShowWireframes;
	}

	inline bool RendererDebugGroup::getShowVertexNormals() const
	{
		return mShowVertexNormals;
	}

	inline void RendererDebugGroup::setShowVertexNormals(bool showVertexNormals)
	{
		assignAndPromoteChange(mShowVertexNormals, showVertexNormals, SHOW_VERTEX_NORMALS);
	}

	inline bool RendererDebugGroup::getShowBones() const
	{
		return mShowBones;
	}

	inline bool RendererDebugGroup::getShowDebugMeshes() const
	{
		return mShowDebugMeshes;
	}

	inline bool RendererDebugGroup::getShowDebugBoxes() const
	{
		return mShowDebugBoxes;
	}

	inline bool RendererDebugGroup::getShowDebugBoxesFilled() const
	{
		return mShowDebugBoxesFilled;
	}

	inline float RendererDebugGroup::getShowDebugBoxesDistance() const
	{
		return mShowDebugBoxesDistance;
	}

	inline bool RendererDebugGroup::getShowDebugTexts() const
	{
		return mShowDebugTexts;
	}

	inline bool RendererDebugGroup::getShowParticlePlayback() const
	{
		return mShowParticlePlayback;
	}

	inline bool RendererDebugGroup::getParticleFastForward() const
	{
		return mParticleFastForward;
	}

	inline bool RendererDebugGroup::getDisableParticles() const
	{
		return mDisableParticles;
	}

	inline bool RendererDebugGroup::getDisableMultiThreadedParticlesUpdate() const
	{
		return mDisableMultiThreadedParticlesUpdate;
	}

	inline void RendererDebugGroup::setDisableMultiThreadedParticlesUpdate(bool disableMultiThreadedParticlesUpdate)
	{
		assignAndPromoteChange(mDisableMultiThreadedParticlesUpdate, disableMultiThreadedParticlesUpdate, DISABLE_MULTITHREADED_PARTICLES_UPDATE);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
