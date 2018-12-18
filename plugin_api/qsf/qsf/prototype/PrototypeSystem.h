// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/base/idgenerator/ConsecutiveIdGenerator.h"

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PrototypeManager;
	class PrototypeFinalizerManager;
	class PrefabCache;
}


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
	*    Prototype system class
	*
	*  @note
	*    - A prototype system is a collection of prototype managers
	*/
	class QSF_API_EXPORT PrototypeSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<PrototypeManager*>						  ManagerList;
		typedef boost::container::flat_map<uint32, PrototypeManager*> ManagerMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PrototypeSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrototypeSystem();

		//[-------------------------------------------------------]
		//[ Main prototype manager                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the main prototype manager instance
		*
		*  @return
		*    The main prototype manager instance, do not destroy the instance
		*/
		inline PrototypeManager& getMainManager() const;

		/**
		*  @brief
		*    Set the main prototype manager instance by using its unique ID
		*
		*  @param[in] id
		*    Unique prototype manager ID
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool setMainManager(uint32 id);

		//[-------------------------------------------------------]
		//[ Management of prototype managers                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new prototype manager instance
		*
		*  @return
		*    The created prototype manager instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique prototype manager ID instead)
		*/
		PrototypeManager* createManager();

		/**
		*  @brief
		*    Create a new prototype manager instance with an enforced given unique prototype manager identifier
		*
		*  @param[in] id
		*    The unique prototype manager identifier
		*
		*  @return
		*    The created prototype manager instance, null pointer on error (e.g. ID already used), do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique prototype manager ID instead)
		*
		*  @note
		*    - Use "createManager()" instead of "createManagerWithId()" whenever possible, do not enforce unique prototype manager identifier if you can avoid it
		*/
		PrototypeManager* createManagerWithId(uint32 id);

		/**
		*  @brief
		*    Destroy a prototype manager instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique prototype manager identifier
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown prototype manager identifier?)
		*/
		bool destroyManagerById(uint32 id);

		/**
		*  @brief
		*    Destroy all prototype managers within this prototype system
		*/
		void destroyAllManagers();

		/**
		*  @brief
		*    Return a prototype manager instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique prototype manager identifier
		*
		*  @return
		*    The requested prototype manager instance, null pointer on error (maybe the ID is unknown?), do not destroy the instance
		*/
		inline PrototypeManager* getManagerById(uint32 id) const;

		/**
		*  @brief
		*    Return the prototype manager list
		*
		*  @return
		*    Reference to the internal prototype manager list, do not manipulate the list or destroy the prototype managers
		*/
		inline const ManagerList& getManagers() const;

		//[-------------------------------------------------------]
		//[ Prototype ID generation                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Generate a new prototype Id
		*
		*  @return
		*    The new prototype ID; you have to make sure yourself that it is no duplicate of an existing ID inside the concrete prototype manager
		*/
		uint64 generatePrototypeId();

		//[-------------------------------------------------------]
		//[ Prototype finalizers                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the prototype finalizer manager
		*
		*  @return
		*    Reference to the internal prototype finalizer manager, do not destroy the instance
		*/
		inline const PrototypeFinalizerManager& getPrototypeFinalizerManager() const;

		//[-------------------------------------------------------]
		//[ Prefab cache                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the prefab cache
		*
		*  @return
		*    Reference to the internal prefab cache, do not destroy the instance
		*/
		inline PrefabCache& getPrefabCache() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the prototype system
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Create the main prototype manager instance
		*/
		virtual bool onStartup(bool serverMode) override;

		/**
		*  @brief
		*    Shutdown the prototype system
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Destroy the main prototype manager instance
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Validate that there's a main prototype manager
		*
		*  @note
		*    - While the prototype system is up-and-running, there must always be a valid main prototype manager
		*/
		void validateMainManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ConsecutiveIdGenerator<uint32> mIdGenerator;	///< Prototype manager ID generator
		PrototypeManager*			   mMainManager;	///< Main prototype manager instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ManagerList					   mManagerList;	///< List of all prototype managers within the prototype system
		ManagerMap					   mManagerMap;		///< Prototype manager map with unique prototype manager identifier as key and pointer to the prototype manager as value
		PrototypeFinalizerManager*	   mPrototypeFinalizerManager;		///< Prototype finalizer manager instance, we're responsible for destroying the instance in case we no longer need it
		PrefabCache*				   mPrefabCache;	///< Prefab cache


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/prototype/PrototypeSystem-inl.h"
