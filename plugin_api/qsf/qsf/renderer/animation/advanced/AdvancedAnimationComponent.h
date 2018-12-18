// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"
#include "qsf/renderer/animation/advanced/AnimationNode.h"
#include "qsf/renderer/animation/advanced/SkeletonGroup.h"
#include "qsf/renderer/animation/advanced/BoneMaskHandle.h"
#include "qsf/renderer/mesh/MeshComponent.h"
#include "qsf/map/Entity.h"
#include "qsf/job/JobProxy.h"
#include "qsf/base/WeakPtr.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
		class OldBone;
		class Animation;
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
	*    Advanced animation component class
	*
	*  @note
	*    - Pure virtual base class for all advanced animation components classes
	*    - We get a system for a node based skeleton altering pipeline
	*/
	class QSF_API_EXPORT AdvancedAnimationComponent : public Component, public JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class AnimationNode;	// We prepared private methods for the animation node base class
		friend class SkeletonGroup;	// We prepared private methods for the skeleton group


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::AdvancedAnimationComponent" unique component ID


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
		inline explicit AdvancedAnimationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AdvancedAnimationComponent();

		/**
		*  @brief
		*    Give back a handle to the output skeleton of the root node
		*
		*  @return
		*    Pointer to a constant skeleton handle, can be a null pointer if no root is connected or the output skeleton wasn't calculated because we are invisible
		*
		*  @note
		*    - Get your information of the output skeleton with a job in ANIMATION at ORDERING_LATE for this frame
		*/
		inline const SkeletonHandle* getRootOutputSkeleton() const;

		/**
		*  @brief
		*    Give back the bone ID from the bone name
		*
		*  @param[in] name
		*    Name of the bone OGRE uses
		*
		*  @return
		*    Bone ID we use in nodes, uninitialized if we cannot find the bone
		*
		*  @note
		*    - We don't use strings internally, if we want to give an animation node a bone we need to give them the bone ID, use this function to get it
		*/
		uint16 getBoneId(const std::string& name) const;

		/**
		*  @brief
		*    Give back the bone ID from an OGRE handle
		*
		*  @param[in] ogreHandle
		*    16 bit OGRE handle identifier
		*
		*  @return
		*    Bone ID we use in nodes, uninitialized if we cannot find the bone
		*/
		inline uint16 getBoneId(uint16 ogreHandle) const;

		/**
		*  @brief
		*    Give back the number of bones of our skeleton
		*/
		uint16 getBoneNumber() const;

		/**
		*  @brief
		*    Check if we calculate the animation every frame without forcing ourself
		*
		*  @return
		*    "true" if we are calculating animation normally, "false" if we are invisible and the entity gets not rendered
		*/
		bool isCalculating();

		/**
		*  @brief
		*    We can force an immediate calculation
		*
		*  @note
		*    - Use this function to get results of calculations for invisible entities
		*/
		void forceCalculation(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Give us an empty bone mask handle, can be used with all skeletons in the same skeletongroup
		*
		*  @return
		*    Bone mask handle
		*/
		inline BoneMaskHandle createBoneMask();

		/**
		*  @brief
		*    Remove and unregister a bone mask, be sure every node using this bone mask was removed
		*
		*  @param[in] boneMask
		*    Reference to bone mask
		*/
		inline void removeBoneMask(BoneMaskHandle& boneMask);

		/**
		*  @brief
		*    Check if the given bone mask has the same skeleton group and has a valid id
		*/
		inline bool checkBoneMask(const BoneMaskHandle& boneMask) const;

		/**
		*  @brief
		*    Check if we update "onNotRendered" jobs for calculation when we are not visible
		*
		*  @return
		*    "true" if the job is used, "false" if we disabled this feature
		*/
		inline bool isUpdatedOnNotRendered() const;

		/**
		*  @brief
		*    Set if we update "onNotRendered" jobs for calculation when we are not visible or not
		*
		*  @param[in] updatedOnNotRendered
		*    Flag to set this feature on or off
		*
		*  @note
		*    - When we activate this feature the job will start the next time the object got invisible again and not immediately
		*/
		void setUpdatedOnNotRendered(bool updatedOnNotRendered);


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Internal problem state set flags
		*/
		enum Problem
		{
			CIRCULAR_DEPENDANCY_PROBLEM		= 1 << 0,	// We have a circular dependency between nodes
			EMPTY_OUTPUT_PROBLEM			= 1 << 1,	// We have nodes who got no valid output skeleton
			SUBOPTIMAL_STRUCTURE_PROBLEM	= 1 << 2,	// We have a non optimized node structure who could be improved
			UNUSED_OUTPUT_PROBLEM			= 1 << 3	// We have an output who is not used, probably a node who is not connected to root but to a node
		};
		typedef BitFlagSet<uint8, Problem> ProblemBitmask;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AdvancedAnimationComponent methods ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Before we calculate the animation nodes we need to set all parameters
		*
		*  @param[in] jobArguments
		*    Arguments from job manager like time passed
		*
		*  @return
		*    "true" if pre calculation succeeded, "false" if it failed and we don't need to calculate the nodes anymore
		*
		*  @note
		*    - Implement this function in any animation component
		*    - This function isn't called if the entity isn't visible
		*/
		virtual bool onPreCalculation(const JobArguments& jobArguments) = 0;

		/**
		*  @brief
		*    After we calculate the animation nodes we need to check for errors and other results
		*
		*  @param[in] jobArguments
		*    Arguments from job manager like time passed
		*
		*  @note
		*    - Implement this function in any animation component
		*    - This function isn't called if the entity isn't visible
		*/
		virtual void onPostCalculation(const JobArguments& jobArguments) = 0;

		/**
		*  @brief
		*    When we don't render our animation we can still for example update our times
		*
		*  @param[in] jobArguments
		*    Arguments from job manager like time passed
		*
		*  @note
		*    - Implement this function in any animation component
		*    - This function is only called if the entity isn't visible
		*/
		virtual void onNotRendered(const JobArguments& jobArguments) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get the number of animation nodes saved in this component
		*/
		inline uint16 getAnimationNodeNumber() const;

		/**
		*  @brief
		*    Get a vector to all animation nodes
		*/
		inline const std::vector<AnimationNode*>& getAnimationNodesVector() const;

		/**
		*  @brief
		*    Create a new animation node
		*
		*  @param[template] T
		*    A class which derives from animation node
		*
		*  @return
		*    A reference to the created animation node
		*/
		template<class T> T& createAnimationNode();

		/**
		*  @brief
		*    Create a new animation node with a certain number of inputs
		*
		*  @param[template] T
		*    A class which derives from animation node and has a constructor for different number of input slots
		*  @param[in] numberOfInputSlots
		*    The number of inputs we want for this node
		*
		*  @return
		*    A reference to the created animation node
		*/
		template<class T> T& createAnimationNode(uint16 numberOfInputSlots);

		/**
		*  @brief
		*    Remove an animation node and delete it
		*
		*  @param[in] animationNode
		*    A reference to the animation node we want to remove
		*
		*  @note
		*    - The reference will be invalid after the node was removed
		*/
		void removeAnimationNode(AnimationNode& animationNode);

		/**
		*  @brief
		*    Remove all animation nodes and delete them
		*/
		void removeAllAnimationNodes();

		/**
		*  @brief
		*    Connect the output of an animation node with an input slot of an other animation node
		*
		*  @param[in] outputAnimationNode
		*    Animation node where we want to connect the output
		*  @param[in] inputAnimationNode
		*    Animation node where we want to connect a input slot
		*  @param[in] inputId
		*    ID of the slot of our input animation node, invalid ids will create no connection
		*/
		void connectNodes(AnimationNode& outputAnimationNode, AnimationNode& inputAnimationNode, uint16 inputId = 0);

		/**
		*  @brief
		*    Connect the output of an animation node to our root, where we take the skeleton and give to ogre to render it
		*
		*  @param[in] outputAnimationNode
		*    Animation node we want to connect to root output
		*/
		void connectToRoot(AnimationNode& outputAnimationNode);

		/**
		*  @brief
		*    Disconnect two nodes from each other if they are connected
		*
		*  @param[in] outputAnimationNode
		*    Animation node where we want to disconnect the output
		*  @param[in] inputAnimationNode
		*    Animation node where we take want to disconnect the input
		*/
		void disconnectNodes(AnimationNode& outputAnimationNode, AnimationNode& inputAnimationNode);

		/**
		*  @brief
		*    Disconnect the output node which is connected to the given input slot
		*
		*  @param[in] inputAnimationNode
		*    Animation node where we take want to disconnect the input
		*  @param[in] inputId
		*    Input slot ID
		*/
		void disconnectInputSlot(AnimationNode& inputAnimationNode, uint16 inputId);

		/**
		*  @brief
		*    Give back the node connected to a input slot
		*
		*  @param[in] inputId
		*    ID of an input slot, with an undefined or invalid ID nothing will be connected
		*
		*  @return
		*    If no output is connected to this input slot we will return a null pointer
		*/
		AnimationNode* getConnectedNode(AnimationNode& inputAnimationNode, uint16 inputId = 0) const;

		/**
		*  @brief
		*    Give back the node connected to root, where we take the output and render it
		*
		*  @return
		*    If we don't have a node connected to root we will return a null pointer
		*/
		AnimationNode* getConnectedRootNode() const;

		/**
		*  @brief
		*    Check if two nodes are connected
		*
		*  @param[in] outputAnimationNode
		*    Animation node where we want to check if the output is connected to our input
		*  @param[in] inputAnimationNode
		*    Animation node where we want to check if the input is connected to our output
		*  @param[out] inputId (optional)
		*    Give back the input slot ID where the two nodes are connected
		*
		*  @return
		*    "true" if we are connected, "false" if not
		*/
		bool isConnected(AnimationNode& outputAnimationNode, AnimationNode& inputAnimationNode, uint16& inputId) const;
		bool isConnected(AnimationNode& outputAnimationNode, AnimationNode& inputAnimationNode) const;

		/**
		*  @brief
		*    Check if a nodes is connected through a connection path to another node
		*
		*  @param[in] outputAnimationNode
		*    Animation node we want to check
		*  @param[in] outputAnimationNode
		*    Animation node we want to check
		*
		*  @return
		*    "true" if we are connected, "false" if not
		*/
		bool isConnectedWithPath(AnimationNode& outputAnimationNode, AnimationNode& inputAnimationNode) const;

		/**
		*  @brief
		*    Check if a nodes is connected through a connection path to our root
		*
		*  @param[in] animationNode
		*    Animation node we want to check
		*
		*  @return
		*    "true" if we are connected, "false" if not
		*/
		bool isConnectedWithPathToRoot(AnimationNode& outputAnimationNode) const;

		/*
		*  @brief
		*    Check if a problem happened
		*
		*  @param[in] problem
		*    The problem we want to check from our enum
		*
		*  @return
		*    true if no problem happened and false if the given problem happened
		*
		*  @note
		*    - Use this function to check if problems connected with the node structure happened, we can check for circular dependency, unused output ec.
		*/
		inline bool checkProblem(Problem problem) const;

		/*
		*  @brief
		*    Give back the time length of a key frame skeleton file
		*
		*  @param[in] animationName
		*    Key frame skeleton animation asset name
		*
		*  @return
		*    Give back the time length
		*/
		inline qsf::Time getAnimationTimeLength(const std::string& animationName) const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;	// Overwritten functions from component to startup and shutdown the component
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Component* getMovableObjectRenderingListenerComponent() override;	// The component we use to check if we are visible
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods for qsf::AnimationNode                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Give us the skeleton group
		*/
		inline SkeletonGroup* getSkeletonGroup();

		/**
		*  @brief
		*    Add a new skeleton instance and fill it with the binding pose
		*/
		inline uint16 registerSkeletonId();

		/**
		*  @brief
		*    Remove all skeletons
		*/
		inline void unregisterSkeletonId(uint16 skeletonId);

		/**
		*  @brief
		*    Tell the component possible problems which happened
		*/
		inline void tellProblem(Problem problem);

		/*
		*  @brief
		*    Add the offset from a key frame skeleton file to a given skeleton
		*
		*  @param[in] animationName
		*    Key frame skeleton animation asset proxy, where we read our key frames
		*  @param[in] toSkeletonId
		*    ID of our skeleton where we add the offset
		*  @param[in] progress
		*    Should be a value between 0.0 (start of animation) and 1.0 (end of animation)
		*  @param[in] retarget
		*    If we want to retarget an animation from the skeleton data file skeleton to our skeleton
		*/
		void addOffsetFromKeyFrameSkeleton(const std::string& animationName, uint16 toSkeletonId, float progress, bool retarget = false);

		/*
		*  @brief
		*    Add the offset from a key frame skeleton file to the binding pose and give back the result
		*
		*  @param[in] animationName
		*    Key frame skeleton animation asset name where we read our key frames
		*  @param[in] toSkeletonId
		*    ID of our skeleton where we want our result
		*  @param[in] progress
		*    Should be a value between 0.0 (start of animation) and 1.0 (end of animation)
		*  @param[in] retarget
		*    If we want to retarget an animation from the skeleton data file skeleton to our skeleton
		*/
		void addOffsetToBindingPoseFromKeyFrameSkeleton(const std::string& animationName, uint16 toSkeletonId, float progress, bool retarget = false);

		/*
		*  @brief
		*    Give back the time length of a key frame skeleton file
		*
		*  @param[in] animationName
		*    Key frame skeleton animation asset name
		*
		*  @param[in] retarget
		*    If we want to retarget an animation from the skeleton data file skeleton to our skeleton
		*
		*  @return
		*    Give back the time length in seconds
		*/
		float getKeyFrameSkeletonTimeLength(const std::string& animationName, bool retarget = false) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Handler for the job proxy when not visible
		*/
		void onNotRenderedHandler(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Handler for the job proxy
		*/
		void onUpdateAnimationCalculation(const JobArguments& jobArguments);

		/**
		*  @brief
		*    We want to give our animation result to ogre for rendering
		*/
		void updateAnimationCalculation(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Start the calculation of animation nodes (calculating recursive from our root node)
		*
		*  @note
		*    - The calculation is separated from the rendering and not updated automatically in this component
		*/
		void calculate();

		/**
		*  @brief
		*    Handler for the job proxy
		*/
		void onUpdateAnimationToOgre(const JobArguments& jobArguments);

		/**
		*  @brief
		*    We want to give our animation result to OGRE for rendering
		*/
		void updateAnimationToOgre(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Give back a pointer to the OGRE entity, or a null pointer if no OGRE entity was found in a mesh component
		*/
		inline Ogre::v1::Entity* getOgreEntity() const;

		/**
		*  @brief
		*    Is the animation node part of this component
		*/
		bool isAnimationNodeOurs(AnimationNode& animationNode) const;		// TODO(tl): Do we really need to be so paranoid to distrust classes who inherit from us ?

		/**
		*  @brief
		*    Read the skeleton from OGRE and build the bone structure in skeleton group
		*
		*  @return
		*    "false" if we failed to get a skeleton from OGRE, "true" if we succeeded
		*
		*  @todo
		*    - TODO(tl) Remove the old bone structure
		*/
		bool initializeSkeleton();

		/**
		*  @brief
		*    Reset the OGRE skeleton
		*/
		void closeSkeleton();

		/**
		*  @brief
		*    Recursively add bones from OGRE, parent bone ID is from our own bone ID system in skeleton group
		*/
		void addChildBones(const Ogre::v1::OldBone& parentOgreBone, uint16 parentBoneId);

		/**
		*  @brief
		*    Give us the OGRE animation from a key frame skeleton file
		*
		*  @note
		*    - We need to use strings for OGRE to identify an animation
		*/
		Ogre::v1::Animation* getOgreAnimation(const std::string& animationName) const;

		/**
		*  @brief
		*    Retarget the OGRE animation file if possible
		*
		*  @note
		*    - We need to use strings for OGRE to identify an animation
		*/
		Ogre::v1::Animation* getOgreAnimationAndRetarget(const std::string& animationName) const;

		/*
		*  @brief
		*    Add the offset from a key frame skeleton file
		*/
		void addKeyFrameSkeleton(const std::string& animationName, uint16 toSkeletonId, float progress, bool retarget);

		/**
		*  @brief
		*    Check if a nodes is connected through a connection path to another node recursively
		*/
		bool isConnectedWithPathRecursive(AnimationNode& outputAnimationNode, AnimationNode& inputAnimationNode, std::vector<AnimationNode*>& traversed) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<AnimationNode*>	mAnimationNodes;				///< All animation nodes of this component
		AnimationNode*				mRootAnimationNode;				///< Pointer to the root animation node, managed in our vector above

		SkeletonGroup*				mSkeletonGroup;					///< Pointer to the skeleton group, managed by skeleton manager

		JobProxy					mAnimationCalculationJobProxy;	///< Animation calculation job proxy where we calculate the animation
		JobProxy					mAnimationToOgreJobProxy;		///< Animation job proxy where we copy our animations to OGRE
		JobProxy					mAnimationNotRenderedProxy;		///< Animation job proxy when we are not rendered and invisible
		bool						mUpdatedOnNotRendered;			///< Flag to check if we wasn't to call onNotRendered

		ProblemBitmask				mProblemBitMask;				///< Remember problems we had

		mutable WeakPtr<MeshComponent>	mMeshComponent;				///< Weak pointer to our mesh component


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
#include "qsf/renderer/animation/advanced/AdvancedAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AdvancedAnimationComponent)
