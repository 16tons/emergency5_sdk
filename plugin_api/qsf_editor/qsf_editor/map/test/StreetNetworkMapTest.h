// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/map/test/MapTest.h"

#include <qsf/reflection/CampDefines.h>


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
		* Tests the edited street network components for errors that would create problems for the automatic conversion to AI data.
		*/
		class StreetNetworkMapTest : public MapTest
		{
		public:
			QSF_DECLARE_CAMP_PLUGINABLE_ID;

			explicit StreetNetworkMapTest(MapTestManager* mapTestManager);

			// Reuse the protected print method as public method here as we call that from outside
			using MapTest::print;
		protected:
			virtual void run() override;

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline StreetNetworkMapTest::StreetNetworkMapTest(MapTestManager* mapTestManager) :
			MapTest(mapTestManager)
		{}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::StreetNetworkMapTest);
