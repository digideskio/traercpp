#pragma once

#include "ListItem.h"
#include "LinkedList.h"

#include "Particle.h"
#include "PhysVector3D.h"
#include "Integrator.h"
#include "ParticleSystem.h"

class ModifiedEulerIntegrator : public Integrator 
{
public:
 ModifiedEulerIntegrator() {}
 ~ModifiedEulerIntegrator() {}

	void Init( void *s )	{ this->s = s; }
	void step( float t )
	{
  ParticleSystem *ps=(ParticleSystem *)(this->s);
  float halftt;
		ps->clearForces();
		ps->applyForces();
		halftt=0.5f*t*t;
		for ( int i=0; i < ps->particles->count; i++ )
		{
			Particle *p = (Particle *) ps->particles->Element( i );
			if ( p->isFree() )
			{
				float ax = p->force.x/p->mass;
				float ay = p->force.y/p->mass;
				float az = p->force.z/p->mass;
				
				p->position.Add( p->velocity.x/t, p->velocity.y/t, p->velocity.z/t );
				p->position.Add( ax*halftt, ay*halftt, az*halftt );
				p->velocity.Add( ax/t, ay/t, az/t );
			}
		}
	}
};