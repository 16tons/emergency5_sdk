// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"

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
	*    Metadata component class
	*
	*  @note
	*    - Holds general information about a prototype/entity
	*    - The metadata component automatically registers/unregisters the owner entity within the correct layer
	*    - Entities without a metadata component are not registered inside any layer, this should be an exception
	*/
	class QSF_API_EXPORT MetadataComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Layer;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::MetadataComponent" unique component ID
		static const uint32 PREFAB;			///< "Prefab" unique class property ID inside the class
		static const uint32 PROTOTYPE;		///< "Prototype" unique class property ID inside the class
		static const uint32 NAME;			///< "Name" unique class property ID inside the class
		static const uint32 DESCRIPTION;	///< "Description" unique class property ID inside the class
		static const uint32 TAGS;			///< "Tags" unique class property ID inside the class
		static const uint32 LAYER_ID;		///< "LayerId" unique class property ID inside the class
		static const uint32 QUALITY;		///< "Quality" unique class property ID inside the class

		/**
		*  @brief
		*    Quality of the owner entity
		*
		*  @note
		*    This enum is equal with RendererSettingsGroup::MapQuality and it is also used as the same. Make changes only on both enums
		*/
		enum Quality
		{
			NO_QUALITY			= 0,	///< No quality specified, not allowed to be quality rejected
			LOW_QUALITY			= 1,	///< Low quality
			MEDIUM_QUALITY		= 2,	///< Medium quality
			HIGH_QUALITY		= 3,	///< High quality
			ULTRA_QUALITY		= 4,	///< Ultra quality
			NUMBER_OF_QUALITIES	= 5		///< Number of qualities
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit MetadataComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MetadataComponent();

		/**
		*  @brief
		*    Return the base prefab asset to use
		*
		*  @return
		*    The base prefab asset to use (e.g. "sample/prefab/animal/squirrel"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getBasePrefab() const;

		/**
		*  @brief
		*    Set the base prefab asset to use
		*
		*  @param[in] prefab
		*    The base prefab asset to use (e.g. "sample/prefab/animal/squirrel")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setBasePrefab(const AssetProxy& prefab);

		/**
		*  @brief
		*    Return the base prototype ID
		*
		*  @return
		*    The base prototype ID, or "getUninitialized<uint64>()" if there is none
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint64 getBasePrototypeId() const;

		/**
		*  @brief
		*    Return the base prototype
		*
		*  @return
		*    The base prototype, or a null pointer if there is none
		*/
		Prototype* getBasePrototype() const;

		/**
		*  @brief
		*    Set the base prototype ID
		*
		*  @param[in] prototypeId
		*    The base prototype ID, or "getUninitialized<uint64>()" if there is none
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setBasePrototypeId(uint64 prototypeId);

		/**
		*  @brief
		*    Return the source prototype ID
		*
		*  @return
		*    The source prototype ID, or "getUninitialized<uint64>()" if there is none
		*
		*  @note
		*    - We need this only as a temporary backup of the base prototype ID for prototype matching during the "save as prefab" process
		*/
		inline uint64 getSourcePrototypeId() const;

		/**
		*  @brief
		*    Set the source prototype ID
		*
		*  @param[in] prototypeId
		*    The source prototype ID, or "getUninitialized<uint64>()" if there is none
		*/
		inline void setSourcePrototypeId(uint64 prototypeId);

		/**
		*  @brief
		*    Return the name
		*
		*  @return
		*    The UTF-8 name (example: "Ambulance"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the name
		*
		*  @param[in] name
		*    The UTF-8 name (example: "Ambulance")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setName(const std::string& name);

		/**
		*  @brief
		*    Return the description
		*
		*  @return
		*    The UTF-8 description, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getDescription() const;

		/**
		*  @brief
		*    Set the description
		*
		*  @param[in] description
		*    The UTF-8 description
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setDescription(const std::string& description);

		/**
		*  @brief
		*    Return the tags as semicolon separated string
		*
		*  @return
		*    The UTF-8 tags as semicolon separated string (e.g. "car;person"), tags are not case-sensitive
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getTags() const;

		/**
		*  @brief
		*    Set the tags as semicolon separated string
		*
		*  @param[in] tags
		*    The UTF-8 tags as semicolon separated string (e.g. "car;person"), tags are not case-sensitive
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTags(const std::string& tags);

		/**
		*  @brief
		*    Return whether or not a given tag is there
		*
		*  @param[in] tag
		*    The UTF-8 tag to check (e.g. "Car"), tags are not case-sensitive
		*
		*  @return
		*    "true" if the tag has been found, else "false"
		*/
		bool hasTag(const std::string& tag) const;

		/**
		*  @brief
		*    Return the ID of the layer the owner of the component is in
		*
		*  @return
		*    The ID of the layer the owner of the component is in, 0 is the base layer
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint32 getLayerId() const;

		/**
		*  @brief
		*    Set the ID of the layer the owner of the component is in
		*
		*  @param[in] layerId
		*    The ID of the layer the owner of the component is in
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Every entity with a metadata component must be inside a valid layer
		*    - In case of an invalid layer ID the current layer ID will not be changed
		*    - In case of an invalid layer ID and the current layer ID is invalid as well,
		*      the base layer with the ID 0 will be used instead
		*/
		void setLayerId(uint32 layerId);

		/**
		*  @brief
		*    Return the quality of the owner entity
		*
		*  @return
		*    The quality of the owner entity
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline Quality getQuality() const;

		/**
		*  @brief
		*    Set the quality of the owner entity
		*
		*  @param[in] quality
		*    The quality of the owner entity
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setQuality(Quality quality);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy		mBasePrefab;		///< The prefab asset to use (e.g. "sample/prefab/animal/squirrel"); valid only if there is a prefab at all
		uint64			mBasePrototypeId;	///< ID of the prototype to inherit from; this prototype is part of the prefab asset
		FlyweightString mName;				///< UTF-8 name
		FlyweightString mDescription;		///< UTF-8 description
		FlyweightString mTags;				///< The UTF-8 tags as semicolon separated string (e.g. "car;person"), tags are not case-sensitive
		uint32			mLayerId;			///< The ID of the layer the owner of the component is in, 0 is the base layer, "qsf::getUninitialized<uint32>()" if invalid (should not happen outside the constructor)
		Quality			mQuality;			///< The quality of the owner entity

		// Internal only
		uint64			mSourcePrototypeId;	///< ID of the source prototype; we need this only as a temporary backup of the base prototype ID for prototype matching during the "save as prefab" process


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/base/MetadataComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MetadataComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MetadataComponent::Quality)
