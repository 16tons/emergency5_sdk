// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/fabricator/PrototypeFabricator.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class ScriptCompilerListener;
}
namespace qsf
{
	namespace editor
	{
		class ScriptCompilerListener;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract OGRE prototype fabricator base class
		*/
		class OgrePrototypeFabricator : public PrototypeFabricator
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::OgrePrototypeFabricator" unique pluginable prototype fabricator ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OgrePrototypeFabricator();


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototypeFabricatorManager
			*    The prototype fabricator manager this prototype fabricator is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit OgrePrototypeFabricator(PrototypeFabricatorManager* prototypeFabricatorManager);

			/**
			*  @brief
			*    Register OGRE script compiler listener
			*/
			void registerScriptCompilerListener() const;

			/**
			*  @brief
			*    Unregister OGRE script compiler listener
			*/
			void unregisterScriptCompilerListener() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			mutable Ogre::ScriptCompilerListener* mPreviousOgreScriptCompilerListener;	///< Previous OGRE script compiler listener, can be a null pointer, do not destroy the instance
			mutable ScriptCompilerListener*		  mScriptCompilerListener;				///< Script compiler listener, can be a null pointer, we have to destroy the instance in case we no longer need it


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::OgrePrototypeFabricator)
