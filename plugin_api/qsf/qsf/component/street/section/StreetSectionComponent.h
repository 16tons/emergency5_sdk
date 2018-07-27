// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/street/StreetComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CameraComponent;
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
	*    Street section component class
	*
	*  @note
	*    - This class is the part between two street crossings
	*    - The street section component takes care that the procedural OGRE mesh is generated and tells the mesh component about the procedural mesh
	*    - Technically quite similar to "qsf::PathMeshComponent"
	*
	*  @todo
	*    - TODO(np) If the node points are to close, the street becomes corrupt. Need to implement exceptional cases
	*    - TODO(co) When saving the OGRE mesh, do also automatically generate mesh LODs by e.g. reducing the number of segments within lower LODs
	*/
	class QSF_API_EXPORT StreetSectionComponent : public StreetComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::StreetSectionComponent" unique component ID
		static const uint32 BASE_MESH;					///< "BaseMesh" unique class property ID inside the class
		static const uint32 FLIP_X_AXIS;				///< "FlipXAxis" unique class property ID inside the class
		static const uint32 FLIP_Y_AXIS;				///< "FlipYAxis" unique class property ID inside the class
		static const uint32 BLEND_OUT_LENGTH_AT_END;	///< "BlendOutLengthAtEnd" unique class property ID inside the class
		static const uint32 BLEND_OUT_LENGTH_AT_START;	///< "BlendOutLengthAtStart" unique class property ID inside the class

		// TODO(co) For now, we have no procedural mesh LODs due to the memory consumption issues of EMERGENCY 5
		static const uint32 STREETSECTION_NUM_LODS = 1;
		// Not constant by intent, usually controlled by a high level quality control
		static int STREETSECTION_SEGMENTS_MIN[STREETSECTION_NUM_LODS];	// The minimum segments when the street section is straight. This value should have a minimum of 4 to prevent artefacts in curve creation.
		static int STREETSECTION_SEGMENTS_MAX[STREETSECTION_NUM_LODS];	// The maximum segments when the curve becomes spiky


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
		inline explicit StreetSectionComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StreetSectionComponent();

		/**
		*  @brief
		*    Return the length from first point till last point
		*
		*  @return
		*    The length from first point till last point
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		float getLength() const;

		/**
		*  @brief
		*    Return the base mesh asset to use
		*
		*  @return
		*    The base mesh asset to use (e.g. "sample/mesh/default/street_01c"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getBaseMesh() const;

		/**
		*  @brief
		*    Set the base mesh asset to use
		*
		*  @param[in] baseMesh
		*    The base mesh asset to use (e.g. "sample/mesh/default/street_01c")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBaseMesh(const AssetProxy& baseMesh);

		/**
		*  @brief
		*    Return whether or not the x axis should be flipped
		*
		*  @return
		*    "true" if the x axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getFlipXAxis() const;

		/**
		*  @brief
		*    Set whether or not the x axis should be flipped
		*
		*  @param[in] flipXAxis
		*    "true" if the x axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFlipXAxis(bool flipXAxis);

		/**
		*  @brief
		*    Return whether or not the y axis should be flipped
		*
		*  @return
		*    "true" if the y axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getFlipYAxis() const;

		/**
		*  @brief
		*    Set whether or not the y axis should be flipped
		*
		*  @param[in] flipYAxis
		*    "true" if the y axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFlipYAxis(bool flipYAxis);

		/**
		*  @brief
		*    Return the length of the blend out at the start of the path
		*
		*  @return
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getBlendOutLengthAtStart() const;

		/**
		*  @brief
		*    Set how long the blend out is at the start of the path
		*
		*  @param[in] blendOutLength
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBlendOutLengthAtStart(float blendOutLengthAtStart);

		/**
		*  @brief
		*    Return the length of the blend out at the end of the path
		*
		*  @return
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getBlendOutLengthAtEnd() const;

		/**
		*  @brief
		*    Set how long the blend out is at the end of the path
		*
		*  @param[in] blendOutLength
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBlendOutLengthAtEnd(float blendOutLengthAtEnd);

		/**
		*  @brief
		*    Mark the procedural street mesh to be dirty
		*
		*  @note
		*   - Please do only call this method if you have to and you know what you're doing
		*/
		void dirtyMesh();

		//[-------------------------------------------------------]
		//[ Ease of use                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the street gateway begin link
		*
		*  @return
		*    The street gateway begin link, do not destroy the instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const StreetGatewayLink& getBeginStreetGatewayLink() const;

		/**
		*  @brief
		*    Return the street gateway end link
		*
		*  @return
		*    The street gateway end link, do not destroy the instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const StreetGatewayLink& getEndStreetGatewayLink() const;

		/**
		*  @brief
		*    Calculate the derived node positions
		*
		*  @param[out] derivedNodes
		*    Will receive the derived local space node positions, list is not cleared before new entries are added
		*  @param[in] segmentsMin
		*    The minimum segments when the street section is straight. This value should have a minimum of 4 to prevent artefacts in curve creation.
		*  @param[in] segmentsMax
		*    The maximum segments when the curve becomes spiky
		*/
		void calculateDerivedNodePositions(std::vector<glm::vec3>& derivedNodes, int segmentsMin, int segmentsMax) const;

		/**
		*  @brief
		*    Snap the given tier node to the given position
		*
		*  @param[in] tierNodeIndex
		*    Index of the tied node to snap
		*  @param[in] localSpacePosition
		*    Local space position to snap the given tier node to
		*  @param[in] localSpaceDirection
		*    Normalized local space direction used during snapping
		*
		*  @note
		*    - This method automatically respects the tier constraints
		*/
		void snapTierNodeToPositionDirection(uint32 tierNodeIndex, const glm::vec3& localSpacePosition, const glm::vec3& localSpaceDirection);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::StreetComponent methods           ]
	//[-------------------------------------------------------]
	public:
		virtual glm::vec3 getStreetGatewayPosition(uint32 streetGatewayIndex) const override;
		virtual glm::vec3 getStreetGatewayDirection(uint32 streetGatewayIndex) const override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::NodesComponent methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void setNode(std::size_t index, const Node& value) override;
		virtual void setNodeToPosition(uint32 nodeIndex, const glm::vec3& newPosition) override;
		virtual void addPosition(const glm::vec3& position) override;
		virtual void setNodes(const std::vector<Node>& nodes) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		virtual void startDebugDrawing() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool ignoreOwnerRunningState() const override;
		inline virtual bool implementsOnComponentPropertyChange() const override;
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Register the given street gateway link inside the street crossing it's referencing
		*
		*  @param[in] gatewayIndex
		*    Street gateway index
		*/
		void registerStreetGatewayLink(uint32 gatewayIndex);

		/**
		*  @brief
		*    Unregister the given street gateway link from the street crossing it's referencing
		*
		*  @param[in] gatewayIndex
		*    Street gateway index
		*/
		void unregisterStreetGatewayLink(uint32 gatewayIndex);

		/**
		*  @brief
		*    Update the procedural street mesh if required
		*/
		void updateMesh();

		/**
		*  @brief
		*    This method gets called before a compositor workspace gets rendered
		*
		*  @param[in] cameraComponent
		*    Used camera component
		*/
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);

		/**
		*  @brief
		*    This method is called after the map has been saved
		*/
		// TODO(co) Remove procedural geometry file caching all together? It is constantly causing issues up to crashes.
		// void onPostMapSave();

		/**
		*  @brief
		*    Build the base mesh
		*
		*  @param[in] refresh
		*    Build the base mesh also when it already exists
		*/
		void buildMesh(bool refresh);

		/**
		*  @brief
		*    Set the mesh of the mesh component
		*/
		void setMeshComponentMesh();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 BEGIN_INDEX;	///< Begin gateway index
		static const uint32 END_INDEX;		///< End gateway index


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy					mBaseMesh;
		bool						mFlipXAxis;
		bool						mFlipYAxis;
		float						mBlendOutLengthAtStart;
		float						mBlendOutLengthAtEnd;
		// Internal only
		std::string					mGeneratedOgreMeshNames[STREETSECTION_NUM_LODS];	///< Name of the OGRE meshes that were created
		bool						mDirtyMesh;											///< Does the procedural street mesh require an update?
		std::vector<StreetGateway>	mRegisteredStreetGateways;							///< Contains all registered street gateways of the street component, this list must keep free of duplicates
		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;
		// boost::signals2::connection mPostMapSaveConnection;	// TODO(co) Remove procedural geometry file caching all together? It is constantly causing issues up to crashes.


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
#include "qsf/component/street/section/StreetSectionComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::StreetSectionComponent)
