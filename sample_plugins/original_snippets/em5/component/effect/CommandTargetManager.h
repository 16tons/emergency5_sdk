// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/manager/Manager.h>

#include <glm/fwd.hpp>

#include <vector>
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GridSceneManager;
}
namespace Ogre
{
	class ManualObject;
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
	*    EMERGENCY 5 command target manager
	*/
	class CommandTargetManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CommandTargetComponent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit CommandTargetManager(qsf::GridSceneManager& gridSceneManager);
		virtual ~CommandTargetManager();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Rent an OGRE manual object
		*
		*  @param[in] iconName
		*    Name of the icon to use
		*  @param[in] clipSpaceSize
		*    Clip space size
		*
		*  @return
		*    Rented OGRE manual object instance, null pointer on error, be polite and return the borrowed instance instead of destroying it
		*/
		Ogre::ManualObject* rentOgreManualObject(const std::string& iconName, const glm::vec2& clipSpaceSize);

		/**
		*  @brief
		*    Return a rented OGRE manual object
		*
		*  @param[in] ogreManualObject
		*    Rented OGRE manual object instance you wish to return in order to get your bail back (just kidding, there's no bail back)
		*/
		void returnRentedOgreManualObject(Ogre::ManualObject& ogreManualObject);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GridSceneManager&			 mGridSceneManager;
		std::vector<Ogre::ManualObject*> mFreeOgreManualObjects;			///< List of currently not used OGRE manual objects
		uint32							 mNumberOfCreatedOgreManualObjects;	///< Number of created OGRE manual objects


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
