// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/CommandTargetManager.h"
#include "em5/command/Command.h"

#include <qsf_game/QsfGameHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/command/CommandManager.h>

#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialManager.h>
#include <qsf/renderer/map/GridSceneManager.h>
#include <qsf/renderer/resource/OgreResourceGroup.h>
#include <qsf/renderer/RenderQueueGroupId.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/QsfHelper.h>

#include <OGRE/OgreTechnique.h>
#include <OGRE/OgreManualObject2.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreMaterialManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CommandTargetManager::CommandTargetManager(qsf::GridSceneManager& gridSceneManager) :
		mGridSceneManager(gridSceneManager),
		mNumberOfCreatedOgreManualObjects(0)
	{
		// Create kickoff OGRE manual object instances for all available commands
		// -> This avoids hiccups due to synchronous icon texture loading when an icon is shown for the first time
		// -> After this we already have a free pool of manual OGRE objects for hiccup reduction
		std::vector<Ogre::ManualObject*> ogreManualObjects;
		for (const qsf::game::Command* genericCommand : QSFGAME_COMMAND.getCommandManager().getList<qsf::game::Command>())
		{
			const Command* command = dynamic_cast<const Command*>(genericCommand);
			if (nullptr != command && !command->getIconSettings().mMouseIconName.empty())
			{
				try
				{
					const std::string& mouseIconName = command->getIconSettings().mMouseIconName;

					// Enforce loading the texture at once
					Ogre::TexturePtr ogreTexturePtr = Ogre::TextureManager::getSingleton().load("em5/gui/cursor/interaction/" + mouseIconName + ".dds", qsf::OgreResourceGroup::OGRE_RESOURCE_GROUP);
					if (!ogreTexturePtr.isNull())
					{
						ogreTexturePtr->load();
					}

					// And up to our stack
					ogreManualObjects.push_back(rentOgreManualObject(mouseIconName, glm::vec2(1.0f, 1.0f)));
				}
				catch (...)
				{
					// Silently ignore this exception (probably a missing image file)
				}
			}
		}

		// I don't want those OGRE manual object, they are scratched
		for (Ogre::ManualObject* ogreManualObject : ogreManualObjects)
		{
			returnRentedOgreManualObject(*ogreManualObject);
		}
	}

	CommandTargetManager::~CommandTargetManager()
	{
		// Check our renting sanity counter
		QSF_ASSERT(mFreeOgreManualObjects.size() == mNumberOfCreatedOgreManualObjects, "Internal command target manager detected a renting mismatch", QSF_REACT_NONE);

		// Destroy the free OGRE manual objects
		for (Ogre::ManualObject* ogreManualObject : mFreeOgreManualObjects)
		{
			// Destroy the OGRE scene node
			mGridSceneManager.destroySceneNode(ogreManualObject->getParentSceneNode());

			// Destroy the OGRE manual object
			mGridSceneManager.destroyManualObject(ogreManualObject);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	Ogre::ManualObject* CommandTargetManager::rentOgreManualObject(const std::string& iconName, const glm::vec2& clipSpaceSize)
	{
		// Check and (if necessary) create OGRE material
		const std::string materialName = "em5::CommandTargetComponent_Material_" + iconName;
		{
			// Check and (if necessary) create OGRE material
			static qsf::MaterialManager& materialManager = QSF_MATERIAL.getMaterialManager();
			qsf::Material* material = materialManager.findElement(qsf::StringHash(materialName));
			if (nullptr == material)
			{
				material = materialManager.createMaterial(materialName, qsf::StringHash("qsf_unlit_mesh"));
			}
			if (nullptr != material)
			{
				material->setPropertyById("DrawInClipSpace", qsf::MaterialPropertyValue::fromBoolean(true));
				material->setPropertyById("UseColorMap", qsf::MaterialPropertyValue::fromBoolean(true));
				material->setPropertyById("ColorMap", qsf::MaterialPropertyValue::fromResourceName("em5/gui/cursor/interaction/" + iconName + ".dds"));
			}
		}

		// Do we still have an OGRE manual object in stock or do we need to create a new instance?
		Ogre::ManualObject* ogreManualObject = nullptr;
		if (mFreeOgreManualObjects.empty())
		{
			// Sorry, we're out of OGRE manual objects, wait a short moment while we're creating a new instance for you

			// Create a new OGRE manual object (that we can fill with generated mesh data)
			ogreManualObject = mGridSceneManager.createManualObject();
			ogreManualObject->setQueryFlags(0);	// Don't allow it to e.g. pick the command target marker

			// Setup OGRE manual object
			ogreManualObject->setCastShadows(false);
			++mNumberOfCreatedOgreManualObjects;

			// Handle the OGRE manual object as overlay by setting the proper render queue group
			ogreManualObject->setRenderQueueGroup(qsf::RENDER_QUEUE_9);	// Don't use "qsf::RENDER_QUEUE_OVERLAY"

			// Set bounding box to infinite
			ogreManualObject->setLocalAabb(Ogre::Aabb::BOX_INFINITE);

			// Assign manual OGRE object to OGRE scene node
			mGridSceneManager.getRootSceneNode()->createChildSceneNode()->attachObject(ogreManualObject);
			ogreManualObject->setVisible(true);

			// To save memory reallocation say the estimate number of vertices and indices to the OGRE manual object
			// -> See http://www.ogre3d.org/docs/api/html/classOgre_1_1ManualObject.html as reference
			const int vertexCount = 4;
			ogreManualObject->estimateVertexCount(vertexCount);
			ogreManualObject->estimateIndexCount(6);

			// Build icon geometry
			ogreManualObject->begin(materialName, Ogre::v1::RenderOperation::OT_TRIANGLE_LIST);
		}
		else
		{
			{ // OGRE manual object, rise from the dead and conquer the world!
				const size_t newNumberOfFreeOgreManualObjects = mFreeOgreManualObjects.size() - 1;
				ogreManualObject = mFreeOgreManualObjects[newNumberOfFreeOgreManualObjects];
				mFreeOgreManualObjects.resize(newNumberOfFreeOgreManualObjects);
				ogreManualObject->getParentSceneNode()->setVisible(true);
			}

			// Update icon geometry
			// TODO(co) Performance: Crash inside OGRE 2.1 when using "Ogre::ManualObject::beginUpdate(0)". Another OGRE bug? Need to investigate.
			ogreManualObject->clear();
			ogreManualObject->begin(materialName, Ogre::v1::RenderOperation::OT_TRIANGLE_LIST);
			// ogreManualObject->setDatablock(0, materialName);
			// ogreManualObject->beginUpdate(0);
		}

		// Fill geometry
		if (nullptr != ogreManualObject)
		{
			const glm::vec2 clipSpaceMin(0.0f, -clipSpaceSize.y);
			const glm::vec2 clipSpaceMax(clipSpaceSize.x, 0.0f);
			const Ogre::ColourValue& ogreColour = Ogre::ColourValue::White;

			ogreManualObject->position(Ogre::Vector3(clipSpaceMin.x, clipSpaceMax.y, 0.0f));
			ogreManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));
			ogreManualObject->colour(ogreColour);

			ogreManualObject->position(Ogre::Vector3(clipSpaceMax.x, clipSpaceMax.y, 0.0f));
			ogreManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));
			ogreManualObject->colour(ogreColour);

			ogreManualObject->position(Ogre::Vector3(clipSpaceMin.x, clipSpaceMin.y, 0.0f));
			ogreManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));
			ogreManualObject->colour(ogreColour);

			ogreManualObject->position(Ogre::Vector3(clipSpaceMax.x, clipSpaceMin.y, 0.0f));
			ogreManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));
			ogreManualObject->colour(ogreColour);

			ogreManualObject->index(0);
			ogreManualObject->index(1);
			ogreManualObject->index(2);
			ogreManualObject->index(2);
			ogreManualObject->index(1);
			ogreManualObject->index(3);

			ogreManualObject->end();

			// We need to reset the AABB after each manual object update
			ogreManualObject->setLocalAabb(Ogre::Aabb::BOX_INFINITE);
		}

		{ // Update OGRE manual object sections
			// The QSF material property "DrawInClipSpace" of "qsf_unlit_mesh" is set elsewhere
			// -> There's no real need in here to call "Ogre::ManualObject::setUseIdentityProjection()" or "Ogre::ManualObject::setUseIdentityView()", but it doesn't hurt either to provide this information
			const unsigned int numSections = ogreManualObject->getNumSections();
			for (unsigned int sectionIndex = 0; sectionIndex < numSections; ++sectionIndex)
			{
				Ogre::ManualObject::ManualObjectSection* ogreManualObjectSection = ogreManualObject->getSection(sectionIndex);
				ogreManualObjectSection->setUseIdentityProjection(true);
				ogreManualObjectSection->setUseIdentityView(true);
			}
		}

		// Done
		return ogreManualObject;
	}

	void CommandTargetManager::returnRentedOgreManualObject(Ogre::ManualObject& ogreManualObject)
	{
		// Hide scene node
		ogreManualObject.getParentSceneNode()->setVisible(false);

		// We got one OGRE manual object more in stock, ready to be rented
		mFreeOgreManualObjects.push_back(&ogreManualObject);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
