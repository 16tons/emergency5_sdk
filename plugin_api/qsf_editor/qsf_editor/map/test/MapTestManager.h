// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/plugin/pluginable/PluginableManager.h>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
	struct LogMessage;
	namespace editor
	{
		class MapTest;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Map test manager class
		*
		*  @remarks
		*    The map test manager is automatically able to use all CAMP classes which are derived from "qsf::editor::MapTest".
		*
		*    The simplest usage of the map test manager looks like the following
		*    @code
		*    MapTestManager mapTestManager(QSF_MAINMAP);
		*    mapTestManager.run(mapTestManager.getList<qsf::editor::MapTest>());
		*    @endcode
		*
		*  @note
		*    - Map tests directly use the QSF log system in order to give feedback
		*/
		class QSF_EDITOR_API_EXPORT MapTestManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public Boost signals                                  ]
		//[-------------------------------------------------------]
		public:
			boost::signals2::signal<void (const LogMessage&, const Entity*)> NewMessage;	///< This Boost signal is emitted as soon as a new message arrives, log message as first parameter, optional entity related to the text as second parameter (can be a null pointer)


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] map
			*    Map instance to perform the map tests on, the instance must stay valid as long as this map test manager instance exists
			*/
			explicit MapTestManager(const Map& map);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MapTestManager();

			/**
			*  @brief
			*    Return the map instance to perform the map tests on
			*
			*  @return
			*    The map instance to perform the map tests on, do no destroy the returned instance
			*/
			inline const Map& getMap() const;

			/**
			*  @brief
			*    Run map tests
			*
			*  @param[in] mapTests
			*    List of map test instances to run (the map tests should be owned by this map test manager, but that's not mandatory)
			*/
			void run(const std::vector<MapTest*>& mapTests);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const Map* mMap;	///< Map instance to perform the map tests on, always valid, do not destroy the instance


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/map/test/MapTestManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::MapTestManager)
