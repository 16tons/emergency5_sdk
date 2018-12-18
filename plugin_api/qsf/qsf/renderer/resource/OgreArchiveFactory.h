// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreArchiveFactory.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF OGRE archive factory implementation class
	*
	*  @remarks
	*    We use the OGRE archive concept to redirect OGRE resource system requests into our QSF asset system.
	*
	*  @note
	*    - Based on http://misterblue.com/programming/dynamic-ogre-resources/ogre-archive-wrapper
	*/
	class QSF_API_EXPORT OgreArchiveFactory : public Ogre::ArchiveFactory
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class OgreResourceGroup;	// QSF OGRE archive factory is QSF internal only


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const Ogre::String OGRE_RESOURCE_TYPE;	///< The OGRE resource type "QsfOgreArchiveFactory" is used


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::FactoryObj methods               ]
	//[-------------------------------------------------------]
	public:
		virtual const Ogre::String& getType() const override;
		virtual Ogre::Archive* createInstance(const Ogre::String& name, bool readOnly) override;
		virtual Ogre::Archive* createInstance(const Ogre::String& name) override;
		virtual void destroyInstance(Ogre::Archive* ogreArchive) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		OgreArchiveFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreArchiveFactory();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
