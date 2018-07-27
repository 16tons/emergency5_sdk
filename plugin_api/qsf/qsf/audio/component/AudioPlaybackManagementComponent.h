// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/job/JobArguments.h"
#include "qsf/job/JobProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioSource;
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
	*    Audio playback management component class
	*
	*  @remarks
	*    Audio sources which are attached to the component are ultimately tied to the
	*    lifetime of the management component (i.e. when the componet is removed, all
	*    sources are stopped and destroyed). Furthermore, when a source stops playing,
	*    it will be automatically removed from the management and destroyed.
	*    Spatial sources will be moved with a potentially attached TransformComponent
	*    as long as it is active
	*/
	class QSF_API_EXPORT AudioPlaybackManagementComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::AudioPlaybackManagementComponent" unique component ID


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
		explicit AudioPlaybackManagementComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AudioPlaybackManagementComponent();

		/**
		*  @brief
		*    Attach a new source to the entity
		*
		*  @remarks
		*    The source will be managed by the AudioPlaybackManagementComponent, so be
		*    sure you don't access it as long as you're sure it won't go away by e.g.
		*    automatically stopping playing
		*
		*/
		void attachAudioSource(AudioSource* newSource, bool destroyWhenFinishedPlaying);
		void detachAudioSource(AudioSource* source);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		virtual void onUpdate(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		struct AttachedAudioSource
		{
			AudioSource* source;
			bool destroyWhenFinished;
		};

		std::vector<AttachedAudioSource> mAttachedSources;
		JobProxy						 mUpdateJob;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AudioPlaybackManagementComponent)
