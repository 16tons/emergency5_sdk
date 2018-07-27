// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline WeatherConfiguration::~WeatherConfiguration()
		{
			// Nothing here
		}

		inline const std::string& WeatherConfiguration::getName() const
		{
			return mName;
		}

		inline void WeatherConfiguration::setName(const std::string& name)
		{
			mName = name;
		}

		inline uint32 WeatherConfiguration::getId() const
		{
			return mId;
		}

		inline void WeatherConfiguration::setId(uint32 id)
		{
			mId = id;
		}

		inline const std::string& WeatherConfiguration::getSource() const
		{
			return mSource;
		}

		inline void WeatherConfiguration::setSource(const std::string& source)
		{
			mSource = source;
		}

		inline WeatherConfiguration::TransitionsMap& WeatherConfiguration::getWeatherTransitions()
		{
			return mTransitionsMap;
		}

		inline const std::string& WeatherConfiguration::getRainLightAudioSourceFile() const
		{
			return mRainLightAudioSourceFile;
		}

		inline const std::string& WeatherConfiguration::getRainHeavyAudioSourceFile() const
		{
			return mRainHeavyAudioSourceFile;
		}

		inline const std::string& WeatherConfiguration::getStormAudioSourceFile() const
		{
			return mStormAudioSourceFile;
		}

		inline const std::string& WeatherConfiguration::getLocalLightingPrefabAssetId() const
		{
			return mLocalLightingPrefabAssetId;
		}

		inline const std::string& WeatherConfiguration::getLocalRainPrefabAssetId() const
		{
			return mLocalRainPrefabAssetId;
		}

		inline const std::string& WeatherConfiguration::getLocalStormPrefabAssetId() const
		{
			return mLocalStormPrefabAssetId;
		}

		inline const std::string& WeatherConfiguration::getLocalSnowPrefabAssetId() const
		{
			return mLocalSnowPrefabAssetId;
		}

		inline const std::string& WeatherConfiguration::getLocalAshFallPrefabAssetId() const
		{
			return mLocalAshFallPrefabAssetId;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
