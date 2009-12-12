/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_REVOLUTE_JOINT_H
#define B2_REVOLUTE_JOINT_H

#include "b2Joint.h"

struct b2RevoluteJointDef : public b2JointDef
{
	b2RevoluteJointDef()
	{
		type = e_revoluteJoint;
		anchorPoint.Set(0.0f, 0.0f);
		lowerAngle = 0.0f;
		upperAngle = 0.0f;
		motorTorque = 0.0f;
		motorSpeed = 0.0f;
		enableLimit = false;
		enableMotor = false;
	}

	b2Vec2 anchorPoint;
	Fixed lowerAngle;
	Fixed upperAngle;
	Fixed motorTorque;
	Fixed motorSpeed;
	bool enableLimit;
	bool enableMotor;
};

class b2RevoluteJoint : public b2Joint
{
public:
	b2Vec2 GetAnchor1() const;
	b2Vec2 GetAnchor2() const;

	b2Vec2 GetReactionForce(Fixed invTimeStep) const;
	Fixed GetReactionTorque(Fixed invTimeStep) const;

	Fixed GetJointAngle() const;
	Fixed GetJointSpeed() const;
	Fixed GetMotorTorque(Fixed invTimeStep) const;

	void SetMotorSpeed(Fixed speed);
	void SetMotorTorque(Fixed torque);

	//--------------- Internals Below -------------------

	b2RevoluteJoint(const b2RevoluteJointDef* def);

	void PrepareVelocitySolver();
	void SolveVelocityConstraints(const b2TimeStep* step);

	bool SolvePositionConstraints();

	b2Vec2 m_localAnchor1;
	b2Vec2 m_localAnchor2;
	b2Vec2 m_ptpImpulse;
	Fixed m_motorImpulse;
	Fixed m_limitImpulse;
	Fixed m_limitPositionImpulse;

	b2Mat22 m_ptpMass;		// effective mass for point-to-point constraint.
	Fixed m_motorMass;	// effective mass for motor/limit angular constraint.
	Fixed m_intialAngle;
	Fixed m_lowerAngle;
	Fixed m_upperAngle;
	Fixed m_maxMotorTorque;
	Fixed m_motorSpeed;

	bool m_enableLimit;
	bool m_enableMotor;
	b2LimitState m_limitState;
};

#endif
