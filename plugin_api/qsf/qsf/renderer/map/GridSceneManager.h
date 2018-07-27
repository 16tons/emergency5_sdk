// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/MapSceneManager.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
	class GridRootSceneNode;
	class GridCellSceneNode;
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
	*    Specialized QSF OGRE scene manager
	*
	*  @remarks
	*    The "Ogre::OctreeSceneManager" implementation is pretty inefficient and has been dropped in OGRE 2.0, so, all we have left with OGRE is a
	*    traditional hierarchical scene graph. While we have some hierarchical entity relation ships expressed via the link component, the mass of
	*    entities "floats loosely in space". A simple 2D grid approach is sufficient for most of our use-cases. So, the map is subdivided into cells,
	*    each of which acts as a slot real scene nodes can plug into.
	*
	*    Terminology: We've chosen the term "cell" in order to make it easier distinguish different use-cases:
	*    - Grid cell
	*    - Memory page
	*    - Terrain chunk
	*/
	class QSF_API_EXPORT GridSceneManager : public MapSceneManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GridSceneNode;
		friend class GridRaySceneQuery;
		friend class GridSphereSceneQuery;
		friend class GridPlaneBoundedVolumeListSceneQuery;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GridSceneManager(const Ogre::String& instanceName, size_t numWorkerThreads, Ogre::InstancingThreadedCullingMethod threadedCullingMethod);
		virtual ~GridSceneManager();

		/**
		*  @brief
		*    Set the axis aligned bounding box of the 2D grid used for spatial subdivision
		*
		*  @param[in] ogreAxisAlignedBox
		*    World space axis aligned bounding box of the 2D grid
		*  @param[in] cellsPerEdge
		*    Number of grid cells along a grid edge, e.g. 16
		*
		*  @note
		*    - This method is considered to be expensive, don't use it if you can avoid it
		*/
		void set2dGridOgreAxisAlignedBox(const Ogre::AxisAlignedBox& ogreAxisAlignedBox, uint32 cellsPerEdge);

		/**
		*  @brief
		*    Return in which cell scene node a given position resides in
		*
		*  @param[in] position
		*    World space position to return the owning cell scene node for
		*
		*  @return
		*    The cell scene node the given position resides in
		*/
		GridCellSceneNode* getCellSceneNode(const glm::vec3& position) const;

		inline GridCellSceneNode* getCellSceneNodeAtIndex(uint32 x, uint32 z) const;

		/**
		*  @brief
		*    Create a cell child scene node
		*
		*  @param[in] position
		*    World space position of the cell child scene node
		*  @param[in] component
		*    Optional owner component making the create request, used for additional error information
		*
		*  @return
		*    The created cell child scene node
		*/
		Ogre::SceneNode* createCellChildSceneNode(const Ogre::Vector3& position, const Component* component = nullptr);

		/**
		*  @brief
		*    Set a cell child scene node position
		*
		*  @param[in] ogreSceneNode
		*    Cell child scene node to set to a new position
		*  @param[in] position
		*    New world space position of the cell child scene node
		*  @param[in] dynamic
		*    Is the scene node considered to be dynamic?
		*  @param[in] component
		*    Optional owner component making the create request, used for additional error information
		*  @param[in] ogreMovableObject
		*    Optional OGRE movable object we can ask e.g. about the local bounding box
		*/
		void setCellChildSceneNodePosition(Ogre::SceneNode& ogreSceneNode, const Ogre::Vector3& position, bool dynamic = true, const Component* component = nullptr, const Ogre::MovableObject* ogreMovableObject = nullptr);

		void attachObjectToCellChildSceneNode(Ogre::SceneNode& ogreSceneNode, Ogre::MovableObject& ogreMovableObject);

		bool getPotentialCells(const Ogre::Vector3& position, int& xStart, int& zStart, int& xEnd, int& zEnd) const;

		inline const Ogre::Vector3& getCellSize() const;
		inline GridRootSceneNode* getGridRootSceneNode() const;
		inline bool getAggressiveGridCulling() const;
		inline void setAggressiveGridCulling(bool aggressiveGridCulling);
		inline bool getAggressiveRenderingDistanceGridCulling() const;
		inline void setAggressiveRenderingDistanceGridCulling(bool aggressiveRenderingDistanceGridCulling);
		inline int32 getAggressiveGridCullingRadius() const;
		inline void setAggressiveGridCullingRadius(int32 aggressiveGridCullingRadius);
		inline bool getShowGrid() const;
		inline void setShowGrid(bool showGrid);


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::SceneManager methods             ]
	//[-------------------------------------------------------]
	public:
		virtual const Ogre::String& getTypeName() const override;
		virtual Ogre::AxisAlignedBoxSceneQuery* createAABBQuery(const Ogre::AxisAlignedBox& box, Ogre::uint32 mask = QUERY_ENTITY_DEFAULT_MASK) override;
		virtual Ogre::SphereSceneQuery* createSphereQuery(const Ogre::Sphere& sphere, Ogre::uint32 mask = QUERY_ENTITY_DEFAULT_MASK) override;
		virtual Ogre::PlaneBoundedVolumeListSceneQuery* createPlaneBoundedVolumeQuery(const Ogre::PlaneBoundedVolumeList& volumes, Ogre::uint32 mask = QUERY_ENTITY_DEFAULT_MASK) override;
		virtual Ogre::RaySceneQuery* createRayQuery(const Ogre::Ray& ray, Ogre::uint32 mask = QUERY_ENTITY_DEFAULT_MASK) override;
		virtual Ogre::IntersectionSceneQuery* createIntersectionQuery(Ogre::uint32 mask = QUERY_ENTITY_DEFAULT_MASK) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MapSceneManager methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void pushOgreManualObjectLines() override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::SceneManager methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual Ogre::SceneNode* createSceneNodeImpl(Ogre::SceneNode* parent, Ogre::NodeMemoryManager* nodeMemoryManager) override;
		virtual void highLevelCull() override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void pushBackNodeMemoryManager(Ogre::SceneManager::NodeMemoryManagerVec& ogreNodeMemoryManagerVec, Ogre::NodeMemoryManager& ogreNodeMemoryManager);
		static void pushBackObjectMemoryManager(Ogre::SceneManager::ObjectMemoryManagerVec& ogreObjectMemoryManagerVec, Ogre::ObjectMemoryManager& ogreObjectMemoryManager);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addStaticGridRootSceneNodeChild(Ogre::SceneNode& ogreSceneNode);
		void pushBackGridCellSceneNode(GridCellSceneNode& gridCellSceneNode);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::AxisAlignedBox			m2dGridOgreAxisAlignedBox;
		uint32							mCellsPerEdge;								///< Number of grid cells along a grid edge, e.g. 16
		Ogre::Vector3					mCellSize;									///< World space size of a single cell which contains the top level OGRE scene node, derived from the given 2D grid axis aligned bounding box
		uint32							mNumberOfCellOgreSceneNodes;
		GridRootSceneNode*				mGridRootSceneNode;
		GridCellSceneNode*				mOversizeGridCellSceneNode;
		Ogre::SceneNode*				mOutsideGridSceneNode;
		std::vector<GridCellSceneNode*>	mCellOgreSceneNodes;
		bool							mAggressiveGridCulling;						///< Aggressive grid culling enabled?
		bool							mAggressiveRenderingDistanceGridCulling;	///< Aggressive grid culling enabled for distance rendering which is used for e.g. a first person camera?
		int32							mAggressiveGridCullingRadius;				///< Radius of grid cells around the camera to consider for culling; what's outside is automatically clipped if aggressive grid culling is enabled
		bool							mShowGrid;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/GridSceneManager-inl.h"
