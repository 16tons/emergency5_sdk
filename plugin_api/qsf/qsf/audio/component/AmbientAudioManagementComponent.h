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
#include "qsf/job/JobProxy.h"
#include "qsf/audio/AudioProxy.h"

#include <boost/container/flat_map.hpp>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioManager;
	class AmbientPolygonComponent;
	class Transform;
	namespace detail
	{
		// Hidden implementation, needs forward
		class AmbientSound;
	}
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
	*    Ambient audio system core component class
	*/
	class QSF_API_EXPORT AmbientAudioManagementComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::AmbientAudioManagementComponent" unique component ID
		static const uint32 ACTIVE;			///< "qsf::Active" unique component ID

		/**
		*  @brief
		*    Interface between ambient sound emitters and the actual playback of ambient sounds.
		*    Emitters get added to the ambient sound management which will then query them for
		*    sound volumes.
		*/
		class AmbientSoundEmitter : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    All data returned from computeDistanceToEmission()
			*/
			struct DistanceComputationResult
			{
				/**
				*  @brief
				*    Direction which points from the listener position onto the emission source
				*/
				glm::vec3 relativeEmissionDirection;
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Computes the distance of the world space position to the emission volume.
			*
			*  @return
			*    Returns a vector which points from the "worldSpacePosition" to the point of emission
			*/
			virtual void computeDistanceToEmission(const glm::vec3& worldSpacePosition, DistanceComputationResult& outResult) const = 0;

			/**
			*  @brief
			*    Fetches the audio asset which is emitted by this emitter (this must not
			*    change during the lifetime of the emitter! If it changes, remove and re-add
			*    the emitter to the manager)
			*/
			virtual const AssetProxy& getEmittedAudioAssetProxy() const = 0;

			/**
			*  @brief
			*    Return the current emitter volume between 0.0 and 1.0
			*/
			virtual float getVolume() const = 0;

			/**
			*  @brief
			*    Return the maximum emitter volume between 0.0 and 1.0 which can be reached
			*/
			virtual float getMaximumVolume() const
			{
				// Base implementation returns always 1.0f = maximum volume
				return 1.0f;
			}


		};

		/**
		*  @brief
		*    Map for quick lookups of the right AmbientSound which is playing a given audio asset
		*/
		typedef boost::container::flat_map<GlobalAssetId, detail::AmbientSound*> AmbientSoundLookup;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline AmbientAudioManagementComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AmbientAudioManagementComponent();

		/**
		*  @brief
		*    Adds a ambient sound emitter. The emitter must stay valid during the
		*    time it is added to the ambient audio manager.
		*
		*  @remarks
		*    The audio asset which the emitter emits must stay static during
		*    the time it is managed here. When the audio asset changes, the emitter
		*    must be re-added by first removing and then adding it again. This is
		*    for caching reasons.
		*/
		void addAmbientSoundEmitter(AmbientSoundEmitter& ambientSoundEmitter);

		/**
		*  @brief
		*    Removes a previously added emitter from the manager
		*/
		void removeAmbientSoundEmitter(AmbientSoundEmitter& ambientSoundEmitter);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Updates the playback of all ambience sounds for the
		*    transform of the audio listener
		*/
		void updateVolumesFromCameraTransform(const Transform& listenerTransformation);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobProxy			mUpdateJobProxy;		///< Job proxy for audio updates
		AmbientSoundLookup	mAmbientSoundLookup;	///< Storage for all ambient sounds which are active


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
#include "qsf/audio/component/AmbientAudioManagementComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AmbientAudioManagementComponent)
