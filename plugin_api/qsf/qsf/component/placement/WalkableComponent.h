// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/base/NamedIdentifier.h"


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
	*    Walkable component class
	*
	*  @remarks
	*    The walkable component serves as a marker for meshes that count as walkable underground, like the terrain or streets.
	*/
	class QSF_API_EXPORT WalkableComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::WalkableComponent" unique component ID
		static const uint32 LEVEL;			///< "Level" unique class property ID inside the class
		static const uint32 TAG;			///< "Tag" unique class property ID inside the class


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
		inline explicit WalkableComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WalkableComponent();

		/**
		*  @brief
		*    Get walkable level
		*/
		inline int32 getLevel() const;

		/**
		*  @brief
		*    Set walkable level
		*/
		void setLevel(int32 level);

		/**
		*  @brief
		*    Get walkable tag as string
		*/
		inline const std::string& getTag() const;

		/**
		*  @brief
		*    Get walkable tag's hash
		*/
		inline uint32 getTagHash() const;

		/**
		*  @brief
		*    Set walkable tag
		*/
		void setTag(const std::string& tag);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int32			mLevel;
		NamedIdentifier mTag;


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
#include "qsf/component/placement/WalkableComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::WalkableComponent)
