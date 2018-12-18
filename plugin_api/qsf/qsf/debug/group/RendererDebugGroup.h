// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/group/DebugGroup.h"
#include "qsf/base/UniqueInstance.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
	*    Renderer debug group implementation
	*/
	class QSF_API_EXPORT RendererDebugGroup : public DebugGroup, public UniqueInstance<RendererDebugGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;							///< "qsf::RendererDebugGroup" unique pluginable debug group ID
		static const uint32 SHOW_WIREFRAMES;						///< "ShowWireframes" unique class property ID inside the class
		static const uint32 SHOW_VERTEX_NORMALS;					///< "ShowVertexNormals" unique class property ID inside the class
		static const uint32 SHOW_BONES;								///< "ShowBones" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_MESHES;						///< "ShowDebugMeshes" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_BOXES;						///< "ShowDebugBoxes" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_BOXES_FILLED;				///< "ShowDebugBoxesFilled" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_BOXES_DISTANCE;				///< "ShowDebugBoxesDistance" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_TEXTS;						///< "ShowDebugTexts" unique class property ID inside the class
		static const uint32 SHOW_PARTICLE_PLAYBACK;					///< "ShowParticlePlayback" unique class property ID inside the class
		static const uint32 PARTICLE_FASTFORWARD;					///< "ParticleFastForward" unique class property ID inside the class
		static const uint32 DISABLE_PARTICLES;						///< "DisableParticles" unique class property ID inside the class
		static const uint32 DISABLE_MULTITHREADED_PARTICLES_UPDATE;	///< "DisableMultiThreadedParticlesUpdate" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] debugGroupManager
		*    Optional pointer to the debug group manager this debug group should be registered to, can be a null pointer
		*/
		explicit RendererDebugGroup(DebugGroupManager* debugGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RendererDebugGroup();

		/**
		*  @brief
		*    Return whether or not wireframes should be shown
		*/
		inline bool getShowWireframes() const;

		/**
		*  @brief
		*    Set whether or not wireframes should be shown
		*/
		void setShowWireframes(bool showWireframes);

		/**
		*  @brief
		*    Return whether or not vertex normals have debug drawing
		*/
		inline bool getShowVertexNormals() const;

		/**
		*  @brief
		*    Set whether or not vertex normals have debug drawing
		*/
		inline void setShowVertexNormals(bool showVertexNormals);

		/**
		*  @brief
		*    Return whether or not bones of meshes have debug drawing
		*/
		inline bool getShowBones() const;

		/**
		*  @brief
		*    Set whether or not bones of meshes have debug drawing
		*/
		void setShowBones(bool showBones);

		/**
		*  @brief
		*    Return whether or not to show debug meshes
		*/
		inline bool getShowDebugMeshes() const;

		/**
		*  @brief
		*    Set whether or not to show debug meshes
		*/
		void setShowDebugMeshes(bool showDebugMeshes);

		/**
		*  @brief
		*    Return whether or not to show debug boxes
		*/
		inline bool getShowDebugBoxes() const;

		/**
		*  @brief
		*    Set whether or not to show debug boxes
		*/
		void setShowDebugBoxes(bool showDebugBoxes);

		/**
		*  @brief
		*    Return whether or not to show debug boxes filled
		*/
		inline bool getShowDebugBoxesFilled() const;

		/**
		*  @brief
		*    Set whether or not to show debug boxes filled
		*/
		void setShowDebugBoxesFilled(bool showDebugBoxesFilled);

		/**
		*  @brief
		*    Return the debug boxes rendering distance
		*/
		inline float getShowDebugBoxesDistance() const;

		/**
		*  @brief
		*    Set the debug boxes rendering distance
		*/
		void setShowDebugBoxesDistance(float showDebugBoxesDistance);

		/**
		*  @brief
		*    Return whether or not to show debug texts
		*/
		inline bool getShowDebugTexts() const;

		/**
		*  @brief
		*    Set whether or not to show debug texts
		*/
		void setShowDebugTexts(bool showDebugTexts);

		/**
		*  @brief
		*    Return whether or not the particle playback is active
		*/
		inline bool getShowParticlePlayback() const;

		/**
		*  @brief
		*    Set whether or not to show particle playback
		*/
		void setShowParticlePlayback(bool showParticlePlayback);

		/**
		*  @brief
		*    Return whether or not the particle fastforward is active
		*/
		inline bool getParticleFastForward() const;

		/**
		*  @brief
		*    Set whether or not to show particle playback
		*/
		void setParticleFastForward(bool particleFastForward);

		/**
		*  @brief
		*    Return whether or not the particles are disabled
		*/
		inline bool getDisableParticles() const;

		/**
		*  @brief
		*    Set whether or not the particles are disabled
		*/
		void setDisableParticles(bool disableParticles);

		inline bool getDisableMultiThreadedParticlesUpdate() const;
		inline void setDisableMultiThreadedParticlesUpdate(bool disableMultiThreadedParticlesUpdate);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void refreshActiveStateForComponentsById(const Map& map, uint32 componentId, bool active);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		bool  mShowVertexNormals;					///< "true" if show vertex normals is enabled, else "false"
		bool  mShowWireframes;						///< "true" if show wireframes is enabled, else "false"
		bool  mShowBones;							///< "true" if show bone rendering is enabled, else "false"
		bool  mShowDebugMeshes;						///< "true" if show debug mesh rendering is enabled, else "false"
		bool  mShowDebugBoxes;						///< "true" if show debug box rendering is enabled, else "false"
		bool  mShowDebugBoxesFilled;				///< "true" if show debug box filled rendering is enabled, else "false"
		float mShowDebugBoxesDistance;				///< Debug boxes rendering distance
		bool  mShowDebugTexts;						///< "true" if show debug text rendering is enabled, else "false"
		bool  mShowParticlePlayback;				///< "true" if the particle update is enabled, else "false"
		bool  mParticleFastForward;					///< "true" if the particle fast forward is enabled, else "false"
		bool  mDisableParticles;					///< "true" if the particles are disabled, else "false"
		bool  mDisableMultiThreadedParticlesUpdate;


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
#include "qsf/debug/group/RendererDebugGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererDebugGroup)
