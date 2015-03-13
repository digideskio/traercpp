#pragma once

#include "ListItem.h"
#include "LinkedList.h"
#include "PhysVector3D.h"

class Particle : public ListItem
{
public:
 PhysVector3D position;
 PhysVector3D velocity;
 PhysVector3D force;
 float mass,age;
 bool dead,fixed;

 Particle(void) {
  mass=age=0.0f;
  dead=fixed=false;
 }

 ~Particle(void) {}
 
 float distanceTo( Particle p ) { return this->position.distanceTo( p.position ); }

 void makeFixed() {	  fixed = true;  velocity.clear();  }
 void makeFree()  {	  fixed = false;  }

 bool isFixed()   {	  return fixed;   }
 bool isFree()    {	  return !fixed;  } 
  
 void setMass( float m ) { mass = m; }
  
 void reset()
 {
	 age = 0;
	 dead = false;
	 position.clear();
	 velocity.clear();
	 force.clear();
	 mass = 1.0f;
 }
};

class Particles : public LinkedList {};