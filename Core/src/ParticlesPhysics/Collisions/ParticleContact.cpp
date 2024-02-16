//
// Created by ianpo on 22/01/2024.
//

#include "Voxymore/ParticlesPhysics/Collisions/ParticleContact.hpp"


namespace Voxymore::Core
{
	ParticleContact::ParticleContact(Particle *p0, Vec3 contactNormal, Real restitution, Real penetration) : particles({p0, nullptr}), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(particles[0] != nullptr, "The Particle contact MUST have at least one Particle associated.");
	}

	ParticleContact::ParticleContact(Particle *p0, Particle *p1, Vec3 contactNormal, Real restitution, Real penetration) : particles({p0, p1}), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(particles[0] != nullptr, "The Particle contact MUST have at least one Particle associated.");
	}

	ParticleContact::ParticleContact(Particle* p[2], Vec3 contactNormal, Real restitution, Real penetration) : particles({p[0], p[1]}), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(particles[0] != nullptr, "The Particle contact MUST have at least one Particle associated.");
	}

	ParticleContact::ParticleContact(const std::array<Particle*, 2>& p, Vec3 contactNormal, Real restitution, Real penetration) : particles(p), contactNormal(contactNormal), restitution(restitution), penetration(penetration)
	{
		VXM_CORE_ASSERT(particles[0] != nullptr, "The Particle contact MUST have at least one Particle associated.");
	}

	void ParticleContact::Resolve(TimeStep duration)
	{
		VXM_PROFILE_FUNCTION();
		ResolveVelocity(duration);
		ResolveInterpenetration(duration);
	}

	Real ParticleContact::CalculateSeparatingVelocity() const
	{
		VXM_PROFILE_FUNCTION();
		Vec3 relativeVelocity = particles[0]->GetVelocity();

		if(particles[1])
		{
			relativeVelocity -= particles[1]->GetVelocity();
		}

		return Math::Dot(relativeVelocity, contactNormal);
	}

	void ParticleContact::ResolveVelocity(TimeStep duration)
	{
		VXM_PROFILE_FUNCTION();
		// Find the velocity in the direction of the contact.
		auto separatingVelocity = CalculateSeparatingVelocity();

		if(separatingVelocity > 0) // Don't need to be resolved.
		{
			return;
		}

		Real newSepVelocity = -separatingVelocity * restitution;

		Vec3 accCausedVelocity = particles[0]->GetAcceleration();
		if(particles[1])
		{
			accCausedVelocity -= particles[1]->GetAcceleration();
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

		Real totalInverseMass = particles[0]->GetInverseMass();
		if(particles[1])
		{
			totalInverseMass += particles[1]->GetInverseMass();
		}

		// Infinite mass, therefore, no resolving.
		if(totalInverseMass <= 0)
		{
			return;
		}

		Real impulse = deltaVelocity / totalInverseMass;

		Vec3 impulsePerIMass = contactNormal * impulse;

		particles[0]->AddVelocity(impulsePerIMass * (+particles[0]->GetInverseMass()));
		if(particles[1])
		{
			// The other particle should go the other way around.
			particles[1]->AddVelocity(impulsePerIMass * (-particles[1]->GetInverseMass()));
		}
	}

	void ParticleContact::ResolveInterpenetration(TimeStep duration)
	{
		VXM_PROFILE_FUNCTION();

		if(penetration <= 0)
		{
			// No penetration, doing nothing.
			return;
		}

		Real totalInverseMass = particles[0]->GetInverseMass();
		if(particles[1])
		{
			totalInverseMass += particles[1]->GetInverseMass();
		}

		// Infinite mass, therefore, no resolving.
		if(totalInverseMass <= 0)
		{
			return;
		}

		Vec3 movePerIMass = contactNormal * (penetration * (penetration  / totalInverseMass));

		std::array<Vec3, 2> particleMovement = {Vec3(0), Vec3(0)};
		particleMovement[0] = movePerIMass * particles[0]->GetInverseMass();
		if(particles[1])
		{
			// Go the other way around.
			particleMovement[1] = movePerIMass * -particles[1]->GetInverseMass();
		}

		particles[0]->AddMovement(particleMovement[0]);
		if(particles[1])
		{
			particles[1]->AddMovement(particleMovement[1]);
		}
	}
} // namespace Voxymore::Core
