// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/audio/AudioProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace server
		{
			class Host;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Extended audio proxy for easier multiplay usage
	*/
	class EM5_API_EXPORT AudioProxy : public qsf::AudioProxy
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AudioProxy();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AudioProxy();

		/**
		*  @brief
		*    Initiates a synchronization with the multiplay code (to synchronize the audio to the multiplayer clients)
		*
		*  @param[in] onlyDataUpdate
		*    Indicates if we should do only an data update (e.g. looping or context entity changed while playing) but no start/stop is handled with this
		*
		*  @note
		*    This method does nothing, when not in multiplayer context
		*/
		void synchronizeWithMultiplay(bool onlyDataUpdate = false);

		/**
		*  @brief
		*    Returns the id of this audio proxy
		*/
		const uint32 getId() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AudioProxy methods             ]
	//[-------------------------------------------------------]
	protected:
		virtual void onPlay() override;
		virtual void onStop() override;
		virtual void onContextChanged(qsf::Entity* contextEntity) override;
		virtual void onDetach() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		multiplayer::server::Host*	mMultiplayerHost;
		uint32						mId;
		bool						mIsStart;
		bool						mIsDetached;
		bool						mIsStop;
		uint64						mContextEntityId;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static uint32 mNextFreeId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
