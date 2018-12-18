// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Filter configuration
		*
		*  @todo
		*    - TODO(co) Might be a good idea to use flags instead of tons of boolean variables
		*/
		class FilterConfiguration
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			FilterConfiguration();

			void setAll(bool enabled);

			bool getFilterPrefabAsset() const;
			void setFilterPrefabAsset(bool value);

			bool getFilterMeshAsset() const;
			void setFilterMeshAsset(bool value);

			bool getFilterParticlesAsset() const;
			void setFilterParticlesAsset(bool value);

			bool getFilterNames() const;
			void setFilterNames(bool value);

			bool getFilterId() const;
			void setFilterId(bool value);

			bool getFilterComponent() const;
			void setFilterComponent(bool value);

			bool getFilterTag() const;
			void setFilterTag(bool value);

			bool getFilterLayer() const;
			void setFilterLayer(bool value);

			bool getFilterDescription() const;
			void setFilterDescription(bool value);

			bool getFilterBaseMeshName() const;
			void setFilterBaseMeshName(bool value);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mFilterPrefabAsset;
			bool mFilterMeshAsset;
			bool mFilterParticlesAsset;
			bool mFilterNames;
			bool mFilterId;
			bool mFilterComponent;
			bool mFilterTag;
			bool mFilterLayer;
			bool mFilterDescription;
			bool mFilterBaseMeshName;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
