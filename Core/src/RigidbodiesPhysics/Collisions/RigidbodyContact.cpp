//
// Created by ianpo on 16/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContact.hpp"

namespace Voxymore::Core
{
	/*
	RigidbodyContact::RigidbodyContact(Rigidbody *p0, Vec3 contactNormal, Real restitution, Real penetration) : rigidbodies({p0, nullptr}), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(rigidbodies[0] != nullptr, "The Rigidbody contact MUST have at least one Rigidbody associated.");
	}

	RigidbodyContact::RigidbodyContact(Rigidbody *p0, Rigidbody *p1, Vec3 contactNormal, Real restitution, Real penetration) : rigidbodies({p0, p1}), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(rigidbodies[0] != nullptr, "The Rigidbody contact MUST have at least one Rigidbody associated.");
	}

	RigidbodyContact::RigidbodyContact(Rigidbody* p[2], Vec3 contactNormal, Real restitution, Real penetration) : rigidbodies({p[0], p[1]}), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(rigidbodies[0] != nullptr, "The Rigidbody contact MUST have at least one Rigidbody associated.");
	}

	RigidbodyContact::RigidbodyContact(const std::array<Rigidbody*, 2>& p, Vec3 contactNormal, Real restitution, Real penetration) : rigidbodies(p), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(rigidbodies[0] != nullptr, "The Rigidbody contact MUST have at least one Rigidbody associated.");
	}

	void RigidbodyContact::Resolve(TimeStep duration)
	{
		VXM_PROFILE_FUNCTION();
		ResolveVelocity(duration);
		ResolveInterpenetration(duration);
	}

	Real RigidbodyContact::CalculateSeparatingVelocity() const
	{
		VXM_PROFILE_FUNCTION();
		Vec3 relativeVelocity = rigidbodies[0]->GetLinearVelocity();

		if(rigidbodies[1])
		{
			relativeVelocity -= rigidbodies[1]->GetLinearVelocity();
		}

		return Math::Dot(relativeVelocity, contactNormal);
	}

	void RigidbodyContact::ResolveVelocity(TimeStep duration)
	{
		VXM_PROFILE_FUNCTION();
		// Find the velocity in the direction of the contact.
		auto separatingVelocity = CalculateSeparatingVelocity();

		if(separatingVelocity > 0) // Don't need to be resolved.
		{
			return;
		}

		Real newSepVelocity = -separatingVelocity * restitution;

		Vec3 accCausedVelocity = Vec3{0,-9.81, 0};
		if(rigidbodies[1])
		{
			accCausedVelocity -= Vec3{0,-9.81, 0};
		}
		Real accCausedSepVel = Math::Dot(accCausedVelocity, contactNormal) * duration;
		if(accCausedSepVel < 0)
		{
			newSepVelocity += restitution * accCausedSepVel;
			if(newSepVelocity < 0)
			{
				newSepVelocity = 0;
			}
		}

		Real deltaVelocity = newSepVelocity - separatingVelocity;

		Real totalInverseMass = rigidbodies[0]->GetInverseMass();
		if(rigidbodies[1])
		{
			totalInverseMass += rigidbodies[1]->GetInverseMass();
		}

		// Infinite mass, therefore, no resolving.
		if(totalInverseMass <= 0)
		{
			return;
		}

		Real impulse = deltaVelocity / totalInverseMass;

		Vec3 impulsePerIMass = contactNormal * impulse;

		rigidbodies[0]->AddLinearVelocity(impulsePerIMass * (+rigidbodies[0]->GetInverseMass()));
		if(rigidbodies[1])
		{
			// The other rigidbody should go the other way around.
			rigidbodies[1]->AddLinearVelocity(impulsePerIMass * (-rigidbodies[1]->GetInverseMass()));
		}
	}

	void RigidbodyContact::ResolveInterpenetration(TimeStep duration)
	{
		VXM_PROFILE_FUNCTION();

		if(penetration <= 0)
		{
			// No penetration, doing nothing.
			return;
		}

		Real totalInverseMass = rigidbodies[0]->GetInverseMass();
		if(rigidbodies[1])
		{
			totalInverseMass += rigidbodies[1]->GetInverseMass();
		}

		// Infinite mass, therefore, no resolving.
		if(totalInverseMass <= 0)
		{
			return;
		}

		Vec3 movePerIMass = contactNormal * (penetration * (penetration  / totalInverseMass));

		std::array<Vec3, 2> rigidbodyMovement = {Vec3(0), Vec3(0)};
		rigidbodyMovement[0] = movePerIMass * rigidbodies[0]->GetInverseMass();
		if(rigidbodies[1])
		{
			// Go the other way around.
			rigidbodyMovement[1] = movePerIMass * -rigidbodies[1]->GetInverseMass();
		}

		rigidbodies[0]->AddMovement(rigidbodyMovement[0]);
		if(rigidbodies[1])
		{
			rigidbodies[1]->AddMovement(rigidbodyMovement[1]);
		}
	}
 */

	void RigidbodyContact::SetBodyData(Rigidbody *one, Rigidbody *two, Real friction, Real restitution)
	{
		this->bodies[0] = one;
		this->bodies[1] = two;
		this->friction = friction;
		this->restitution = restitution;
	}

	CollisionData::CollisionData() : contacts(10)
	{
//		contactsLeft = contacts.size();
	}

	CollisionData::~CollisionData()
	{
	}

	void CollisionData::AddContact(int i/* = 1*/)
	{
//		contactsLeft -= i;
	}

	RigidbodyContact* CollisionData::GetContact()
	{
		VXM_PROFILE_FUNCTION();
		contacts.emplace_back();
		return &contacts[contacts.size() - 1];
	}

	RigidbodyContact* CollisionData::GetContact(int i)
	{
		VXM_CORE_ASSERT(i >= 0 && i < contacts.size(), "Contact {0} is not a valid index.");
		if(i < 0 || i >= contacts.size()) return nullptr;
		return &contacts[i];
	}

	void CollisionData::AddContact(const RigidbodyContact& contact)
	{
		VXM_PROFILE_FUNCTION();
		if(contact.bodies[0] || contact.bodies[1]) {
			contacts.push_back(contact);
			AddContact(1);
		}
	}

	void CollisionData::reserve(size_t count)
	{
		contacts.reserve(count);
	}

	bool CollisionData::empty() const
	{
		return contacts.empty();
	}

	size_t CollisionData::size() const
	{
		return contacts.size();
	}

	void CollisionData::clear()
	{
		contacts.clear();
	}
}// namespace Voxymore::Core