// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"


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
	*    Annotation component class
	*
	*  @remarks
	*    This component is for internal development purposes only. A use case would be
	*    the "flagging" of a certain map position inside from the editor. In this case
	*    the editor would create a new entity with instances of a transform component as
	*    well as a annotation component. An annotation browser can then automatically
	*    enumerate all annotation component instances within a map.
	*/
	class QSF_API_EXPORT AnnotationComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::AnnotationComponent" unique component ID
		static const uint32 AUTHOR;				///< "Author" unique class property ID inside the class
		static const uint32 NAME;				///< "Name" unique class property ID inside the class
		static const uint32 DESCRIPTION;		///< "Description" unique class property ID inside the class
		static const uint32 ANNOTATION_TYPE;	///< "AnnotationType" unique class property ID inside the class
		static const uint32 SEVERITY_LEVEL;		///< "SeverityLevel" unique class property ID inside the class

		/**
		*  @brief
		*    Annotation type, indicates what kind of annotation it is
		*/
		enum AnnotationType
		{
			BUG_TYPE	 = 0,	///< Indicates a error in the level/graphics/etc
			COMMENT_TYPE = 1	///< A commented to something
		};

		/**
		*  @brief
		*    Severity level
		*/
		enum SeverityLevel
		{
			LOW_SEVERITY	= 0,	///< Low severity
			NORMAL_SEVERITY	= 1,	///< Normal severity
			HIGH_SEVERITY	= 2,	///< High severity
			URGENT_SEVERITY	= 3		///< Urgent severity
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
		inline explicit AnnotationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AnnotationComponent();

		/**
		*  @brief
		*    Return the annotation author
		*
		*  @return
		*    The UTF-8 annotation author, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getAuthor() const;

		/**
		*  @brief
		*    Set the annotation author
		*
		*  @param[in] author
		*    The UTF-8 annotation author
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAuthor(const std::string& author);

		/**
		*  @brief
		*    Return the annotation name
		*
		*  @return
		*    The UTF-8 annotation name, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the annotation name
		*
		*  @param[in] name
		*    The UTF-8 annotation name
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setName(const std::string& name);

		/**
		*  @brief
		*    Return the annotation description
		*
		*  @return
		*    The UTF-8 annotation description, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getDescription() const;

		/**
		*  @brief
		*    Set the annotation description
		*
		*  @param[in] description
		*    The UTF-8 annotation description
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setDescription(const std::string& description);

		/**
		*  @brief
		*    Return the annotation type
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline AnnotationType getAnnotationType() const;

		/**
		*  @brief
		*    Set the annotation type
		*
		*  @param[in] annotationType
		*    The type of the annotation
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAnnotationType(AnnotationType annotationType);

		/**
		*  @brief
		*    Return the severity level
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline SeverityLevel getSeverityLevel() const;

		/**
		*  @brief
		*    Set the severity level
		*
		*  @param[in] severityLevel
		*    Severity level
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSeverityLevel(SeverityLevel severityLevel);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update debug visualization components of the owner entity
		*/
		void updateVisualizationComponents() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		std::string	   mAuthor;			///< UTF-8 annotation author
		std::string	   mName;			///< UTF-8 annotation name
		std::string	   mDescription;	///< UTF-8 annotation description
		AnnotationType mAnnotationType; ///< Indicates the kind of annotation
		SeverityLevel  mSeverityLevel;	///< Severity level


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
#include "qsf/component/utility/AnnotationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AnnotationComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AnnotationComponent::AnnotationType)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AnnotationComponent::SeverityLevel)
