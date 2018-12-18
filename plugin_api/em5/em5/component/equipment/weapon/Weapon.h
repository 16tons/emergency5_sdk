// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace weapon
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		enum Weapon
		{
			NOFIGHT,		///< This is a special case to flag someone as not fighting or defending at all
			FIST,
			BASEBALLBAT,
			KNIFE,
			PISTOL,
			RIFLE,
			STONE,
			MOLOTOV,
			TORCH,			///< Used by medieval units
			SWORD,			///< Used by Guardsman in medieval maps
			PEPPERSPRAY,	///< Only used by units
			STUNGRENADE		///< Only used by units
		};

		enum class Category
		{
			CLOSE_COMBAT,
			FIREARM,
			THROWABLE,
			UNKNOWN
		};


		//[-------------------------------------------------------]
		//[ Public functions                                      ]
		//[-------------------------------------------------------]
		inline static Category getWeaponCategory(Weapon weapon)
		{
			switch (weapon)
			{
				case weapon::FIST:
				case weapon::BASEBALLBAT:
				case weapon::KNIFE:
				case weapon::PEPPERSPRAY:
				case weapon::TORCH:
				case weapon::SWORD:
					return Category::CLOSE_COMBAT;

				case weapon::PISTOL:
				case weapon::RIFLE:
					return Category::FIREARM;

				case weapon::STONE:
				case weapon::MOLOTOV:
				case weapon::STUNGRENADE:
					return Category::THROWABLE;
			}
			return Category::UNKNOWN;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // weapon
} // em5
