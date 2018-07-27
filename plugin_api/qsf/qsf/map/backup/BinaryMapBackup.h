// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/backup/MapBackup.h"
#include "qsf/base/VectorStreamBuf.h"


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
	*    Map backup implementation using a binary serializer
	*/
	class QSF_API_EXPORT BinaryMapBackup : public MapBackup
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BinaryMapBackup();

		/**
		*  @brief
		*    Destructor
		*/
		~BinaryMapBackup();

		/**
		*  @brief
		*    Check if the map backup is empty
		*
		*  @return
		*    "true" if the map backup is empty, "false" if it has any content
		*/
		bool isEmpty() const;

		/**
		*  @brief
		*    Clear the map backup's contents
		*/
		void clear();

		/**
		*  @brief
		*    Copy contents from a stream
		*/
		void copyFromStream(std::istream& stream);

		/**
		*  @brief
		*    Copy contents to a stream
		*/
		void copyToStream(std::ostream& stream);

		/**
		*  @brief
		*    Returns a reference to the internal paged memory stream tht contains the binary map backup
		*/
		VectorIOStream& getStream();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MapBackup methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create a backup of the map
		*
		*  @param[in] map
		*    The map instance to save
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool backupMap(const Map& map, const Map::SerializationOptions& serializationOptions) override;

		/**
		*  @brief
		*    Load a map from the backup
		*
		*  @param[out] map
		*    The map instance to load
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool restoreMap(Map& map, const Map::SerializationOptions& serializationOptions) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<char> mBuffer;
		VectorIOStream	  mStream;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
