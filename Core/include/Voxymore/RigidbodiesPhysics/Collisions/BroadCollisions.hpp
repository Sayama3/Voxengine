//
// Created by ianpo on 26/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingObject.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	struct PotentialContact
	{
		std::array<std::pair<Rigidbody*, Entity>, 2> bodies;
	};

	template<class BoundingClass>
	class BVHNode
	{
	public:
		BVHNode(BVHNode<BoundingClass>* parent, Rigidbody* body, BoundingClass volume);
		BVHNode(BVHNode<BoundingClass>* parent, Rigidbody* body, Entity e, BoundingClass volume);
		~BVHNode();
		inline BVHNode(const BVHNode<BoundingClass>&) = delete;
		inline BVHNode<BoundingClass>& operator=(const BVHNode<BoundingClass>&) = delete;
	public:
		BVHNode<BoundingClass>* parent = nullptr;
		std::array<BVHNode<BoundingClass>* ,2> children = {nullptr, nullptr};
		BoundingClass volume;

		Rigidbody* body = nullptr;
		Entity entity = Entity();

		inline bool IsLeaf() const { return body != nullptr; }

		bool Overlaps(const BVHNode<BoundingClass>& other) const;
		bool Overlaps(const BVHNode<BoundingClass>* other) const;
		uint32_t GetPotentialContacts(std::vector<PotentialContact>& contacts) const;
		uint32_t GetPotentialContactsWith(const BVHNode<BoundingClass>* other, std::vector<PotentialContact>& contacts) const;

		void Insert(Rigidbody* newBody, Entity newEntity, const BoundingClass& newVolume);
		void RecalculateBoundingVolume();
	};

	template<class BoundingClass>
	BVHNode<BoundingClass>::BVHNode(BVHNode<BoundingClass> *parent, Rigidbody *body, BoundingClass volume) : parent(parent), body(body), volume(volume)
	{

	}

	template<class BoundingClass>
	BVHNode<BoundingClass>::BVHNode(BVHNode<BoundingClass> *parent, Rigidbody *body, Entity e, BoundingClass volume) : parent(parent), body(body), entity(e), volume(volume)
	{

	}

	template<class BoundingClass>
	BVHNode<BoundingClass>::~BVHNode()
	{
		if(parent)
		{
			BVHNode<BoundingClass>* sibling;
			sibling = (parent->children[0] == this) ? parent->children[1] : parent->children[0];

			parent->volume = sibling->volume;
			parent->body = sibling->body;

			parent->children[0] = sibling->children[0];
			parent->children[1] = sibling->children[1];

			sibling->parent = nullptr;
			sibling->children[0] = nullptr;
			sibling->children[1] = nullptr;
			delete sibling;

			parent->RecalculateBoundingVolume();
		}

		if(children[0])
		{
			children[0]->parent = nullptr;
			delete children[0];
		}

		if(children[1])
		{
			children[1]->parent = nullptr;
			delete children[1];
		}
	}

	template<class BoundingClass>
	bool BVHNode<BoundingClass>::Overlaps(const BVHNode<BoundingClass>& other) const
	{
		VXM_PROFILE_FUNCTION();
		return volume.Overlaps(other.volume);
	}

	template<class BoundingClass>
	bool BVHNode<BoundingClass>::Overlaps(const BVHNode<BoundingClass>* other) const
	{
		VXM_PROFILE_FUNCTION();
		return volume.Overlaps(other->volume);
	}

	template<class BoundingClass>
	uint32_t BVHNode<BoundingClass>::GetPotentialContacts(std::vector<PotentialContact>& contacts) const
	{
		VXM_PROFILE_FUNCTION();
		// Checking if there is a reason to do the checks
		if(IsLeaf()) return 0;

		// If we should do the check, check that we do have children. We theoretically should have some.
		VXM_CORE_CHECK(children[0] != nullptr && children[1] != nullptr, "The children are null.");
		if(children[0] == nullptr || children[1] == nullptr) return 0;

		return children[0]->GetPotentialContactsWith(children[1], contacts);
	}

	template<class BoundingClass>
	uint32_t BVHNode<BoundingClass>::GetPotentialContactsWith(const BVHNode<BoundingClass> *other, std::vector<PotentialContact>& contacts) const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(other != nullptr, "The other node is null.");
		if(other == nullptr) return 0;

		if(!Overlaps(other)) {
			uint32_t count = 0;
			if (!IsLeaf() && children[0] && children[1]) {
				count += children[0]->GetPotentialContactsWith(children[1], contacts);
			}
			if (!other->IsLeaf() && other->children[0] && other->children[1]) {
				count += other->children[0]->GetPotentialContactsWith(other->children[1], contacts);
			}
			return count;
		}

		// If both leaves and we have a contact, there is a contact.
		if(IsLeaf() && other->IsLeaf())
		{
			PotentialContact contact;
			contact.bodies[0] = {body, entity};
			contact.bodies[1] = {other->body, other->entity};
			contacts.push_back(contact);
			return 1;
		}

		if(other->IsLeaf() || (!IsLeaf() && volume.GetSize() >= other->volume.GetSize()))
		{
			uint32_t count = children[0]->GetPotentialContactsWith(other, contacts);
			return count + children[1]->GetPotentialContactsWith(other, contacts);
		}
		else
		{
			uint32_t count = GetPotentialContactsWith(other->children[0], contacts);
			return count + GetPotentialContactsWith(other->children[1], contacts);
		}
	}


	template<class BoundingClass>
	void BVHNode<BoundingClass>::Insert(Rigidbody* newBody, Entity newEntity, const BoundingClass& newVolume)
	{
		if(IsLeaf())
		{
			children[0] = new BVHNode<BoundingClass>(this, body, entity, volume);
			children[1] = new BVHNode<BoundingClass>(this, newBody, newEntity, newVolume);
			body = nullptr;
			entity = Entity();
			RecalculateBoundingVolume();
		}
		else
		{
			if(children[0]->volume.GetGrowth(newVolume) < children[1]->volume.GetGrowth(newVolume))
			{
				children[0]->Insert(newBody, newEntity, newVolume);
			}
			else
			{
				children[1]->Insert(newBody, newEntity, newVolume);
			}
		}
	}

	template<class BoundingClass>
	void BVHNode<BoundingClass>::RecalculateBoundingVolume()
	{
		volume = BoundingClass(children[0]->volume, children[1]->volume);
	}
}
