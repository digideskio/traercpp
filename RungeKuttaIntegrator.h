#pragma once

#include "ListItem.h"

#include "PhysVector3D.h"
#include "Integrator.h"
#include "ParticleSystem.h"

class RungeKuttaIntegrator : public Integrator
{	
public:
	PhysVector3Ds *originalPositions;
	PhysVector3Ds *originalVelocities;
	PhysVector3Ds *k1Forces;
	PhysVector3Ds *k1Velocities;
	PhysVector3Ds *k2Forces;
	PhysVector3Ds *k2Velocities;
	PhysVector3Ds *k3Forces;
	PhysVector3Ds *k3Velocities;
	PhysVector3Ds *k4Forces;
	PhysVector3Ds *k4Velocities;

 RungeKuttaIntegrator() {
  originalPositions=null;
  originalVelocities=null;
  k1Forces=null;
  k1Velocities=null;
  k2Forces=null;
  k2Velocities=null;
  k3Forces=null;
  k3Velocities=null;
  k4Forces=null;
  k4Velocities=null;
  s=null;
 }

 ~RungeKuttaIntegrator() { 
		if ( originalPositions )  delete originalPositions;
  if ( originalVelocities ) delete originalVelocities;
  if ( k1Forces )           delete k1Forces;
  if ( k1Velocities )       delete k1Velocities;
  if ( k2Forces )           delete k2Forces;
  if ( k2Velocities )       delete k2Velocities;
  if ( k3Forces )           delete k3Forces;
  if ( k3Velocities )       delete k3Velocities;
  if ( k4Forces )           delete k4Forces;
  if ( k4Velocities )       delete k4Velocities;
 }
	
 void Init( void *s )	{
		this->s = s;
		if ( originalPositions )  delete originalPositions;		originalPositions  = new PhysVector3Ds;
  if ( originalVelocities ) delete originalVelocities; originalVelocities = new PhysVector3Ds;
  if ( k1Forces )           delete k1Forces;           k1Forces           = new PhysVector3Ds;
  if ( k1Velocities )       delete k1Velocities;       k1Velocities       = new PhysVector3Ds;
  if ( k2Forces )           delete k2Forces;           k2Forces           = new PhysVector3Ds;
  if ( k2Velocities )       delete k2Velocities;      	k2Velocities       = new PhysVector3Ds;
  if ( k3Forces )           delete k3Forces;          	k3Forces           = new PhysVector3Ds;
  if ( k3Velocities )       delete k3Velocities;       k3Velocities       = new PhysVector3Ds;
  if ( k4Forces )           delete k4Forces;           k4Forces           = new PhysVector3Ds;
  if ( k4Velocities )       delete k4Velocities;       k4Velocities       = new PhysVector3Ds;
	}
	
	void allocateParticles()
	{
		while ( ((ParticleSystem *)s)->particles->count > originalPositions->count )
		{
	 	originalPositions->Append(  (ListItem *) (new PhysVector3D) );
			originalVelocities->Append( (ListItem *) (new PhysVector3D) );
			k1Forces->Append(           (ListItem *) (new PhysVector3D) );
			k1Velocities->Append(       (ListItem *) (new PhysVector3D) );
			k2Forces->Append(           (ListItem *) (new PhysVector3D) );
			k2Velocities->Append(       (ListItem *) (new PhysVector3D) );
			k3Forces->Append(           (ListItem *) (new PhysVector3D) );
			k3Velocities->Append(       (ListItem *) (new PhysVector3D) );
			k4Forces->Append(           (ListItem *) (new PhysVector3D) );
			k4Velocities->Append(       (ListItem *) (new PhysVector3D) );
		}
	}
	
	void step( float deltaT )
	{	
  ParticleSystem *ps=((ParticleSystem *)this->s);
  Particle *p;
  int i;
		allocateParticles();

  i=0;
		for ( p=(Particle *) (ps->particles->first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )	{
  		// save original position and velocities
    PhysVector3D *v;
				v=(PhysVector3D *) originalPositions->Element( i );  v->Copy( p->position );
				v=(PhysVector3D *) originalVelocities->Element( i ); v->Copy( p->velocity );
			}
			p->force.clear();	// and clear the forces
   i++;
		}
		
		// get all the k1 values	
		ps->applyForces();
		
  i=0;
		for ( p=(Particle *) (ps->particles->first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )	{
  		// save the intermediate forces
    PhysVector3D *v;
				v=(PhysVector3D *) k1Forces->Element( i );     v->Copy( p->force );
				v=(PhysVector3D *) k1Velocities->Element( i ); v->Copy( p->velocity );
			}		
			p->force.clear();

	 	// get k2 values
   if ( p->isFree() )
			{
				PhysVector3D *originalPosition = (PhysVector3D *) originalPositions->Element( i );
				PhysVector3D *k1Velocity = (PhysVector3D *) k1Velocities->Element( i );
				PhysVector3D *originalVelocity,*k1Force;

				p->position.x = originalPosition->x + k1Velocity->x * 0.5f * deltaT;
				p->position.y = originalPosition->y + k1Velocity->y * 0.5f * deltaT;
				p->position.z = originalPosition->z + k1Velocity->z * 0.5f * deltaT;
				
				originalVelocity = (PhysVector3D *) originalVelocities->Element( i );
				k1Force = (PhysVector3D *) k1Forces->Element( i );
				
				p->velocity.x = originalVelocity->x + k1Force->x * 0.5f * deltaT / p->mass;
				p->velocity.y = originalVelocity->y + k1Force->y * 0.5f * deltaT / p->mass;
				p->velocity.z = originalVelocity->z + k1Force->z * 0.5f * deltaT / p->mass;
			}
   i++;
		}
		ps->applyForces();

  i=0;
		for ( p=(Particle *) (ps->particles->first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )
			{
  		// save the intermediate forces
    PhysVector3D *v;
    v=(PhysVector3D *) k2Forces->Element( i );     v->Copy( p->force );
				v=(PhysVector3D *) k2Velocities->Element( i ); v->Copy( p->velocity );
			}			
			p->force.clear();	// and clear the forces now that we are done with them

 		// get k3 values
			if ( p->isFree() )
			{
 			PhysVector3D *originalPosition = (PhysVector3D *) originalPositions->Element( i );
				PhysVector3D *k2Velocity = (PhysVector3D *) k2Velocities->Element( i );
				PhysVector3D *originalVelocity,*k2Force;
				
				p->position.x = originalPosition->x + k2Velocity->x * 0.5f * deltaT;
				p->position.y = originalPosition->y + k2Velocity->y * 0.5f * deltaT;
				p->position.z = originalPosition->z + k2Velocity->z * 0.5f * deltaT;
				
				originalVelocity = (PhysVector3D *) originalVelocities->Element( i );
				k2Force = (PhysVector3D *) k2Forces->Element( i );
				
				p->velocity.x = originalVelocity->x + k2Force->x * 0.5f * deltaT / p->mass;
				p->velocity.y = originalVelocity->y + k2Force->y * 0.5f * deltaT / p->mass;
				p->velocity.z = originalVelocity->z + k2Force->z * 0.5f * deltaT / p->mass;
			}
   i++;
		}
		ps->applyForces();

  i=0;
		for ( p=(Particle *) (ps->particles->first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )
			{
  		// save the intermediate forces
    PhysVector3D *v;
				v=(PhysVector3D *) k3Forces->Element( i );     v->Copy( p->force );
				v=(PhysVector3D *) k3Velocities->Element( i ); v->Copy( p->velocity );
			}			
			p->force.clear();	// and clear the forces now that we are done with them

 		// get k4 values
			if ( p->isFree() )
			{
				PhysVector3D *originalPosition = (PhysVector3D *) originalPositions->Element( i );
				PhysVector3D *k3Velocity = (PhysVector3D *) k3Velocities->Element( i );
				PhysVector3D *originalVelocity, *k3Force;

				p->position.x = originalPosition->x + k3Velocity->x * deltaT;
				p->position.y = originalPosition->y + k3Velocity->y * deltaT;
				p->position.z = originalPosition->z + k3Velocity->z * deltaT;
				
				originalVelocity = (PhysVector3D *) originalVelocities->Element( i );
				k3Force = (PhysVector3D *) k3Forces->Element( i );
				
				p->velocity.x = originalVelocity->x + k3Force->x * deltaT / p->mass;
				p->velocity.y = originalVelocity->y + k3Force->y * deltaT / p->mass;
				p->velocity.z = originalVelocity->z + k3Force->z * deltaT / p->mass;
			}
   i++;
		}
		ps->applyForces();

  i=0;
		for ( p=(Particle *) (ps->particles->first); p; p=(Particle *) (p->next) ) {
   p->age += deltaT;
			if ( p->isFree() )
			{
  		// save the intermediate forces
    PhysVector3D *v;
				v=(PhysVector3D *) k4Forces->Element( i );     v->Copy( p->force );
				v=(PhysVector3D *) k4Velocities->Element( i ); v->Copy( p->velocity );

  		// put them all together and what do you get?
    PhysVector3D *originalVelocity,*k1Force,*k2Force,*k3Force,*k4Force;

    // update position
				PhysVector3D *originalPosition = (PhysVector3D *) originalPositions->Element( i );
				PhysVector3D *k1Velocity = (PhysVector3D *) k1Velocities->Element( i );
				PhysVector3D *k2Velocity = (PhysVector3D *) k2Velocities->Element( i );
				PhysVector3D *k3Velocity = (PhysVector3D *) k3Velocities->Element( i );
				PhysVector3D *k4Velocity = (PhysVector3D *) k4Velocities->Element( i );
				
				p->position.x = originalPosition->x + deltaT / 6.0f * ( k1Velocity->x + 2.0f*k2Velocity->x + 2.0f*k3Velocity->x + k4Velocity->x );
				p->position.y = originalPosition->y + deltaT / 6.0f * ( k1Velocity->y + 2.0f*k2Velocity->y + 2.0f*k3Velocity->y + k4Velocity->y );
				p->position.z = originalPosition->z + deltaT / 6.0f * ( k1Velocity->z + 2.0f*k2Velocity->z + 2.0f*k3Velocity->z + k4Velocity->z );
				
				// update velocity
				originalVelocity = (PhysVector3D *) originalVelocities->Element( i );
				k1Force = (PhysVector3D *) k1Forces->Element( i );
				k2Force = (PhysVector3D *) k2Forces->Element( i );
				k3Force = (PhysVector3D *) k3Forces->Element( i );
				k4Force = (PhysVector3D *) k4Forces->Element( i );
				
				p->velocity.x = originalVelocity->x + deltaT / ( 6.0f * p->mass ) * ( k1Force->x + 2.0f*k2Force->x + 2.0f*k3Force->x + k4Force->x );
				p->velocity.y = originalVelocity->y + deltaT / ( 6.0f * p->mass ) * ( k1Force->y + 2.0f*k2Force->y + 2.0f*k3Force->y + k4Force->y );
				p->velocity.z = originalVelocity->z + deltaT / ( 6.0f * p->mass ) * ( k1Force->z + 2.0f*k2Force->z + 2.0f*k3Force->z + k4Force->z );
			}
   i++;
  }
	}
};