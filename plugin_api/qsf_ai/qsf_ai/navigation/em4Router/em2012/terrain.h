// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/em4Router/ge_baselib/singleton.h"

#include <map>
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class EGenericObject;
	}
}
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// Keep congruent with TerrainClass in actor.h
		enum EObjectTerrainClass
		{
			EOTC_ANYTHING,
			EOTC_CAR,
			EOTC_TRUCK,
			EOTC_OFFROAD,
			EOTC_TRACKVEHICLE,
			EOTC_BOAT,
			EOTC_SHIP,
			EOTC_DIVER,
			EOTC_AIRPLANE,
			EOTC_CIVILIAN,
			EOTC_SQUAD,
			EOTC_SQUADCAR,
			EOTC_HELILANDING,
			EOTC_ANIMAL,
			EOTC_VIRTUALFLOOR,
			EOTC_TRAFFIC,
			EOTC_SHOOTABLE,
			EOTC_SEETHROUGH,
			EOTC_FORCESTREET,
			EOTC_RESCUEDOG,

			_EOTC_NUMCLASSES,
			EOTC_INVALID,
		};

		// TODO(co) "EM3 terrain" = "not the visual terrain as the term is commonly used", it's a "ground speed"-definition (surface property)
		class QSF_AI_API_EXPORT ETerrain
		{
		public:
			ETerrain();
			virtual ~ETerrain();

			inline float GetSpeed(EObjectTerrainClass Class_) const { return mSpeed[Class_]; }
			float GetSpeed(EGenericObject& obj_) const;
			inline void SetSpeed(EObjectTerrainClass Class_, float value_) { mSpeed[Class_] = value_; }
			inline const std::string& GetName() const { return mName; };
			bool IsWater() const { return (mSpeed[EOTC_CAR] <= 0.f) ? (mSpeed[EOTC_BOAT] > 0.f) || (mSpeed[EOTC_SHIP] > 0.f) || (mSpeed[EOTC_DIVER] > 0.f) : false; };
			// void SaveTo(baselib::TiXmlElement* Root_);	// TODO(co) Only loading was ported
			void LoadFrom(const boost::property_tree::ptree& terrainPTree);

			bool IsCameraFloor() const;

		protected:
			std::string mName;
			float mSpeed[_EOTC_NUMCLASSES];
		};

		class QSF_AI_API_EXPORT ETerrainManager : public BLSingleton<ETerrainManager>
		{
			DECLARESINGLETON(ETerrainManager);
		public:
			typedef std::map<std::string, ETerrain*> TerrainTree;

			void Clear();
			void LoadTerrains(const std::string& filename = "");
			// void SaveTerrains();	// TODO(co) Only loading was ported
			inline const TerrainTree& GetTerrains() const { return mTerrains; }
			ETerrain* GetTerrain(const std::string& name) const;

		private:
			ETerrainManager();
			~ETerrainManager();
			TerrainTree mTerrains;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
