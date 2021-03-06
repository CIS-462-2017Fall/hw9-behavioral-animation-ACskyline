#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include "aJitter.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	std::cout << (targetPos - actorPos).Length() << std::endl;

	Vdesired = BehaviorController::gMaxSpeed * (targetPos - actorPos).Normalize();

	//if (Vdesired.Length() < 200.0)
	//{
	//	Vdesired = 100*Vdesired.Normalize().Cross(axisY);
	//}

	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	Vdesired = BehaviorController::gMaxSpeed * (actorPos - targetPos).Normalize();

	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	Vdesired = BehaviorController::KArrival*(targetPos - actorPos);

	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired


	Vdesired = (actorPos - targetPos);

	double length = Vdesired.Length();

	Vdesired = Vdesired.Normalize() * BehaviorController::KDeparture / length;

	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TODO: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target

	vec3 targetPos = m_pTarget->getLocalTranslation();
	Varrival = BehaviorController::KArrival*(targetPos - m_actorPos);


	vec3 Vavoid(0, 0, 0);
	//TODO: add your code here to compute Vavoid 

	// Step 2. compute Lb
	//TODO: add your code here
	double lb = BehaviorController::TAvoid * Varrival.Length();


	// Step 3. find closest obstacle 
	//TODO: add your code here
	Obstacle* closest = nullptr;
	double dis = std::numeric_limits<double>().max();
	for (int i = 0; i < mObstacles->size(); i++)
	{
		if ((m_actorPos - mObstacles->at(i).m_Center.getLocalTranslation()).Length() < dis)
		{
			dis = (m_actorPos - mObstacles->at(i).m_Center.getLocalTranslation()).Length();
			closest = &mObstacles->at(i);
		}
	}

	if(closest!=nullptr)
		m_obstaclePos = closest->m_Center.getLocalTranslation();

	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
	bool collide = false;
	vec3 dworld(0, 0, 0);
	vec3 dlocal(0, 0, 0);

	if (closest != nullptr)
	{
		dworld = m_obstaclePos - m_actorPos;
		dlocal = mat3::Rotation3D(-axisY, actor->getOrientation()[1]) * dworld;
		//front
		if (abs(dlocal[2]) < lb + BehaviorController::gAgentRadius + closest->m_Radius)
		{
			//side
			if (abs(dlocal[0]) <= BehaviorController::gAgentRadius + closest->m_Radius)
			{
				collide = true;
			}
		}

	}

	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here

	if (collide)
	{
		Vavoid = (mat3::Rotation3D(axisY, actor->getOrientation()[1]) * vec3(-dlocal[0], 0, 0)).Normalize();

		double rorb = BehaviorController::gAgentRadius + closest->m_Radius;
		double length = BehaviorController::KAvoid * (rorb - abs(dlocal[0])) / (rorb);
		Vavoid = length * Vavoid;
	}

	Vdesired  = Varrival + Vavoid;


	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

	// Step. 1 find a random direction
	//TODO: add your code here
  
	vec3 randDir = AJitterVec(AJitter(-1, 1)).Normalize();

	// Step2. scale it with a noise factor
	//TODO: add your code here

	double scale = AJitterVal(AJitter(10,20));
	randDir = scale * randDir;

	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here

	Vdesired = randDir;


	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here

	Vdesired = BehaviorController::KWander*(Vdesired + m_Wander).Normalize();
	m_Wander = Vdesired;

	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TODO: add your code here

	vec3 Varrival(0, 0, 0);

	if(leader == actor)
		return BehaviorController::KArrival*(targetPos - actorPos);

	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here
	
	vec3 Valign = Varrival;
	int n = agentList.size();
	for (int i = 0; i < n; i++)
	{
		if (agentList[i].getBehaviorController() == actor)
			continue;

		vec3 yourPos = agentList[i].getBehaviorController()->getPosition();
		vec3 yourVel = agentList[i].getBehaviorController()->getVelocity();
		
		if((actorPos - yourPos).Length() < BehaviorController::gKNeighborhood)
			Valign += 1.0 / (n-1) * yourVel;

	}

	Valign *= BehaviorController::KAlignment;
	
	Vdesired = Valign;

	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	
	vec3 Vseperate(0, 0, 0);
	int n = agentList.size();
	for (int i = 0; i < n; i++)
	{
		if (agentList.at(i).getBehaviorController() == actor)
			continue;

		vec3 d(0, 0, 0);
		vec3 yourPos = agentList.at(i).getBehaviorController()->getPosition();
		d = actorPos - yourPos;
		//std::cout << d.Length() << "?" << BehaviorController::RNeighborhood << std::endl;
		if (d.Length() < BehaviorController::gKNeighborhood)
		{
			Vseperate += (1.0 / (n-1)) * (d / (d.Length()*d.Length()));
		}
	}

	Vseperate *= BehaviorController::KSeparation * 20;

	//std::cout << Vseperate << std::endl;

	Vdesired = Vseperate;

	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 

	vec3 Vcohesion(0, 0, 0);
	vec3 center(0, 0, 0);
	int n = agentList.size();
	for (int i = 0; i < n; i++)
	{
		if (agentList.at(i).getBehaviorController() == actor)
			continue;

		vec3 yourPos = agentList.at(i).getBehaviorController()->getPosition();
		vec3 d = actorPos - yourPos;
		if (d.Length() < BehaviorController::gKNeighborhood)
			center += (1.0 / (n-1)) * yourPos;
	}

	Vcohesion = BehaviorController::KCohesion * (center - actorPos);

	//std::cout << Vseperate << std::endl;

	Vdesired = Vcohesion;

	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;


	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// compute Vdesired = Vflocking
	// TODO: add your code here 

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
																	  
	vec3 targetPos = m_pTarget->getLocalTranslation();

	vec3 Varrival(0, 0, 0);

	if (leader == actor)
		Varrival = BehaviorController::KArrival*(targetPos - actorPos);

	vec3 Vcohesion(0, 0, 0);
	vec3 Vseperate(0, 0, 0);
	vec3 Valign(0, 0, 0);

	vec3 center(0, 0, 0);

	int n = agentList.size();
	for (int i = 0; i < n; i++)
	{
		vec3 yourPos = agentList.at(i).getBehaviorController()->getPosition();
		vec3 yourVel = agentList[i].getBehaviorController()->getVelocity();
		vec3 d = actorPos - yourPos;
		if (d.Length() < BehaviorController::gKNeighborhood)
		{


			if (agentList.at(i).getBehaviorController() != actor)
			{
				Vseperate += (1.0 / (n - 1)) * (d / (d.Length()*d.Length()));
				Valign += 1.0 / (n - 1) * yourVel;
				center += (1.0 / (n-1)) * yourPos;

			}

		}
	}

	Vcohesion = BehaviorController::KCohesion * (center - actorPos);
	Vseperate *= BehaviorController::KSeparation * 5;
	Valign *= BehaviorController::KAlignment * 2;

	if(leader == actor)
		Vdesired = 0.2 * Vcohesion + 0.3 * Vseperate + 0.5 * Varrival;
	else
		Vdesired = 0.2 * Vcohesion + 0.3 * Vseperate + 0.5 * Valign;

	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent

	vec3 targetPos(0, 0, 0);

	vec3 Varrival(0, 0, 0);

	if (leader == actor)
	{
		targetPos = m_pTarget->getLocalTranslation();
		Varrival = BehaviorController::KArrival*(targetPos - actorPos);
	}
	else
	{
		targetPos = Rmat*vec3(0, 0, -200) + leader->getPosition();
		Varrival = BehaviorController::KArrival*(targetPos - actorPos);
	}

	vec3 Vseperate(0, 0, 0);

	int n = agentList.size();
	for (int i = 0; i < n; i++)
	{
		vec3 yourPos = agentList.at(i).getBehaviorController()->getPosition();
		vec3 yourVel = agentList[i].getBehaviorController()->getVelocity();
		vec3 d = actorPos - yourPos;
		if (d.Length() < BehaviorController::gKNeighborhood)
		{


			if (agentList.at(i).getBehaviorController() != actor)
			{
				Vseperate += (1.0 / (n - 1)) * (d / (d.Length()*d.Length()));

			}

		}
	}

	Vseperate *= BehaviorController::KSeparation * 5;

	if (leader == actor)
		Vdesired = 0.5 * Vseperate + 0.5 * Varrival;
	else
		Vdesired = 0.5 * Vseperate + 0.5 * Varrival;




	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

