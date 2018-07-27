// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/component/Component.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
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
	*    Marker component class
	*
	*  @remarks
	*    This component replaces the OGRE sub-entities material with a transparent copy. Over the time,
	*    the transparent material fades in/out. When the component is shutdown, the original OGRE sub-entities
	*    material will be restored.
	*
	*  @note
	*    - Not derived from "qsf::RendererComponent" by intent because it's logic update and nothing directly visible on the screen
	*
	*  @todo
	*    - TODO(co) For now, only the active state of the renderer component is toggled. It would be nicer to have a transparent pulsating material.
	*/
	class QSF_API_EXPORT MarkerComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::MarkerComponent" unique component ID
		static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
		static const uint32 SPEED;			///< "Speed" unique class property ID inside the class


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
		inline explicit MarkerComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MarkerComponent();

		/**
		*  @brief
		*    Return the animation playback speed
		*
		*  @return
		*    Animation playback speed, 1 means normal, <1 slower, 1> faster
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getSpeed() const;

		/**
		*  @brief
		*    Set animation playback speed
		*
		*  @param[in] speed
		*    Animation playback speed, 1 means normal, <1 slower, 1> faster
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setSpeed(float speed);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the OGRE entity instance
		*
		*  @return
		*    The OGRE entity instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the OGRE entity instance if you don't have to
		*/
		inline Ogre::v1::Entity* getOgreEntity() const;


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
		*    Set the visibility of the OGRE scene node
		*
		*  @param[in] visible
		*    "true" to show the OGRE scene node, "false" to hide it
		*/
		void setOgreSceneNodeVisible(bool visible);

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float mSpeed;	///< Animation playback speed, 1 means normal, <1 slower, 1> faster
		// Internal only
		Ogre::v1::Entity* mOgreEntity;		///< OGRE entity instance, can be a null pointer, just shared, do not destroy it
		float			  mTime;			///< Internal time
		bool			  mVisible;			///< Current visibility state
		JobProxy		  mUpdateJobProxy;	///< Job proxy
		bool			  mPreviousActive;	///< Previous active state of the renderer component


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
#include "qsf/renderer/utility/MarkerComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MarkerComponent)
