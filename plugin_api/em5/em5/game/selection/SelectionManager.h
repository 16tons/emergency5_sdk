// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/selection/EntitySelectionManager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
	*    EMERGENCY 5 selection manager class
	*/
	class EM5_API_EXPORT SelectionManager : public qsf::EntitySelectionManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline SelectionManager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SelectionManager();

		/**
		*  @brief
		*    Serialization of the selection
		*/
		inline void serialize(qsf::BinarySerializer& serializer);

		void startup();
		void shutdown();

		/**
		*  @brief
		*    Enable/disable the automatic selection of spawned units; default is enabled
		*/
		void setAutomaticSpawnedUnitSelection(bool automaticSelection);

		// TODO(mk) Override the setSelectionById() to emit the message "Messages::EM5_CHANGE_SELECTION". So we are able to detect all selection changes automatically
		// TODO(co) Removed "override" since those are not virtual methods. Fixed broken build. Since those base classes please don't make those virtual without talking to me.
		void setSelectionById(uint64 id);
		void setSelectionByIdSet(const boost::container::flat_set<uint64>& idSet);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onSelectionChange();
		void onUnitSpawnedPlayerUpdateSelection(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::MessageProxy mUnitSpawnedPlayerUpdateSelectionMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/selection/SelectionManager-inl.h"
