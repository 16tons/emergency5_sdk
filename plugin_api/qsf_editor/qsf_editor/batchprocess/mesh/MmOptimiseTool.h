/*
This file is part of MeshMagick - An Ogre mesh file manipulation tool.
Copyright (C) 2007-2009 Daniel Wickert

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __MM_OPTIMISE_TOOL_H__
#define __MM_OPTIMISE_TOOL_H__


// Originally from "MeshMagick" - https://bitbucket.org/ogreaddons/meshmagick - already made some changes/extensions. Assimilate and cleanup.


#include <OgreMesh.h>
#include <OgreMeshSerializer.h>
#include <OgreSubMesh.h>
#include <OgreVector3.h>
#include <OgreVector4.h>


namespace meshmagick
{
	class OptimiseTool
	{
	public:
		OptimiseTool();
		void processMesh(Ogre::v1::Mesh& ogreMesh);

	protected:
		float mPosTolerance, mNormTolerance, mUVTolerance;

		struct IndexInfo
		{
			Ogre::uint32 targetIndex;
			bool isOriginal; // was this index the one that created the vertex in the first place

			IndexInfo(Ogre::uint32 targIdx, bool orig) : targetIndex(targIdx), isOriginal(orig) {}
			IndexInfo() : targetIndex(0), isOriginal(false) {}
		};
		/** Mapping from original vertex index to new (potentially shared) vertex index */
		typedef std::vector<IndexInfo> IndexRemap;
		IndexRemap mIndexRemap;

		struct UniqueVertex
		{
			Ogre::Vector3 position;
			Ogre::Vector3 normal;
			Ogre::Vector4 tangent;
			Ogre::Vector3 binormal;
			Ogre::Vector4 uv[OGRE_MAX_TEXTURE_COORD_SETS];

			UniqueVertex()
				: position(Ogre::Vector3::ZERO),
				  normal(Ogre::Vector3::ZERO),
				  tangent(Ogre::Vector4::ZERO),
				  binormal(Ogre::Vector3::ZERO)
			{
				memset(uv, 0, sizeof(Ogre::Vector4) * OGRE_MAX_TEXTURE_COORD_SETS);
			}

		};
		struct UniqueVertexLess
		{
			float pos_tolerance, norm_tolerance, uv_tolerance;
			unsigned short uvSets;
			bool operator()(const UniqueVertex& a, const UniqueVertex& b) const;

			bool equals(const Ogre::Vector3& a, const Ogre::Vector3& b, Ogre::Real tolerance) const;
			bool equals(const Ogre::Vector4& a, const Ogre::Vector4& b, Ogre::Real tolerance) const;
			bool less(const Ogre::Vector3& a, const Ogre::Vector3& b, Ogre::Real tolerance) const;
			bool less(const Ogre::Vector4& a, const Ogre::Vector4& b, Ogre::Real tolerance) const;
		};


		struct VertexInfo
		{
			Ogre::uint32 oldIndex;
			Ogre::uint32 newIndex;

			VertexInfo(Ogre::uint32 o, Ogre::uint32 n) : oldIndex(o), newIndex(n) {}
			VertexInfo() : oldIndex(0), newIndex(0) {}

		};
		/** Map used to efficiently look up vertices that have the same components.
		The second element is the source vertex info.
		*/
		typedef std::map<UniqueVertex, VertexInfo, UniqueVertexLess> UniqueVertexMap;
		UniqueVertexMap mUniqueVertexMap;
		/** Ordered list of unique vertices used to write the final reorganized vertex buffer
		*/
		typedef std::vector<VertexInfo> UniqueVertexList;
		UniqueVertexList mUniqueVertexList;

		Ogre::v1::VertexData* mTargetVertexData;
		typedef std::list<Ogre::v1::IndexData*> IndexDataList;
		IndexDataList mIndexDataList;

		void setTargetVertexData(Ogre::v1::VertexData* vd);
		void addIndexData(Ogre::v1::IndexData* id);
		bool optimiseGeometry();
		bool calculateDuplicateVertices();
		void rebuildVertexBuffers();
		void remapIndexDataList();
		void remapIndexes(Ogre::v1::IndexData* idata);
		Ogre::v1::Mesh::VertexBoneAssignmentList getAdjustedBoneAssignments(
			Ogre::v1::Mesh::BoneAssignmentIterator& it);
        void fixLOD(Ogre::v1::SubMesh::LODFaceList lodFaces);
		void print(const std::string& message);
		void warn(const std::string& message);

	};
}
#endif
