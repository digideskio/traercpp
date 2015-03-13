#pragma once

#include "macros.h"

#include "Particle.h"
#include "Force.h"
#include "Attraction.h"
#include "Spring.h"

#include "Integrator.h"

class ParticleSystem
{
public:
 int RUNGE_KUTTA, MODIFIED_EULER;	
 float DEFAULT_GRAVITY, DEFAULT_DRAG;
 Particles *particles;
 Forces *springs;
 Forces *attractions;
 Forces *customForces;
 Integrator *integrator; 
 PhysVector3D gravity;
 float drag;
 bool hasDeadParticles;

 ParticleSystem() {
  RUNGE_KUTTA = 0;
  MODIFIED_EULER = 1;
  DEFAULT_GRAVITY = 0;
  DEFAULT_DRAG = 0.001f;	
  hasDeadParticles=false;
  particles = new Particles;
  springs = new Forces;
  attractions= new Forces;
  customForces = new Forces;
  integrator=null;
 }

 ~ParticleSystem() {
  delete particles;
  delete springs;
  delete attractions;
  delete customForces;
  if ( integrator ) delete integrator;
 }
 
 void setIntegrator( int i );

  // default down gravity
 void setGravity( float g );
 void SetGravity( float x, float y, float z );
 void setDrag( float d );
  
 void step( float t );
 void tick();
  
 Particle *makeParticle( float m, float x, float y, float z );
 Particle *makeDefaultParticle();
 Spring *makeSpring( Particle a, Particle b, float ks, float d, float r );
 Attraction *makeAttraction( Particle a, Particle b, float k, float minDistance );
 void clear();
 void InitG( float g, float somedrag );
 void InitXYZ( float gx, float gy, float gz, float somedrag );
  
 void Init();
 void applyForces();
 void clearForces();
};