#include "ParticleSystem.h"

#include "Particle.h"
#include "Force.h"
#include "Attraction.h"
#include "Spring.h"

#include "Integrator.h"

#include "RungeKuttaIntegrator.h"
#include "ModifiedEulerIntegrator.h"

 void ParticleSystem::setIntegrator( int i )
 {
  if ( this->integrator ) delete this->integrator;
  if ( i==RUNGE_KUTTA )         this->integrator = (Integrator *) (new RungeKuttaIntegrator);
  else if ( i==MODIFIED_EULER )	this->integrator = (Integrator *) (new ModifiedEulerIntegrator);
 }
  
  // default down gravity
 void ParticleSystem::setGravity( float g )                   { gravity.Set( 0.0f, g, 0.0f ); }
 void ParticleSystem::SetGravity( float x, float y, float z ) { gravity.Set( x, y, z ); }
 void ParticleSystem::setDrag( float d )                      { drag = d; }
  
 void ParticleSystem::step( float t ) { integrator->step( t ); }
 void ParticleSystem::tick()          {	step( 1 );             }
  
 Particle *ParticleSystem::makeParticle( float m, float x, float y, float z )
 {
  Particle *p= new Particle;
  p->mass = m;
  p->position.Set( x, y, z );
  particles->Append( p );
  return p;
 }
  
 Particle *ParticleSystem::makeDefaultParticle()  { return makeParticle( 1.0f, 0.0f, 0.0f, 0.0f ); }
  
 Spring *ParticleSystem::makeSpring( Particle a, Particle b, float ks, float d, float r )
 {
  Spring *s= new Spring;
  s->Init( a,b,ks,d,r );
  springs->Append( s );
  return s;
 }
  
 Attraction *ParticleSystem::makeAttraction( Particle a, Particle b, float k, float minDistance )
 {
  Attraction *m= new Attraction;
  m->Init( a, b, k, minDistance );
  attractions->Append( m );
  return m;
 }
  
 void ParticleSystem::clear()
 {
  delete particles; particles=new Particles;
  delete springs; springs=new Forces;
  delete attractions; attractions= new Forces;
  delete customForces; customForces= new Forces;
 }
  
 void ParticleSystem::InitG( float g, float somedrag )
 {
  delete integrator; 
  integrator = (Integrator *) (new RungeKuttaIntegrator);
  integrator->Init(this);
  clear();
  gravity.x=0.0f; gravity.y=g; gravity.z=0.0f;
  drag = somedrag;
 }
  
 void ParticleSystem::InitXYZ( float gx, float gy, float gz, float somedrag )
 {
  delete integrator;
  integrator = (Integrator *) (new RungeKuttaIntegrator);
  integrator->Init(this);
  clear();
  gravity.x=gx; gravity.y=gy; gravity.z=gz;
  drag = somedrag;
 }
  
 void ParticleSystem::Init()
 {
  delete integrator;
  integrator = (Integrator *) (new RungeKuttaIntegrator);
  integrator->Init(this);
  clear();
  gravity.x=0.0f;   gravity.y=DEFAULT_GRAVITY;   gravity.z=0.0f;
  drag = DEFAULT_DRAG;
 }
  
 void ParticleSystem::applyForces()
 {
  Particle *p;
  if ( !gravity.isZero() ) for( p=(Particle *) particles->first; p; p=(Particle *) (p->next) ) p->force.add( gravity );
  for( p=(Particle *) (particles->first);                 p; p=(Particle *) (p->next)   ) p->force.Add( p->velocity.x * -drag, p->velocity.y * -drag, p->velocity.z * -drag ); 
  for( Spring *s=(Spring *) (springs->first);             s; s=(Spring *) (s->next)     ) s->apply();
  for( Attraction *a=(Attraction *) (attractions->first); a; a=(Attraction *) (a->next) ) a->apply();
  for( Force *f=(Force *) (customForces->first);          f; f=(Force *) (f->next)      ) f->apply();
 }
  
 void ParticleSystem::clearForces() { 
  Particle *p;
  for ( p=(Particle *) (particles->first); p; p=(Particle *) (p->next) ) p->force.clear(); 
 }