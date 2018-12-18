// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"
#include "qsf_logic/sequence/serialization/SequenceSerializer.h"

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}
namespace qsf
{
	namespace logic
	{
		class SequenceTrack;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    JSON sequence serializer
		*/
		class QSF_LOGIC_API_EXPORT JsonSequenceSerializer : public SequenceSerializer
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 FORMAT_VERSION;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Load the complete sequence with format headers from a ptree
			*/
			static void loadJson(Sequence& sequence, const boost::property_tree::ptree& rootPTree);

			/**
			*  @brief
			*    Load the whole sequence (without header) from a ptree
			*/
			static void loadSequenceV1(Sequence& sequence, const boost::property_tree::ptree& rootPTree);
			static void loadSequenceV2(Sequence& sequence, const boost::property_tree::ptree& rootPTree);

			/**
			*  @brief
			*    Load a individual track from a ptree
			*/
			static void loadTrackV1(SequenceTrack& sequenceTrackEntityComponentProperty, const boost::property_tree::ptree& trackPTree);
			static void loadTrackV2(SequenceTrack& sequenceTrackEntityComponentProperty, const boost::property_tree::ptree& trackPTree);

			/**
			*  @brief
			*    Save the complete sequence with format headers into a ptree
			*/
			static void saveJson(const Sequence& sequence, bool differenceToDefault, boost::property_tree::ptree& parentPTree);

			/**
			*  @brief
			*    Save the whole sequence (without header) into the ptree
			*/
			static void saveSequence(const Sequence& sequence, boost::property_tree::ptree& parentPTree);

			/**
			*  @brief
			*    Save a individual track to the ptree
			*/
			static void saveTrack(const SequenceTrack& sequenceTrackEntityComponentProperty, boost::property_tree::ptree& trackPTree);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			JsonSequenceSerializer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~JsonSequenceSerializer();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::SequenceSerializer methods ]
		//[-------------------------------------------------------]
		public:
			virtual void deserialize(Sequence& sequence, std::istream& istream) const override;
			virtual void serialize(Sequence& sequence, std::ostream& ostream, bool differenceToDefault = true) const override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
