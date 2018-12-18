// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/NamedIdentifier.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class WeatherState;
	}
}
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Weather configuration class
		*
		*  @note
		*    - Buzzword "finite state machine": this is a very basic example of a FSM
		*/
		class QSF_GAME_API_EXPORT WeatherConfiguration : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/** Single transition entry */
			struct TransitionEntry
			{
				uint32		newWeatherId;
				float		changeFactor;
				std::string weatherName;
			};

			/** Array of transition entries */
			struct TransitionArray
			{
				std::vector<TransitionEntry> transitions;
				std::string					 transitionName;
			};

			/** Map of weather state ID to transition entry array */
			typedef std::unordered_map<uint32, TransitionArray> TransitionsMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			WeatherConfiguration() = delete;

			/**
			*  @brief
			*    Constructor
			*/
			explicit WeatherConfiguration(uint32 key);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			inline ~WeatherConfiguration();

			inline const std::string& getName() const;
			inline void setName(const std::string& name);

			inline uint32 getId() const;
			inline void setId(uint32 id);

			inline const std::string& getSource() const;
			inline void setSource(const std::string& source);

			/**
			*  @brief
			*    Return the start weather state
			*
			*  @return
			*    Pointer to the start weather, or null pointer if none was defined; do not destroy the instance
			*/
			const WeatherState* getStartWeatherState() const;

			/**
			*  @brief
			*    Randomly choose the next weather state
			*
			*  @param[in] currentWeatherId
			*    ID of the current weather state
			*
			*  @return
			*    Pointer to the next weather, or null pointer if none was found; do not destroy the instance
			*/
			const WeatherState* getNextWeatherState(uint32 currentWeatherId) const;

			inline TransitionsMap& getWeatherTransitions();

			/**
			*  @brief
			*    Load weather transitions from Boost property tree
			*
			*  @param[in] pTree
			*    Boost property tree to load the weather configuration from
			*
			*  @return
			*    "true" on success, "false" on failure
			*/
			bool loadFromBoostPTree(const boost::property_tree::ptree& pTree);

			/**
			*  @brief
			*    Save weather transitions to Boost property tree
			*
			*  @param[out] pTree
			*    Boost property tree to save the weather configuration to
			*
			*  @return
			*    "true" on success, "false" on failure
			*/
			bool saveToBoostPTree(boost::property_tree::ptree& pTree) const;

			//[-------------------------------------------------------]
			//[ Additional settings                                   ]
			//[-------------------------------------------------------]
			inline const std::string& getRainLightAudioSourceFile() const;
			inline const std::string& getRainHeavyAudioSourceFile() const;
			inline const std::string& getStormAudioSourceFile() const;
			inline const std::string& getLocalLightingPrefabAssetId() const;
			inline const std::string& getLocalRainPrefabAssetId() const;
			inline const std::string& getLocalStormPrefabAssetId() const;
			inline const std::string& getLocalSnowPrefabAssetId() const;
			inline const std::string& getLocalAshFallPrefabAssetId() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string		mName;				///< Name of this weather configuration
			uint32			mId;				///< Unique ID of this weather configuration
			std::string		mSource;			///< Source of this weather configuration, usually an UTF-8 virtual file name
			NamedIdentifier	mStartWeather;		///< Name and ID of the start weather
			TransitionsMap	mTransitionsMap;	///< Map of transitions; key is a weather ID, value the possible transitions from there
			// Additional settings
			std::string mRainLightAudioSourceFile;
			std::string mRainHeavyAudioSourceFile;
			std::string mStormAudioSourceFile;
			std::string mLocalLightingPrefabAssetId;
			std::string mLocalRainPrefabAssetId;
			std::string mLocalStormPrefabAssetId;
			std::string mLocalSnowPrefabAssetId;
			std::string mLocalAshFallPrefabAssetId;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/environment/weather/WeatherConfiguration-inl.h"
