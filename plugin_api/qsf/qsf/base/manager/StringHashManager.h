// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <string>
#include <unordered_map>


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
	*    String hash manager class
	*
	*  @remarks
	*    This class manages a list of all existing string hashes.
	*    So it is able to recognize hash conflicts and dump all string hashes to a file.
	*    Also, you can use it for debugging, in Visual Studio simply add a watch to qsf::StringHashManager::mInstance
	*    to get a sorted list of all hashed values together with the strings they represent.
	*
	*  @todo
	*    - TODO(fw): The dump to file functionality is still missing. Could be called automatically in the destructor.
	*/
	class QSF_API_EXPORT StringHashManager
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Static method to register a string hash
		*/
		static void registerStringHash(const char* string, uint32 hash);

		/**
		*  @brief
		*    Destroy the singleton instance
		*/
		static void destroyInstance();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		StringHashManager();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~StringHashManager();

		/**
		*  @brief
		*    Registration of a string hash
		*/
		void internalRegisterStringHash(const char* string, uint32 hash);


	//[-------------------------------------------------------]
	//[ Protected static data                                 ]
	//[-------------------------------------------------------]
	protected:
		static StringHashManager* mInstance;	///< Singleton instance of the StringHashManager class


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, std::string> StringHashMap;
		StringHashMap mStringHashes;	///< Map of all known string hashes


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
