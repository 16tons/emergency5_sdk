// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/mode/CommandMode.h>

#include <qsf/reflection/CampClass.h>	// This include is not needed here directly, but most likely in a derived class
#include <qsf/base/WeakPtr.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}
namespace em5
{
	class BarrierTapeComponent;
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
	*    Place a tape barrier that stops persons.
	*
	*  @remarks
	*    Users place 2 poles which have a tape in between.
	*/
	class EM5_API_EXPORT PlaceBarrierTapeCommandMode : public qsf::game::CommandMode
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::PlaceBarrierTapeCommandMode" unique command pluginable ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlaceBarrierTapeCommandMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlaceBarrierTapeCommandMode();

		/**
		*  @brief
		*    Fill class with all necessary variables
		*/
		void initialize(qsf::Entity& caller);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual void shutdown() override;
		virtual bool executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds) override;
		virtual void updateCommandMode(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Find the position of the first hit in the scene which is _not_ the pole itself
		bool get3dPositionUnderMouse(const glm::vec2& normalizedMousePosition, glm::vec3& outPosition, BarrierTapeComponent* barrierTapeComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity*					   mCaller;
		qsf::Time						   mElapsedWaitTime;
		qsf::WeakPtr<BarrierTapeComponent> mBarrierTapeComponent;
		uint32							   mPreviewPole;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlaceBarrierTapeCommandMode)
