//
// Created by ianpo on 16/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContact.hpp"

namespace Voxymore::Core
{

	namespace Helper
	{
		//		void makeOrthonormalBasis(Vec3& x, Vec3& y, Vec3& z)
		//		{
		//			x = Math::Normalize(x);
		//			z = Math::Cross(x,y);
		//			if(Math::SqrMagnitude(z) == 0) {
		//				VXM_CORE_ERROR("The cross product of x({0}) and y({1}) has resulted in a null vector.", Math::ToString(x), Math::ToString(y));
		//				return;
		//			}
		//			z = Math::Normalize(z);
		//			y = Math::Normalize(Math::Cross(z, x));
		//		}

		void makeOrthonormalBasis(Vec3 &x, Vec3 &y, Vec3 &z)
		{
			const Real s = 1.0 / Math::Sqrt(x.z * x.z + x.y * x.y);

			z.x = x.z * s;
			z.y = 0;
			z.z = -x.x * s;

			y.x = x.y * z.x;
			y.y = x.z * z.x - x.x * z.z;
			y.z = -x.y * z.x;
		}
	}

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

	void RigidbodyContact::CalculateInternals(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		if(!bodies[0]) SwapBodies();
		VXM_CORE_ASSERT(bodies[0], "Cannot calculate internals without any Rigidbodies.");
		if(!bodies[0]) return;

		CalculateContactBasis();

		m_RelativeContactPosition[0] = contactPoint - bodies[0]->GetPosition();
		if(bodies[1])
		{
			m_RelativeContactPosition[1] = contactPoint - bodies[1]->GetPosition();
		}

		m_ContactVelocity = CalculateLocalVelocity(0, ts);
		if(bodies[1])
		{
			m_ContactVelocity -= CalculateLocalVelocity(1, ts);
		}

		CalculateDesiredDeltaVelocity(ts);
	}

	void RigidbodyContact::SwapBodies()
	{
		VXM_PROFILE_FUNCTION();

		contactNormal *= (Real)-1;

		auto* tmp = bodies[0];
		bodies[0] = bodies[1];
		bodies[1] = tmp;
	}

	void RigidbodyContact::CalculateContactBasis()
	{
		VXM_PROFILE_FUNCTION();
		std::array<Vec3, 2> contactTangent {};

		VXM_CORE_ASSERT(Math::Abs(Math::SqrMagnitude(contactNormal)-1) < 0.001, "The vector contact normal is not normalized.");

		if(Math::Abs(contactNormal.x) > Math::Abs(contactNormal.y))
		{
			const Real s = (Real)1.0 / Math::Sqrt(contactNormal.z*contactNormal.z + contactNormal.x * contactNormal.x);

			contactTangent[0].x = contactNormal.z * s;
			contactTangent[0].y = 0;
			contactTangent[0].z = -contactNormal.x* s;

			contactTangent[1].x = contactNormal.y * contactTangent[0].x;
			contactTangent[1].y = contactNormal.z * contactTangent[0].x - contactNormal.x * contactTangent[0].z;
			contactTangent[1].z = -contactNormal.y * contactTangent[0].x;
		}
		else
		{
			const Real s = (Real)1.0 / Math::Sqrt(contactNormal.z*contactNormal.z + contactNormal.y * contactNormal.y);

			contactTangent[0].x = 0;
			contactTangent[0].y = -contactNormal.z * s;
			contactTangent[0].z = contactNormal.y * s;

			contactTangent[1].x = contactNormal.y * contactTangent[0].z - contactNormal.z * contactTangent[0].y;
			contactTangent[1].y = -contactNormal.x * contactTangent[0].z;
			contactTangent[1].z = contactNormal.x * contactTangent[0].y;
		}

		m_ContactToWorld[0] = contactNormal;
		m_ContactToWorld[1] = contactTangent[0];
		m_ContactToWorld[2] = contactTangent[1];
	}

	Vec3 RigidbodyContact::CalculateLocalVelocity(uint32_t bodyIndex, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		Rigidbody& rb = *bodies[bodyIndex];

		Vec3 velocity = Math::Cross(rb.GetAngularVelocity(), m_RelativeContactPosition[bodyIndex]);
		velocity += rb.GetLinearVelocity();

		auto worldToContact = GetWorldToContact();

		Vec3 contactVelocity = worldToContact * velocity;

		Vec3 accVelocity = rb.GetLastFrameAcceleration() * ts.as<Real>();

		accVelocity = worldToContact * accVelocity;

		accVelocity.x = 0;

		contactVelocity += accVelocity;

		return contactVelocity;
	}

	void RigidbodyContact::CalculateDesiredDeltaVelocity(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		Real thisRestitution = restitution;
		if(Math::Abs(m_ContactVelocity.x) < c_VelocityLimit)
		{
			thisRestitution = (Real)0;
		}

		Real accVelocity = Math::Dot(bodies[0]->GetLastFrameAcceleration() * ts.as<Real>(), contactNormal);
		if (bodies[1])
		{
			accVelocity -= Math::Dot(bodies[1]->GetLastFrameAcceleration() * ts.as<Real>(), contactNormal);
		}
		m_DesiredDeltaVelocity = -m_ContactVelocity.x - thisRestitution * (m_ContactVelocity.x - accVelocity);
	}

	const Mat3& RigidbodyContact::GetContactToWorld() const
	{
		return m_ContactToWorld;
	}

	Mat3 RigidbodyContact::GetWorldToContact() const
	{
		return glm::transpose(m_ContactToWorld);
	}

	RigidbodyContact::operator bool() const
	{
		return bodies[0] || bodies[1];
	}

	CollisionData::CollisionData() : contacts()
	{
		contacts.reserve(10);
		// contactsLeft = contacts.size();
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