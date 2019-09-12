/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
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

#ifndef SLIDER_CRANK_H
#define SLIDER_CRANK_H

// A motor driven slider crank with joint friction.

class SliderCrank : public Test
{
public:
	SliderCrank()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);
		}

		{
			b2Body* prevBody = ground;

			// Define crank.
			{
				b2PolygonShape shape;
				shape.SetAsBox("0.5"_fx, 2_fx);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0_fx, 7_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 2_fx);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0_fx, 5_fx));
				rjd.motorSpeed = 1_fx * b2_pi;
				rjd.maxMotorTorque = 10000_fx;
				rjd.enableMotor = true;
				m_joint1 = (b2RevoluteJoint*)m_world->CreateJoint(&rjd);

				prevBody = body;
			}

			// Define follower.
			{
				b2PolygonShape shape;
				shape.SetAsBox("0.5"_fx, 4_fx);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0_fx, 13_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 2_fx);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0_fx, 9_fx));
				rjd.enableMotor = false;
				m_world->CreateJoint(&rjd);

				prevBody = body;
			}

			// Define piston
			{
				b2PolygonShape shape;
				shape.SetAsBox("1.5"_fx, "1.5"_fx);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.fixedRotation = true;
				bd.position.Set(0_fx, 17_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 2_fx);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0_fx, 17_fx));
				m_world->CreateJoint(&rjd);

				b2PrismaticJointDef pjd;
				pjd.Initialize(ground, body, b2Vec2(0_fx, 17_fx), b2Vec2(0_fx, 1_fx));

				pjd.maxMotorForce = 1000_fx;
				pjd.enableMotor = true;

				m_joint2 = (b2PrismaticJoint*)m_world->CreateJoint(&pjd);
			}

			// Create a payload
			{
				b2PolygonShape shape;
				shape.SetAsBox("1.5"_fx, "1.5"_fx);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0_fx, 23_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 2_fx);
			}
		}
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_F:
			m_joint2->EnableMotor(!m_joint2->IsMotorEnabled());
			m_joint2->GetBodyB()->SetAwake(true);
			break;

		case GLFW_KEY_M:
			m_joint1->EnableMotor(!m_joint1->IsMotorEnabled());
			m_joint1->GetBodyB()->SetAwake(true);
			break;
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Keys: (f) toggle friction, (m) toggle motor");
		m_textLine += DRAW_STRING_NEW_LINE;
		float32 torque = m_joint1->GetMotorTorque(settings->hz);
		g_debugDraw.DrawString(5, m_textLine, "Motor Torque = %5.0f_fx", (float) torque);
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new SliderCrank;
	}

	b2RevoluteJoint* m_joint1;
	b2PrismaticJoint* m_joint2;
};

#endif
