// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline const AssetProxy& MultipleSensorsComponent::getSensorConfigurationsAsset() const
		{
			return mSensorConfigurationsAsset;
		}

		inline const boost::ptr_vector<SensorConfiguration>& MultipleSensorsComponent::getSensorConfigurations() const
		{
			return mSensorConfigurations;
		}

		inline uint32 MultipleSensorsComponent::getNumberOfSensorConfigurations() const
		{
			return static_cast<uint32>(mSensorConfigurations.size());
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
