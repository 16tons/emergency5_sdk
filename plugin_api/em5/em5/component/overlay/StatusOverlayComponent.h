// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/overlay/OverlayComponent.h"

#include <qsf/math/EnumBitmask.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Element;
	}
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
	*    EMERGENCY 5 status overlay component class
	*/
	class EM5_API_EXPORT StatusOverlayComponent : public OverlayComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "em5::StatusOverlayComponent" unique component ID
		static const uint32 STATUS_BAR_WIDTH;
		static const uint32 STATUS_BAR_ARROW_WIDTH;

		static const glm::vec2 WINDOW_SIZE;
		static const glm::vec2 WINDOW_OFFSET;

		static const glm::vec3 HEALTHBAR_WORLD_SPACE_OFFSET;

		static const float STATUS_BAR_ARROW_BLINK_PERIOD;
		static const float STATUS_BAR_FULL_TO_LOW_PERCENT;
		static const float STATUS_BAR_FULL_TO_VERY_LOW_PERCENT;

		static const std::string STATUS_FIRE_BAR_COLOR_FULL;
		static const std::string STATUS_FIRE_BAR_COLOR_LOW;

		static const std::string STATUS_HEALTH_BAR_COLOR_FULL;
		static const std::string STATUS_HEALTH_BAR_COLOR_LOW;

		static const std::string STATUS_LIFE_BAR_COLOR_FULL;
		static const std::string STATUS_LIFE_BAR_COLOR_LOW;

		static const std::string STATUS_DEATH_BAR_COLOR_FULL;
		static const std::string STATUS_DEATH_BAR_COLOR_LOW;

		static const std::string STATUS_PROGRESS_BAR_COLOR_FULL;
		static const std::string STATUS_PROGRESS_BAR_COLOR_LOW;

		static const std::string STATUS_HEALTH_VEHICLE_BAR_COLOR_FULL;
		static const std::string STATUS_HEALTH_VEHICLE_BAR_COLOR_LOW;
		static const std::string STATUS_HEALTH_VEHICLE_BAR_COLOR_VERY_LOW;

		enum BarType
		{
			BAR_TYPE_NONE = -1,
			BAR_TYPE_FIRE = 0,			///< Fire energy bar
			BAR_TYPE_HEALTH,			///< Health energy bar
			BAR_TYPE_SMALL_HEALTH,		///< Small health bar
			BAR_TYPE_LIFE,				///< Life energy bar
			BAR_TYPE_DEATH,				///< Deathfight energy bar
			BAR_TYPE_PROGRESS,			///< Progress bar
			_BAR_COUNT
		};

		enum StatusIconType
		{
			STATUS_ICON_UNDEFINED = -1,			///< Icon is not registered with icon symbol
			STATUS_ICON_ARSONIST,
			STATUS_ICON_ATTACKED,
			STATUS_ICON_BOMB,
			STATUS_ICON_CHEMICAL_CONTAMINATION,
			STATUS_ICON_CONDITION_RESCUE_AXE,
			STATUS_ICON_CONDITION_RESCUE_DLK,
			STATUS_ICON_CONDITION_RESCUE_HELI,
			STATUS_ICON_DEAD,
			STATUS_ICON_DEFECT,
			STATUS_ICON_DEFECT_VEHICLE,
			STATUS_ICON_EVENT_VARIOUS,
			STATUS_ICON_FANATIC,
			STATUS_ICON_GAPER,
			STATUS_ICON_GANGSTER,
			STATUS_ICON_GANGSTER_IN_VEHICLE,
			STATUS_ICON_GUNFIRE,
			STATUS_ICON_HEAT_HAZARD,
			STATUS_ICON_INFECTED,
			STATUS_ICON_NPC,
			STATUS_ICON_POSSIBLY_INFECTED,
			STATUS_ICON_POSSIBLY_INFECTED_BUILDING,
			STATUS_ICON_SEARCH,
			STATUS_ICON_SEARCH_VARIOUS,
			STATUS_ICON_RADIOACTIVE_CONTAMINATION,
			STATUS_ICON_BIOLOGICAL_CONTAMINATION,
			STATUS_ICON_SICK,
			STATUS_ICON_SICK_CONTAGIOUS,
			STATUS_ICON_SMOKE_POISONING,
			STATUS_ICON_STAKE,
			STATUS_ICON_TREE,
			STATUS_ICON_WARBLERTWEET,
			STATUS_ICON_WELL,
			STATUS_ICON_WITCH,
			STATUS_ICON_WRENCH,
			_STATUS_ICON_COUNT
		};

		struct Bar
		{
			float mProgress;
			float mLastProgress;
			float mShownProgress;
			int mNumberOfArrows;
			int mDirection;
		};
		typedef boost::container::flat_map<BarType, Bar> BarMap;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		// TODO(sw) Needed for multiplay so that we add the correct entity to the cache for the status overlay component
		static qsf::Entity& getEntityOrChildWithMesh(qsf::Entity& entity);

		/**
		*  @param[in] progress has range [0-1] where 1 is a full bar
		*/
		static void showBarOverlay(qsf::Entity& overlayOwnerEntity, BarType barType, float progress = 1.0f);
		static void hideBarOverlay(qsf::Entity& overlayOwnerEntity, BarType barType);

		static void showIcon(qsf::Entity& overlayOwnerEntity, StatusIconType statusIconType, bool show = true);
		static void hideIcon(qsf::Entity& overlayOwnerEntity, StatusIconType statusIconType);


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
		explicit StatusOverlayComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StatusOverlayComponent();

		void showBarOverlay(BarType barType, float progress);
		void hideBarOverlay(BarType barType);

		void showIcon(StatusIconType statusIconType, bool enabled = true);
		void hideIcon(StatusIconType statusIconType);

		// Needed for multiplay
		inline uint64 getIconFlags() const;
		inline const BarMap& getBarValues() const;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::OverlayComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual bool isContentEmpty() const override;
		virtual void updateContent(Rocket::Core::ElementDocument& document) override;
		virtual const glm::vec3& getContentWorldSpaceOffset() const override;
		virtual const glm::vec2& getContentWindowSize() const override;
		virtual const glm::vec2& getContentWindowOffset() const override;
		virtual void updateAnimation(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void initializeIconInfoMap();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void centerStatusIcons(Rocket::Core::Element& element);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef qsf::EnumBitmask<uint64, StatusIconType> StatusIconFlags;

		struct IconInfo
		{
			std::string mStyleSheetClass;		///< The used stylesheet class in the rml file
			std::string mTooltipId;				///< Localization text ID of the tooltip
		};
		typedef boost::container::flat_map<StatusIconType, IconInfo> IconInfoMap;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static IconInfoMap mIconInfoMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BarMap			mBarMap;
		StatusIconFlags mStatusIconFlags;
		qsf::Time		mTimePassed;		///< Time passed since job proxy was started
		qsf::Time		mLastUpdateTime;
		bool			mBarBackgroundsVisible;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/overlay/StatusOverlayComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::StatusOverlayComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::StatusOverlayComponent::StatusIconType)
