// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	namespace ai
	{
		class WorldModel;
	}
	namespace editor
	{
		class StreetNetworkMapTest;
		namespace base
		{
			class CompoundOperation;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		/** This creates a lane world from all the manually edited parts of the street network and returns the result.
		* We collect all relevant components and convert them to an AI representation.
		* Function object that does all the work in the constructor already.
		* Only realized as a class for technical reasons so we can better split up the code in sub functions
		*
		* It has two basic use cases - as a map test or a real AI map creation.
		* The map test doesn't change anything and receives a map test instance, the conversion creates a new asset and changes properties of entities inside the map.
		* Therefore the latter version receives a compound operation instance which it fills up and expects to be pushed later on.
		* This operation should be pushed as not reversible because reverting it would mean also reverting the assert created / updated in here which is not trivial.
		* Therefore it is best if this operation is not visible to the user at all.
		*/
		class QSF_EDITOR_API_EXPORT StreetNetworkConverter : public boost::noncopyable
		{
		public:
			// This constructor starts a test run that doesn't change anything
			explicit StreetNetworkConverter(StreetNetworkMapTest& test);
			// Regular constructor that creates a world with the ID passed and assigns it to the reference parameter.
			// The Compound operation is filled with property changes and should be pushed as not reversible afterwards
			StreetNetworkConverter(const Map& map, unsigned int aiMapId, base::CompoundOperation& trafficLightConnectorOperation, std::auto_ptr<ai::WorldModel>& result);
			~StreetNetworkConverter();

		private:
			class Impl;
			Impl* mPImpl;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
