/*
* Copyright (c) 2011 Erin Catto http://www.box2d.org
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

#ifndef TUMBLER_H
#define TUMBLER_H

class Tumbler : public Test
{
public:

	enum
	{
		e_count = 800
	};

	Tumbler()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);
		}

		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.allowSleep = false;
			bd.position.Set(0_fx, 10_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, 10_fx, b2Vec2( 10_fx, 0_fx), 0_fx);
			body->CreateFixture(&shape, 5_fx);
			shape.SetAsBox("0.5"_fx, 10_fx, b2Vec2(-10_fx, 0_fx), 0_fx);
			body->CreateFixture(&shape, 5_fx);
			shape.SetAsBox(10_fx, "0.5"_fx, b2Vec2(0_fx, 10_fx), 0_fx);
			body->CreateFixture(&shape, 5_fx);
			shape.SetAsBox(10_fx, "0.5"_fx, b2Vec2(0_fx, -10_fx), 0_fx);
			body->CreateFixture(&shape, 5_fx);

			b2RevoluteJointDef jd;
			jd.bodyA = ground;
			jd.bodyB = body;
			jd.localAnchorA.Set(0_fx, 10_fx);
			jd.localAnchorB.Set(0_fx, 0_fx);
			jd.referenceAngle = 0_fx;
			jd.motorSpeed = "0.05"_fx * b2_pi;
            jd.maxMotorTorque = "0.00000001"_fx;// 1e8f;
			jd.enableMotor = true;
			m_joint = (b2RevoluteJoint*)m_world->CreateJoint(&jd);
		}

		m_count = 0;
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		if (m_count < e_count)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 10_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox("0.125"_fx, "0.125"_fx);
			body->CreateFixture(&shape, 1_fx);

			++m_count;
		}
	}

	static Test* Create()
	{
		return new Tumbler;
	}

	b2RevoluteJoint* m_joint;
	int32 m_count;
};

#endif
