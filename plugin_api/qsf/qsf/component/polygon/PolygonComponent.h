// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/nodes/NodesComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BulletTriangleMeshCollisionComponent;
	class Color4;
	class Complex2DPolygon;
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
	*    Polygon component class
	*
	*  @todo
	*    - TODO(co) Please note that in here, the term "polygon" is not used as it is used normally in mathematics and computer graphics. As a graphics programmer I'm not really happy that we misuse the name "polygon" in here which will probably confuse people outside the company. Something like "PolygonVolumeComponent", "AmbientVolumeComponent" etc. would make much more sense.
	*/
	class QSF_API_EXPORT PolygonComponent : public NodesComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PolygonComponent" unique component ID
		static const uint32 HEIGHT;			///< "Height" unique class property ID inside the class


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
		inline PolygonComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PolygonComponent();

		/**
		*  @brief
		*    Return the height of the polygon
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getHeight() const;

		/**
		*  @brief
		*   Set the height of the polygon
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setHeight(float height);

		/**
		*  @brief
		*    "true" if the tested position is inside the polygon; test is only 2d, until stated otherwise
		*/
		bool isPositionInside(const glm::vec3& worldPosition, bool perform3dTest = false) const;

		/**
		*  @brief
		*    Return the closed polygon distance by using a given world space position
		*/
		float getDistanceToClosestLine(const glm::vec3& worldPosition, bool allowInnerDistance = false, glm::vec3* nearestPointOnPoly = nullptr) const;

		/**
		*  @brief
		*    Fills out a Complex2DPolygon structure with the information from this polygon
		*/
		void getComplex2DPolygon(Complex2DPolygon& outPolygon) const;

		/**
		*  @brief
		*    Internal helper method to create a fitting collision mesh for this polygon
		*/
		void buildCollisionMeshIfNecessary(bool isStartup);
		void buildCollisionMeshIfNecessary(bool isStartup, BulletTriangleMeshCollisionComponent& bulletTriangleMeshCollisionComponent);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::NodesComponent methods            ]
	//[-------------------------------------------------------]
	public:
		virtual uint32 getMiniumNumberOfNodes() const override;
		virtual void setNode(std::size_t index, const Node& value) override;
		virtual void setNodeToPosition(uint32 nodeIndex, const glm::vec3& newPosition) override;
		virtual void addPosition(const glm::vec3& position) override;
		virtual void setNodes(const std::vector<Node>& nodes) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void setDebug(bool debugMode) override;
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool closedContinuousLines() const override;
		virtual void startDebugDrawing() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PolygonComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual const Color4& getPolygonDebugColor() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float mHeight;	///< Current height of polygon


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
#include "qsf/component/polygon/PolygonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PolygonComponent)
