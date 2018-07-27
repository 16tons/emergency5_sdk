// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
	*    Decompression helper class for binary serialization
	*/
	class QSF_API_EXPORT BinarySerializerDecompress
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		BinarySerializerDecompress(bool isCompressed, BinarySerializer& inputSerializer, size_t uncompressedBufferMinSize = 0);
		~BinarySerializerDecompress();

		BinarySerializer& getUncompressedDataSerializer();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BinarySerializer& mInputSerializer;			///< Binary serializer we got in the constructor, will be filled with compressed data
		std::vector<char> mUncompressedBuffer;

		struct Uncompressed;
		Uncompressed* mUncompressed;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
