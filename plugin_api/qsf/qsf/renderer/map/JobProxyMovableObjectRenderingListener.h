// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/MovableObjectRenderingListener.h"
#include "qsf/job/JobProxy.h"


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
	*    Job proxy based OGRE movable object rendering listener
	*/
	class QSF_API_EXPORT JobProxyMovableObjectRenderingListener : public MovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline JobProxyMovableObjectRenderingListener();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JobProxyMovableObjectRenderingListener();

		/**
		*  @brief
		*    Return the nonvisible update timeout
		*
		*  @return
		*    The nonvisible update timeout
		*/
		inline const Time& getNonvisibleUpdateTimeout() const;

		/**
		*  @brief
		*    Set the nonvisible update timeout
		*
		*  @param[in] nonvisibleUpdateTimeout
		*    The nonvisible update timeout
		*/
		inline void setNonvisibleUpdateTimeout(const Time& nonvisibleUpdateTimeout);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*
		*  @note
		*    - The default implementation checks the time since last visible and puts the listener to sleep if required
		*/
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRendering(const Component& component, const Ogre::Camera& ogreCamera) override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		JobProxy mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Time mNonvisibleUpdateTimeout;		///< Nonvisible update timeout
		Time mTimeSinceLastVisible;			///< Time since last being visible
		uint32 mUpdatesSinceLastVisible;	///< How many updates since last being visible


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener-inl.h"
