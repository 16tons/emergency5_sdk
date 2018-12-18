// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/game/units/Department.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/reflection/type/CampBoostPTree.h>
#include <qsf/base/NamedIdentifier.h>

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class VehicleComponent;
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
	*    Order information
	*
	*  @remarks
	*    The order information class describes an ingame entity the player can order, this is (usually) selectable and commandable.
	*    This can be a vehicle, helicopter, ship or a person. In the future maybe buildings and equipment.
	*    This has icons for representing in the GUI.
	*/
	class EM5_API_EXPORT OrderInfo : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::pair<std::string, uint32>					OrderPair;
		typedef boost::container::flat_map<std::string, uint32>	OrderPairList;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void getOrderPairListByVehicleComponent(const VehicleComponent& vehicleComponent, OrderPairList& orderPairList);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OrderInfo(uint32 key);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OrderInfo();

		/**
		*  @brief
		*    Get unique order information ID, which is the hashed name
		*
		*  @note
		*    - Elsewhere, there order info ID is referred to as "unit type ID", but this is the same thing
		*/
		uint32 getOrderInfoId() const;

		/**
		*  @brief
		*    Get name of the order information, e.g. "RW"
		*
		*  @note
		*    - The order info ID (a.k.a. "unit type ID") is directly related to this name, as it is the name's hash
		*/
		const std::string& getName() const;

		/**
		*  @brief
		*    Set name of the order information
		*/
		void setName(const std::string& name);

		/**
		*  @brief
		*    Return the prefab asset to use
		*
		*  @return
		*    The prefab asset to use (e.g. "sample/prefab/animal/squirrel"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		const qsf::AssetProxy& getPrefab() const;

		/**
		*  @brief
		*    Set the prefab asset to use
		*
		*  @param[in] prefab
		*    The prefab asset to use (e.g. "sample/prefab/animal/squirrel")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setPrefab(const qsf::AssetProxy& prefab);

		/**
		*  @brief
		*    Get the localized name ID
		*
		*  @note
		*    - This is a name for display to the user; note that this is only the text ID, which still needs to be translated with e.g. QSF_TRANSLATE_STRING
		*/
		const std::string& getLocalizedName() const;

		/**
		*  @brief
		*    Set the localized name ID
		*/
		void setLocalizedName(const std::string& name);

		/**
		*  @brief
		*    Get the department
		*/
		units::Department getDepartment() const;

		/**
		*  @brief
		*    Get the department name
		*/
		const std::string& getDepartmentAsString() const;

		/**
		*  @brief
		*    Set the department
		*/
		void setDepartment(units::Department department);

		/**
		*  @brief
		*    Set the department by name
		*/
		void setDepartmentAsString(const std::string& department);

		/**
		*  @brief
		*    Get unit tags as a single string
		*
		*  @note
		*    - There can be multiple unit tags for an order info, which get merged into a single, space-separated string, e.g. "Firefighters Vehicle Road RW"
		*/
		std::string getUnitTagsAsString() const;

		/**
		*  @brief
		*    Set unit tags by a single string
		*
		*  @note
		*    - The unit tags string is expected as space-separated string, e.g. "Firefighters Vehicle Road RW"
		*/
		void setUnitTagsAsString(const std::string& unitTags);

		/**
		*  @brief
		*    Check for presence of a specific unit tag
		*/
		bool hasUnitTag(const qsf::StringHash& tag) const;

		/**
		*  @brief
		*    Get name space of the order information
		*/
		uint32 getNameSpace() const;

		/**
		*  @brief
		*    Set name space of the order information
		*/
		void setNameSpace(uint32 space);

		/**
		*  @brief
		*    Return the avatar icon asset to use
		*
		*  @return
		*    The avatar icon asset to use (e.g. "sample/texture/animal/squirrel"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		const std::string& getAvatarIcon() const;

		/**
		*  @brief
		*    Set the avatar icon name to use
		*
		*  @param[in] name
		*    The avatar icon name to use
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAvatarIcon(const std::string& name);

		/**
		*  @brief
		*    Return the selection icon name to use
		*
		*  @return
		*    The selection icon name to use, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		const std::string& getSelectionIcon() const;

		/**
		*  @brief
		*    Set the selection icon name to use
		*
		*  @param[in] name
		*    The selection icon name to use
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSelectionIcon(const std::string& name);

		/**
		*  @brief
		*    Get the mini map icon type
		*/
		const std::string& getMiniMapIconType() const;

		/**
		*  @brief
		*    Set the mini map icon type
		*/
		void setMiniMapIconType(const std::string& miniMapIconType);

		/**
		*  @brief
		*    Return the order cost of the unit in ingame credits
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		uint32 getOrderCost() const;

		/**
		*  @brief
		*    Set the order cost of the unit in ingame credits
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setOrderCost(uint32 orderCost);

		/**
		*  @brief
		*    Return the order speed of the unit in seconds // TODO(mk) find a good value for speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		int getOrderSpeed() const;

		/**
		*  @brief
		*    Set the order speed of the unit in seconds
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setOrderSpeed(int orderSpeed);

		/**
		*  @brief
		*    Return the amount of credits it costs to unlock the unit
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		uint32 getUnlockCost() const;

		/**
		*  @brief
		*    Set the amount of credits it cost to unlock the unit
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setUnlockCost(uint32 unlockCost);

		/**
		*  @brief
		*    Return the maximum number of units are available for the player
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		uint32 getNumberAvailable() const;

		/**
		*  @brief
		*    Set the maximum number of units are available for the player
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setNumberAvailable(uint32 numberAvailable);

		/**
		*  @brief
		*    Return the maximum number of controllable personnel, like medics or firefighters of the unit
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		uint32 getPersonnelSpace() const;

		/**
		*  @brief
		*    Set the maximum number of controllable personnel, like medics or firefighters of the unit
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setPersonnelSpace(uint32 personnelSpace);

		/**
		*  @brief
		*    Return the maximum number of passengers, like injured persons or gangsters
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		uint32 getPassengerSpace() const;

		/**
		*  @brief
		*    Set the maximum number of passengers of the unit
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setPassengerSpace(uint32 passengerSpace);

		/**
		*  @brief
		*    Read the start personnel from pTree
		*/
		void setStartPersonnel(const boost::property_tree::ptree& pTree);

		/**
		*  @brief
		*    Return the pTree of the start personnel, needed for loading data from .json; use "getStartPersonnelList()" instead
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		const boost::property_tree::ptree& getStartPersonnel() const;

		/**
		*  @brief
		*    Return the list with start personnel (name / number)
		*/
		const OrderPairList& getStartPersonnelList() const;

		/**
		*  @brief
		*    Return the list with start personnel (name / number)
		*/
		OrderPairList& getStartPersonnelListForWriting();

		/**
		*  @brief
		*    Return the pTree of the supported personnel, needed for loading data from .json
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		std::string getSupportedPersonnelAsString() const;

		/**
		*  @brief
		*    Read the supported personnel from a single, space-separated string
		*/
		void setSupportedPersonnelAsString(const std::string& supported);

		/**
		*  @brief
		*    Check whether this is a vehicle and if a given type of unit persons can enter this
		*
		*  @remarks
		*    This checks both standard personnel and supported personnel properties
		*    and returns "true" if the given personnel unit name is listed in any of these
		*/
		bool supportsPersonnel(const std::string& personnelUnitTypeName) const;

		/**
		*  @brief
		*    Returns the speaker index which should be used for e.g. the voice feedback
		*/
		uint32 getSpeakerIndex() const;

		/**
		*  @brief
		*    Set the speaker index which should be used for e.g. the voice feedback
		*/
		void setSpeakerIndex(uint32 speakerIndex);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<std::string> UnitNameArray;
		typedef std::vector<qsf::NamedIdentifier> UnitTagArray;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		std::vector<std::string> mHelpPages;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General
		qsf::NamedIdentifier mName;				///< Unit name and ID of the order information
		qsf::AssetProxy		 mPrefab;			///< Prefab asset for the unit

		// Metadata
		std::string		  mLocalizedName;		///< Localization ID of the unit name
		units::Department mDepartment;			///< Department this unit belongs to
		UnitTagArray	  mUnitTags;			///< List of unit tags; please don't make a set out of this, as this would mean losing the order of elements

		// GUI-related
		std::string		mAvatarIcon;			///< Icon graphics, displayed in the GUI
		std::string		mSelectionIcon;
		std::string		mMiniMapIconType;
		uint32			mNameSpace;				///< Needed inside the selection window for generating space between the icon and name

		// Ordering
		uint32			mOrderCost;
		int				mOrderSpeed;
		uint32			mUnlockCost;			///< Amount of credits it costs to unlock this unit
		uint32			mNumberAvailable;

		// Personnel
		uint32			mPersonnelSpace;		///< Units controlled by player
		uint32			mPassengerSpace;		///< Entities like injured civils or gangster
		OrderPairList	mStandardPersonnel;		///< Unit types of persons to be placed in the vehicle by default
		boost::property_tree::ptree mStartPersonnelPTree;
		std::vector<qsf::NamedIdentifier> mSupportedPersonnel;	// Unit types of persons that may enter the vehicle in addition to the standard personnel

		uint32			mSpeakerIndex;			///< Speaker index, e.g. used by the voice feedback


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::OrderInfo)
