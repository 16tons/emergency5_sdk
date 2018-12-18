// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <qsf/component/Component.h>
#include <qsf/asset/AssetProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class SequencePlayer;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Play sequence component class
		*/
		class QSF_LOGIC_API_EXPORT PlaySequenceComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;		///< "qsf::PlaySequenceComponent" unique component ID
			static const uint32 ACTIVE;				///< "Active" unique class property ID inside the class
			static const uint32 SEQUENCE;			///< "Sequence" unique class property ID inside the class
			static const uint32 LOOP;				///< "Loop" unique class property ID inside the class


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
			inline explicit PlaySequenceComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PlaySequenceComponent();

			/**
			*  @brief
			*    Return the sequence asset to play
			*
			*  @return
			*    The sequence asset to use (e.g. "sample/sequence/ambient/skilift") as animation source, do no destroy the returned instance
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline const AssetProxy& getSequence() const;

			/**
			*  @brief
			*    Set the sequence asset to play
			*
			*  @param[in] sequence
			*    The sequence asset to use (e.g. "sample/skeleton/character/female_angry_01") as animation source
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setSequence(const AssetProxy& skeleton);

			bool getLoop() const;
			void setLoop(bool loop);

			/**
			*  @brief
			*    Start or restart the sequence
			*
			*  @note
			*    - Usually only needed for restarting, as setting the sequence asset, loop flag or active also starts the sequence
			*    - Does nothing if not running or active
			*/
			void startSequence();

			void stopSequence();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetSimulating(bool simulating) override;
			virtual void onSetActive(bool active) override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateSequenceActive();
			void restartSequence();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			AssetProxy		mSequence;			///< Sequence assets to play
			bool			mLoop;

			// Internal only
			SequencePlayer* mSequencePlayer;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/component/PlaySequenceComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::PlaySequenceComponent)
