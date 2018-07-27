// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/Serializer.h>

#include <boost/ptr_container/ptr_vector.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class SensorConfiguration;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Class to (de)serialize one or multiple sensor-configurations.
		*/
		class SensorConfigurationSerializer : public Serializer
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			static void deserialize(boost::ptr_vector<SensorConfiguration>& sensorConfigurations,const std::string& filename);
			static void serialize(const boost::ptr_vector<SensorConfiguration>& sensorConfigurations,const std::string& filename);

		};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
