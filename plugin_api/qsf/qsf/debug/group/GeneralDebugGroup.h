// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/group/DebugGroup.h"
#include "qsf/base/UniqueInstance.h"


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
	*    General debug group implementation
	*/
	class QSF_API_EXPORT GeneralDebugGroup : public DebugGroup, public UniqueInstance<GeneralDebugGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;									///< "qsf::GeneralDebugGroup" unique pluginable debug group ID
		static const uint32 SHOW_FRAMES_PER_SECOND;							///< "ShowFramesPerSecond" unique class property ID inside the class
		static const uint32 SHOW_FRAMERATE_STABILITY;						///< "ShowFramerateStability" unique class property ID inside the class
		static const uint32 SHOW_COLLISION;									///< "ShowCollision" unique class property ID inside the class
		static const uint32 SHOW_LINKED_CHILDREN;							///< "ShowLinkedChildren" unique class property ID inside the class
		static const uint32 SHOW_POLYGON;									///< "ShowPolygon" unique class property ID inside the class
		static const uint32 SHOW_PATH;										///< "ShowPath" unique class property ID inside the class
		static const uint32 IGNORE_ENFORCED_APPLICATION_CORE_PROJECT_LOCK;	///< "IgnoreEnforcedApplicationCoreProjectLock" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] debugGroupManager
		*    Optional pointer to the debug group manager this debug group should be registered to, can be a null pointer
		*/
		explicit GeneralDebugGroup(DebugGroupManager* debugGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GeneralDebugGroup();

		/**
		*  @brief
		*    Return whether or frames per second information is shown
		*/
		inline bool getShowFramesPerSecond() const;

		/**
		*  @brief
		*    Set whether or not frames per second information is shown
		*/
		inline void setShowFramesPerSecond(bool showFramesPerSecond);

		/**
		*  @brief
		*    Return whether or not framerate stability shown
		*
		*  @note
		*    - Optional part of the frames per seconds visualization
		*/
		inline bool getShowFramerateStability() const;

		/**
		*  @brief
		*    Set whether or not framerate stability shown
		*
		*  @see
		*    - "qsf::GeneralDebugGroup::getShowFramerateStability()"
		*/
		inline void setShowFramerateStability(bool showFramerateStability);

		/**
		*  @brief
		*    Return whether or not collision rendering is enabled
		*/
		inline bool getShowCollision() const;

		/**
		*  @brief
		*    Set whether or not collision rendering is enabled
		*/
		void setShowCollision(bool showCollision);

		/**
		*  @brief
		*    Return whether or not linked children rendering is enabled
		*/
		inline bool getShowLinkedChildren() const;

		/**
		*  @brief
		*    Set whether or not linked children rendering is enabled
		*/
		inline void setShowLinkedChildren(bool showLinkdedChildren);

		/**
		*  @brief
		*    Return polygon show mode
		*/
		inline DrawMode getShowPolygon() const;

		/**
		*  @brief
		*    Set polygon show mode
		*/
		inline void setShowPolygon(DrawMode showPolygon);

		/**
		*  @brief
		*    Return path show mode
		*/
		inline DrawMode getShowPath() const;

		/**
		*  @brief
		*    Set path show mode
		*/
		inline void setShowPath(DrawMode showPath);

		inline bool getIgnoreEnforcedApplicationCoreProjectLock() const;
		inline bool getIgnoreEnforcedApplicationCoreProjectLockInternal() const;
		inline void setIgnoreEnforcedApplicationCoreProjectLock(bool ignoreEnforcedApplicationCoreProjectLock);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		bool	 mShowFramesPerSecond;		///< "true" if frames per second information is shown, else "false"
		bool	 mShowFramerateStability;	///< "true" if framerate stability is shown, else "false"
		bool	 mShowCollision;			///< "true" if collision rendering is enabled, else "false"
		bool	 mShowLinkedChildren;		///< "true" if linked children rendering is enabled, else "false"
		DrawMode mShowPolygon;				///< Polygon show mode
		DrawMode mShowPath;					///< Path show mode
		bool	 mIgnoreEnforcedApplicationCoreProjectLock;


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
#include "qsf/debug/group/GeneralDebugGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::GeneralDebugGroup)
