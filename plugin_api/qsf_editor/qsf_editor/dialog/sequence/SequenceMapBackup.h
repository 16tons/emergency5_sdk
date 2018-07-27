// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/PagedMemoryBuffer.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	namespace logic
	{
		class Sequence;
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
		*    Sequence map backup implementation using a binary serializer
		*
		*  @todo
		*    - TODO(co) Adding new tracks during sequence editing has to be handled property. Currently done via hack. See "qsf::editor::SequenceAddTrackOperation::sealImpl()".
		*/
		class SequenceMapBackup : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const std::string FORMAT_TYPE;		///< Sequence map backup format type string "SequenceMap"
			static const uint32		 FORMAT_VERSION;	///< Current sequence map backup format version


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			SequenceMapBackup();

			/**
			*  @brief
			*    Destructor
			*/
			~SequenceMapBackup();

			/**
			*  @brief
			*    Check if the sequence map backup is empty
			*
			*  @return
			*    "true" if the sequence map backup is empty, "false" if it has any content
			*/
			bool isEmpty() const;

			/**
			*  @brief
			*    Clear the sequence map backup's contents
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
			*    Create a backup of the sequence map
			*
			*  @param[in] sequence
			*    The sequence map instance to save
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool backupSequenceMap(const logic::Sequence& sequence);

			/**
			*  @brief
			*    Load a sequence map from the backup
			*
			*  @param[out] sequence
			*    The sequence map instance to load
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool restoreSequenceMap(logic::Sequence& sequence) const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			bool serializeSequence(BinarySerializer& serializer, logic::Sequence& sequence);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PagedMemoryStream mStream;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
