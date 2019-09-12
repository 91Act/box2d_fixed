/*
* Copyright (c) 2006-2012 Erin Catto http://www.box2d.org
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

#ifndef MOTOR_JOINT_H
#define MOTOR_JOINT_H

/// This test shows how to use a motor joint. A motor joint
/// can be used to animate a dynamic body. With finite motor forces
/// the body can be blocked by collision with other bodies.
class MotorJoint : public Test
{
public:
	MotorJoint()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-20_fx, 0_fx), b2Vec2(20_fx, 0_fx));

			b2FixtureDef fd;
			fd.shape = &shape;

			ground->CreateFixture(&fd);
		}

		// Define motorized body
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 8_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(2_fx, "0.5"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.friction = "0.6"_fx;
			fd.density = 2_fx;
			body->CreateFixture(&fd);

			b2MotorJointDef mjd;
			mjd.Initialize(ground, body);
			mjd.maxForce = 1000_fx;
			mjd.maxTorque = 1000_fx;
			m_joint = (b2MotorJoint*)m_world->CreateJoint(&mjd);
		}

		m_go = false;
		m_time = 0_fx;
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_S:
			m_go = !m_go;
			break;
		}
	}

	void Step(Settings* settings)
	{
		if (m_go && settings->hz > 0_fx)
		{
			m_time += 1_fx / settings->hz;
		}

		b2Vec2 linearOffset;
		linearOffset.x = 6_fx * (2_fx * m_time).sin();
		linearOffset.y = 8_fx + 4_fx * (1_fx * m_time).sin();
		
		float32 angularOffset = 4_fx * m_time;

		m_joint->SetLinearOffset(linearOffset);
		m_joint->SetAngularOffset(angularOffset);

		g_debugDraw.DrawPoint(linearOffset, 4_fx, b2Color("0.9"_fx, "0.9"_fx, "0.9"_fx));

		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Keys: (s) pause");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new MotorJoint;
	}

	b2MotorJoint* m_joint;
	float32 m_time;
	bool m_go;
};

#endif
