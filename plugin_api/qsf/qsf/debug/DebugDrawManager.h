// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/debug/DebugDrawLifetimeData.h"
#include "qsf/debug/DebugDrawOwnerData.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Transform;
	class GuiContext;
	class DebugDrawObject;
	class DebugDrawProxy;
	class DebugDrawRequest;
}
namespace Rocket
{
	namespace Core
	{
		class ElementDocument;
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
	*    Abstract debug draw manager class
	*
	*  @remarks
	*    The following example shows how to perform a debug draw request
	*    @code
	*    QSF_DEBUGDRAW.requestDraw(qsf::SegmentDebugDrawRequest(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(10.0f, 1.0f, 0.0f)));
	*    @endcode

	*  @note
	*    - Use e.g. "QSF_DEBUGDRAW" to get a debug draw manager instance in case you don't care about a particular map instance
	*    - The debug draw manager is part of the map, but not inside the
	*      map directory because the debug features are important enough for an own root directory
	*    - All debug draw requests result in permanent renderings by default, not only a single frame;
	*      "DebugDrawLifetimeData" can be used to control this behaviour, e.g. limit drawing to fixed time period
	*/
	class QSF_API_EXPORT DebugDrawManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Map;				// Only the map is allowed to create debug draw manager instances
	friend class DebugDrawProxy;	// Proxies may access protected methods


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the owner map instance of this debug draw manager instance
		*
		*  @return
		*    The owner map instance of this debug draw manager instance, do not destroy the instance
		*/
		inline Map& getMap() const;

		/**
		*  @brief
		*    Return the used GUI context instance
		*
		*  @return
		*    The used GUI context instance, can be a null pointer, do no destroy the returned instance
		*/
		inline GuiContext* getGuiContext() const;

		/**
		*  @brief
		*    Set the used GUI context instance
		*
		*  @param[in] guiContext
		*    The used GUI context instance, null pointer to set no GUI context
		*/
		inline void setGuiContext(GuiContext* guiContext);

		/**
		*  @brief
		*    Request a single simple debug draw
		*
		*  @param[in] debugDrawRequest
		*    Debug draw request instance
		*  @param[in] debugDrawLifetimeData
		*    Debug draw lifetime data, by default an unlimited lifetime setting is used
		*  @param[in] debugDrawOwnerData
		*    Gives information about the draw request's owner, used for allowing selection of an entity by the draw request's geometry
		*
		*  @return
		*    The returned ID can be used to cancel the request manually
		*/
		uint32 requestDraw(const DebugDrawRequest& debugDrawRequest, const DebugDrawLifetimeData& debugDrawLifetimeData = DebugDrawLifetimeData(), const DebugDrawOwnerData& debugDrawOwnerData = DebugDrawOwnerData());

		/**
		*  @brief
		*    Set the transform of a debug draw request
		*
		*  @param[in] id
		*    ID of the debug request to update
		*  @param[in] transform
		*    New transform of the debug draw request
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false"
		*/
		bool setRequestTransform(uint32 id, const Transform& transform) const;

		/**
		*  @brief
		*    Set rendering space of the draw request
		*
		*  @param[in] id
		*    ID of the debug request
		*  @param[in] drawInClipSpace
		*    If true, render in clip space (with coordinates in [-1, 1]), otherwise in world space
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false"
		*/
		bool setRequestDrawInClipSpace(uint32 id, bool drawInClipSpace) const;

		/**
		*  @brief
		*    Set the object in billboard state, orientates to the camera
		*
		*  @return
		*    "true" if all went fine and the debug draw request was updated, else "false"
		*/
		bool setRequestDrawAsBillboard(uint32 id, bool drawAsBillboard) const;

		/**
		*  @brief
		*    Check whether a given debug draw request ID is valid
		*
		*  @param[in] id
		*    ID of the debug draw request
		*
		*  @return
		*    "true" if the ID is valid, "false" if the request was already canceled or its lifetime is over
		*/
		bool isRequestIdValid(uint32 id) const;

		/**
		*  @brief
		*    Cancel a created request
		*
		*  @param[in] id
		*    ID of the debug request to cancel
		*
		*  @return
		*    "true" if all went fine and the debug draw request was canceled, else "false"
		*/
		bool cancelRequest(uint32 id);

		/**
		*  @brief
		*    Cancel all requests
		*/
		void cancelAllRequests();

		/**
		*  @brief
		*    Show or hide debug drawing
		*/
		void setDebugDrawsVisible(bool visible);

		/**
		*  @brief
		*    Return the debug draw object's internal libRocket element document - if necessary, create one
		*/
		Rocket::Core::ElementDocument& getOrCreateRocketElementDocument();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] map
		*    The owner map instance of this debug draw manager instance
		*/
		inline explicit DebugDrawManager(Map& map);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DebugDrawManager();

		/**
		*  @brief
		*    Register a debug draw proxy
		*/
		void registerProxy(DebugDrawProxy& proxy);

		/**
		*  @brief
		*    Return the debug draw object associated with a given ID, or null pointer if there is none
		*/
		DebugDrawObject* getDebugDrawObjectById(uint32 id) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		DebugDrawObject* createNewDebugDrawObject(DebugDrawProxy* proxy = nullptr);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, DebugDrawObject*> DebugDrawObjectMap;	///< Maps a debug draw object's unique ID to pointer


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Map&			   mMap;		///< The owner map instance of this debug draw manager instance
		GuiContext*		   mGuiContext;	///< Used GUI context instance, can be a null pointer, do not destroy the instance
		Rocket::Core::ElementDocument* mRocketElementDocument;	///< libRocket element document instance holding the libRocket elements
		bool			   mDebugDrawsVisible;
		uint32			   mNextId;
		DebugDrawObjectMap mDebugDrawObjects;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawManager-inl.h"
