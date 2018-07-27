// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/application/Application.h"
#include "qsf/time/Time.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RealTimeClock;
	class PlatformSystem;
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
	*    Application implementation with a real-time clock
	*/
	class QSF_API_EXPORT TimedApplication : public Application
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 APPLICATION_ID;	///< "qsf::TimedApplication" unique ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TimedApplication();

		/**
		*  @brief
		*    Return the real-time clock instance
		*
		*  @return
		*    The real-time clock instance, can be a null pointer in case "onStartup()" failed or was not called yet, do no destroy the returned instance
		*/
		inline RealTimeClock* getRealTimeClock() const;

		/**
		*  @brief
		*    Return the enforced frames per second (FPS) limitation
		*
		*  @return
		*    The enforced FPS limitation, 0 if there's no limitation
		*
		*  @remarks
		*    There are situations, like when playing a fullscreen video or having a fullscreen ingame menu, were one has pretty high
		*    frame rates (1000 > FPS). In such a situation, the hardware might start to make funny noises which people mislike or even get
		*    into panic. In such a situation, set a FPS limitation in order to keep the extreme-performance situation under control.
		*    "qsf::RendererSettingsGroup::VerticalSynchronizationEnabled" or the GPU driver might also add a limitation.
		*/
		inline float getFpsLimitation() const;

		/**
		*  @brief
		*    Set enforced frames per second (FPS) limitation
		*
		*  @param[in] fpsLimitation
		*    The enforced FPS limitation, 0 if there's no limitation
		*
		*  @see
		*    - "qsf::TimedApplication::setFpsLimitation()"
		*/
		void setFpsLimitation(float fpsLimitation);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Application methods               ]
	//[-------------------------------------------------------]
	public:
		virtual uint32 getId() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Application methods            ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the application
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Create the real-time clock render window, it stays valid until it's destroyed within "qsf::TimedApplication::onShutdown()"
		*/
		virtual bool onStartup() override;

		/**
		*  @brief
		*    Shutdown the application
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Destroy the real-time clock
		*
		*  @note
		*    - Only called in case "onStartup()" returned successfully
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		TimedApplication();

		/**
		*  @brief
		*    Update real-time clock
		*
		*  @remarks
		*    - By default, the default job manager is registered at the real-time clock and will also be updated with each clock update.
		*/
		void updateRealTimeClock();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		RealTimeClock* mRealTimeClock;	///< Real-time clock used to update the job system, always valid, we have to destroy the instance in case we no longer need it


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PlatformSystem& mPlatformSystem;			///< Platform system instance, don't destroy the instance
		float			mFpsLimitation;				///< The enforced FPS limitation, 0 if there's no limitation, "qsf::RendererSettingsGroup::VerticalSynchronizationEnabled" or the GPU driver might also add a limitation
		Time			mFpsLimitationPreviousTime;


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
#include "qsf/application/TimedApplication-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::TimedApplication)
