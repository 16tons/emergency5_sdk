// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/serializer/MapSerializer.h"


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
	*    OgreMAX scene map serializer
	*
	*  @note
	*    - "OgreMax Scene Exporter" (www.ogremax.com) scene support for rapid prototyping
	*/
	class SceneMapSerializer : public MapSerializer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline SceneMapSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SceneMapSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MapSerializer methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(Map& map, std::istream& istream, const Map::SerializationOptions& serializationOptions) const override;
		virtual void serialize(const Map& map, std::ostream& ostream, const Map::SerializationOptions& serializationOptions) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load a OgreMax scene
		*
		*  @param[in] map
		*    Map instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadScene(Map& map, boost::property_tree::ptree& rootPTree) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/serializer/SceneMapSerializer-inl.h"
