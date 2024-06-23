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

	void RigidbodyContact::SetBodyData(Rigidbody *one, Rigidbody *two, Real friction, Real restitution)
	{
		this->bodies[0] = one;
		this->bodies[1] = two;
		this->friction = friction;
		this->restitution = restitution;
	}

	void RigidbodyContact::ApplyPositionChange(std::array<Vec3, 2>& linearChange, std::array<Vec3, 2>& angularChange, Real penetration)
	{
		VXM_PROFILE_FUNCTION();
		const Real angularLimit = (Real)0.2f;
		std::array<Real,2> angularMove;
		std::array<Real,2> linearMove;

		Real totalInertia = 0;
		std::array<Real,2> linearInertia;
		std::array<Real,2> angularInertia;

		// We need to work out the inertia of each object in the direction
		// of the contact normal, due to angular inertia only.
		for (unsigned i = 0; i < 2; i++) if (bodies[i])
			{
				Mat3 inverseInertiaTensor = bodies[i]->GetInverseInertiaTensorWorld();

				// Use the same procedure as for calculating frictionless
				// velocity change to work out the angular inertia.
				Vec3 angularInertiaWorld = Math::Cross(m_RelativeContactPosition[i], contactNormal);
				angularInertiaWorld = inverseInertiaTensor * angularInertiaWorld;
				angularInertiaWorld = Math::Cross(angularInertiaWorld, m_RelativeContactPosition[i]);
				angularInertia[i] = Math::Dot(angularInertiaWorld, contactNormal);

				// The linear component is simply the inverse mass
				linearInertia[i] = bodies[i]->GetInverseMass();

				// Keep track of the total inertia from all components
				totalInertia += linearInertia[i] + angularInertia[i];

				// We break the loop here so that the totalInertia value is
				// completely calculated (by both iterations) before
				// continuing.
			}

		// Loop through again calculating and applying the changes
		for (unsigned i = 0; i < 2; i++) if (bodies[i])
			{
				// The linear and angular movements required are in proportion to
				// the two inverse inertias.
				Real sign = (i == 0)?1:-1;
				angularMove[i] =
						sign * penetration * (angularInertia[i] / totalInertia);
				linearMove[i] =
						sign * penetration * (linearInertia[i] / totalInertia);

				// To avoid angular projections that are too great (when mass is large
				// but inertia tensor is small) limit the angular move.
				Vec3 projection = m_RelativeContactPosition[i];
				projection += contactNormal * -Math::Dot(m_RelativeContactPosition[i], contactNormal);

				// Use the small angle approximation for the sine of the angle (i.e.
				// the magnitude would be sine(angularLimit) * projection.magnitude
				// but we approximate sine(angularLimit) to angularLimit).
				Real maxMagnitude = angularLimit * Math::Magnitude(projection);

				if (angularMove[i] < -maxMagnitude)
				{
					Real totalMove = angularMove[i] + linearMove[i];
					angularMove[i] = -maxMagnitude;
					linearMove[i] = totalMove - angularMove[i];
				}
				else if (angularMove[i] > maxMagnitude)
				{
					Real totalMove = angularMove[i] + linearMove[i];
					angularMove[i] = maxMagnitude;
					linearMove[i] = totalMove - angularMove[i];
				}

				// We have the linear amount of movement required by turning
				// the rigid body (in angularMove[i]). We now need to
				// calculate the desired rotation to achieve that.
				if (angularMove[i] == 0)
				{
					// Easy case - no angular movement means no rotation.
					angularChange[i] = {0,0,0};
				}
				else
				{
					// Work out the direction we'd like to rotate in.
					Vec3 targetAngularDirection =
							Math::Cross(m_RelativeContactPosition[i], contactNormal);

					Mat3 inverseInertiaTensor;
					bodies[i]->GetInverseInertiaTensorWorld(inverseInertiaTensor);

					// Work out the direction we'd need to rotate to achieve that
					angularChange[i] =
							(inverseInertiaTensor * targetAngularDirection) *
							(angularMove[i] / angularInertia[i]);
				}

				// Velocity change is easier - it is just the linear movement
				// along the contact normal.
				linearChange[i] = contactNormal * linearMove[i];

				// Now we can start to apply the values we've calculated.
				// Apply the linear movement
				Vec3 pos = bodies[i]->GetPosition();
				pos += contactNormal * linearMove[i];
				bodies[i]->SetPosition(pos);

				// And the change in orientation
				Quat q = bodies[i]->GetOrientation();
				Math::AddScaledVector(q, angularChange[i], Real(1.0));
				bodies[i]->SetOrientation(q);
			}
	}

	void RigidbodyContact::ApplyVelocityChange(std::array<Vec3, 2>& linearChange, std::array<Vec3, 2>& angularChange)
	{
		VXM_PROFILE_FUNCTION();
		// Get hold of the inverse mass and inverse inertia tensor, both in
		// world coordinates.
		std::array<Mat3,2> inverseInertiaTensor;
		bodies[0]->GetInverseInertiaTensorWorld(inverseInertiaTensor[0]);
		if (bodies[1]) {
			bodies[1]->GetInverseInertiaTensorWorld(inverseInertiaTensor[1]);
		}

		// We will calculate the impulse for each contact axis
		Vec3 impulseContact;

		if (friction == (Real)0.0)
		{
			// Use the short format for frictionless contacts
			impulseContact = CalculateFrictionlessImpulse(inverseInertiaTensor);
		}
		else
		{
			// Otherwise we may have impulses that aren't in the direction of the
			// contact, so we need the more complex version.
			impulseContact = CalculateFrictionImpulse(inverseInertiaTensor);
		}

		// Convert impulse to world coordinates
		Vec3 impulse = m_ContactToWorld * impulseContact;

		// Split in the impulse into linear and rotational components
		Vec3 impulsiveTorque = Math::Cross(m_RelativeContactPosition[0], impulse);
		angularChange[0] = inverseInertiaTensor[0] * impulsiveTorque;
		linearChange[0] = {0,0,0};
		linearChange[0] += impulse * bodies[0]->GetInverseMass();

		// Apply the changes
		bodies[0]->AddLinearVelocity(linearChange[0]);
		bodies[0]->AddAngularVelocity(angularChange[0]);

		Vec3 linear, angular;

		linear = bodies[0]->GetLinearVelocity();
		if(Math::Abs(linear.x) < c_VelocityLimit) {linear.x = 0;}
		if(Math::Abs(linear.y) < c_VelocityLimit) {linear.y = 0;}
		if(Math::Abs(linear.z) < c_VelocityLimit) {linear.z = 0;}
		bodies[0]->SetLinearVelocity(linear);

		angular = bodies[0]->GetAngularVelocity();
		if(Math::Abs(angular.x) < c_VelocityLimit) {angular.x = 0;}
		if(Math::Abs(angular.y) < c_VelocityLimit) {angular.y = 0;}
		if(Math::Abs(angular.z) < c_VelocityLimit) {angular.z = 0;}
		bodies[0]->SetAngularVelocity(angular);

		if (bodies[1])
		{
			// Work out body one's linear and angular changes
			Vec3 impulsiveTorque = Math::Cross(impulse, m_RelativeContactPosition[1]);
			angularChange[1] = inverseInertiaTensor[1] * impulsiveTorque;
			linearChange[1] = {0,0,0};
			linearChange[1] += impulse * -bodies[1]->GetInverseMass();

			// And apply them.
			bodies[1]->AddLinearVelocity(linearChange[1]);
			bodies[1]->AddAngularVelocity(angularChange[1]);


			linear = bodies[1]->GetLinearVelocity();
			if(Math::Abs(linear.x) < c_VelocityLimit) {linear.x = 0;}
			if(Math::Abs(linear.y) < c_VelocityLimit) {linear.y = 0;}
			if(Math::Abs(linear.z) < c_VelocityLimit) {linear.z = 0;}
			bodies[1]->SetLinearVelocity(linear);

			angular = bodies[1]->GetAngularVelocity();
			if(Math::Abs(angular.x) < c_VelocityLimit) {angular.x = 0;}
			if(Math::Abs(angular.y) < c_VelocityLimit) {angular.y = 0;}
			if(Math::Abs(angular.z) < c_VelocityLimit) {angular.z = 0;}
			bodies[1]->SetAngularVelocity(angular);
		}
	}


	Vec3 RigidbodyContact::CalculateFrictionlessImpulse(const std::array<Mat3, 2>& inverseInertiaTensor)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 impulseContact;

		// Build a vector that shows the change in velocity in
		// world space for a unit impulse in the direction of the contact
		// normal.
		Vec3 deltaVelWorld = Math::Cross(m_RelativeContactPosition[0], contactNormal);
		deltaVelWorld = inverseInertiaTensor[0] * (deltaVelWorld);
		deltaVelWorld = Math::Cross(deltaVelWorld, m_RelativeContactPosition[0]);

		// Work out the change in velocity in contact coordiantes.
		Real deltaVelocity = Math::Dot(deltaVelWorld, contactNormal);

		// Add the linear component of velocity change
		deltaVelocity += bodies[0]->GetInverseMass();

		// Check if we need to the second body's data
		if (bodies[1])
		{
			// Go through the same transformation sequence again
			Vec3 deltaVelWorld = Math::Cross(m_RelativeContactPosition[1], contactNormal);
			deltaVelWorld = inverseInertiaTensor[1] * deltaVelWorld;
			deltaVelWorld = Math::Cross(deltaVelWorld, m_RelativeContactPosition[1]);

			// Add the change in velocity due to rotation
			deltaVelocity += Math::Dot(deltaVelWorld, contactNormal);

			// Add the change in velocity due to linear motion
			deltaVelocity += bodies[1]->GetInverseMass();
		}

		// Calculate the required size of the impulse
		impulseContact.x = m_DesiredDeltaVelocity / deltaVelocity;
		impulseContact.y = 0;
		impulseContact.z = 0;
		return impulseContact;
	}

	Vec3 RigidbodyContact::CalculateFrictionImpulse(const std::array<Mat3, 2>& inverseInertiaTensor)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 impulseContact;
		Real inverseMass = bodies[0]->GetInverseMass();

		// The equivalent of a cross product in matrices is multiplication
		// by a skew symmetric matrix - we build the matrix for converting
		// between linear and angular quantities.
		Mat3 impulseToTorque = Math::SetSkewSymmetric(m_RelativeContactPosition[0]);

		// Build the matrix to convert contact impulse to change in velocity
		// in world coordinates.
		Mat3 deltaVelWorld = impulseToTorque;
		deltaVelWorld *= inverseInertiaTensor[0];
		deltaVelWorld *= impulseToTorque;
		deltaVelWorld *= -1;

		// Check if we need to add body two's data
		if (bodies[1])
		{
			// Set the cross product matrix
			impulseToTorque = Math::SetSkewSymmetric(m_RelativeContactPosition[1]);

			// Calculate the velocity change matrix
			Mat3 deltaVelWorld2 = impulseToTorque;
			deltaVelWorld2 *= inverseInertiaTensor[1];
			deltaVelWorld2 *= impulseToTorque;
			deltaVelWorld2 *= -1;

			// Add to the total delta velocity.
			deltaVelWorld += deltaVelWorld2;

			// Add to the inverse mass
			inverseMass += bodies[1]->GetInverseMass();
		}

		// Do a change of basis to convert into contact coordinates.
		Mat3 deltaVelocity = glm::transpose(m_ContactToWorld);
		deltaVelocity *= deltaVelWorld;
		deltaVelocity *= m_ContactToWorld;

		// Add in the linear velocity change
		deltaVelocity[0][0] += inverseMass;
		deltaVelocity[1][1] += inverseMass;
		deltaVelocity[2][2] += inverseMass;

		// Invert to get the impulse needed per unit velocity
		Mat3 impulseMatrix = Math::Inverse(deltaVelocity);

		// Find the target velocities to kill
		Vec3 velKill(m_DesiredDeltaVelocity,
						-m_ContactVelocity.y,
						-m_ContactVelocity.z);

		// Find the impulse to kill target velocities
		impulseContact = impulseMatrix * (velKill);

		// Check for exceeding friction
		Real planarImpulse = Math::Sqrt(
				impulseContact.y*impulseContact.y +
				impulseContact.z*impulseContact.z
		);
		if (planarImpulse > impulseContact.x * friction)
		{
			// We need to use dynamic friction
			impulseContact.y /= planarImpulse;
			impulseContact.z /= planarImpulse;

			impulseContact.x = deltaVelocity[0][0] +
							   deltaVelocity[1][0]*friction*impulseContact.y +
							   deltaVelocity[2][0]*friction*impulseContact.z;
			impulseContact.x = m_DesiredDeltaVelocity / impulseContact.x;
			impulseContact.y *= friction * impulseContact.x;
			impulseContact.z *= friction * impulseContact.x;
		}
		return impulseContact;
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

		//VXM_CORE_ASSERT(Math::Abs(Math::SqrMagnitude(contactNormal)-1) < 0.001, "The vector contact normal is not normalized.");

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

	RigidbodyContact& CollisionData::operator[](uint64_t index)
	{
		return contacts[index];
	}

	const RigidbodyContact& CollisionData::operator[](uint64_t index) const
	{
		return contacts[index];
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