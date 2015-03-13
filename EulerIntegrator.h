#pragma once

#include "ListItem.h"
#include "LinkedList.h"

#include "Particle.h"
#include "PhysVector3D.h"
#include "ParticleSystem.h"
#include "Integrator.h"

class EulerIntegrator : public Integrator
{
public:
	void Init( void *s )	{	this->s = s;	}
	void step( float t )
	{
  ParticleSystem *ps=((ParticleSystem *)this->s);
		ps->clearForces();
		ps->applyForces();
		for ( int i = 0; i < ps->particles->count; i++ )
		{
			Particle *p = (Particle *) ps->particles->Element( i );
			if ( p->isFree() )
			{
				p->velocity.Add( p->force.x/(p->mass*t), p->force.y/(p->mass*t), p->force.z/(p->mass*t) );
				p->position.Add( p->velocity.x/t, p->velocity.y/t, p->velocity.z/t );
			}
		}
	}
};