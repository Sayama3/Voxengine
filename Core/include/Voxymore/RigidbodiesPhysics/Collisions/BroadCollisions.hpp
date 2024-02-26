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
		Rigidbody* bodies[2];
	};

	template<class BoundingClass>
	class BVHNode
	{
		BVHNode(BVHNode<BoundingClass>* parent, Rigidbody* body, BoundingClass volume);
		~BVHNode();
	public:
		BVHNode<BoundingClass>* parent = nullptr;
		BVHNode<BoundingClass>* children[2] = {nullptr, nullptr};
		BoundingObject<BoundingClass> volume;

		Rigidbody* body = nullptr;

		inline bool IsLeaf() const { return body != nullptr; }

		bool Overlaps(const BVHNode<BoundingClass>& other) const;
		uint32_t GetPotentialContacts(PotentialContact* contacts, uint32_t limit) const;
		uint32_t GetPotentialContactsWith(const BVHNode<BoundingClass>* other,PotentialContact* contacts, uint32_t limit) const;

		void Insert(Rigidbody* newBody, const BoundingClass& newVolume);
		void RecalculateBoundingVolume();
	};

	template<class BoundingClass>
	BVHNode<BoundingClass>::BVHNode(BVHNode<BoundingClass> *parent, Rigidbody *body, BoundingClass volume) : parent(parent), body(body), volume(volume)
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
	bool BVHNode<BoundingClass>::Overlaps(const BVHNode<BoundingClass> &other) const
	{
		VXM_PROFILE_FUNCTION();
		return volume.Overlaps(other.volume);
	}

	template<class BoundingClass>
	uint32_t BVHNode<BoundingClass>::GetPotentialContacts(PotentialContact *contacts, uint32_t limit) const
	{
		VXM_PROFILE_FUNCTION();
		// Checking if there is a reason to do the checks
		if(limit == 0 || IsLeaf()) return 0;

		// If we should do the check, check that we do have children. We theoretically should have some.
		VXM_CORE_ASSERT(children[0] != nullptr && children[1] != nullptr, "The children are null.");
		if(children[0] == nullptr || children[1] == nullptr) return 0;

		return children[0]->GetPotentialContactsWith(children[1], contacts, limit);
	}

	template<class BoundingClass>
	uint32_t BVHNode<BoundingClass>::GetPotentialContactsWith(const BVHNode<BoundingClass> *other, PotentialContact *contacts, uint32_t limit) const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(other != nullptr, "The other node is null.");
		if(other == nullptr) return 0;
		if(limit == 0 || !Overlaps(other)) return 0;

		// If both leaves and we have a contact, there is a contact.
		if(IsLeaf() && other->IsLeaf())
		{
			contacts->bodies[0] = body;
			contacts->bodies[1] = other->body;
			return 1;
		}

		if(other->IsLeaf() || (IsLeaf() && volume.GetSize() >= other->volume.GetSize()))
		{
			uint32_t count = children[0]->GetPotentialContactsWith(other, contacts, limit);
			if (limit > count)
			{
				return count + children[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
			}
			else
			{
				return count;
			}
		}
		else
		{
			uint32_t count = GetPotentialContactsWith(other->children[0], contacts, limit);
			if (limit > count)
			{
				return count + GetPotentialContactsWith(other->children[1], contacts + count, limit - count);
			}
			else
			{
				return count;
			}
		}
	}


	template<class BoundingClass>
	void BVHNode<BoundingClass>::Insert(Rigidbody* newBody, const BoundingClass& newVolume)
	{
		if(IsLeaf())
		{
			children[0] = new BVHNode<BoundingClass>(this, volume, body);
			children[1] = new BVHNode<BoundingClass>(this, newBody, newVolume);
			body = nullptr;
			RecalculateBoundingVolume();
		}
		else
		{
			if(children[0]->volume.GetGrowth(newVolume) < children[1]->volume.GetGrowth(newVolume))
			{
				children[0]->Insert(newBody, newVolume);
			}
			else
			{
				children[1]->Insert(newBody, newVolume);
			}
		}
	}

	template<class BoundingClass>
	void BVHNode<BoundingClass>::RecalculateBoundingVolume()
	{
		volume = BoundingObject<BoundingClass>(children[0]->volume, children[1]->volume);
	}
}
