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

#ifndef BODY_TYPES_H
#define BODY_TYPES_H

class BodyTypes : public Test
{
public:
	BodyTypes()
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

		// Define attachment
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 3_fx);
			m_attachment = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, 2_fx);
			m_attachment->CreateFixture(&shape, 2_fx);
		}

		// Define platform
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-4_fx, 5_fx);
			m_platform = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, 4_fx, b2Vec2(4_fx, 0_fx), "0.5"_fx * b2_pi);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.friction = "0.6"_fx;
			fd.density = 2_fx;
			m_platform->CreateFixture(&fd);

			b2RevoluteJointDef rjd;
			rjd.Initialize(m_attachment, m_platform, b2Vec2(0_fx, 5_fx));
			rjd.maxMotorTorque = 50_fx;
			rjd.enableMotor = true;
			m_world->CreateJoint(&rjd);

			b2PrismaticJointDef pjd;
			pjd.Initialize(ground, m_platform, b2Vec2(0_fx, 5_fx), b2Vec2(1_fx, 0_fx));

			pjd.maxMotorForce = 1000_fx;
			pjd.enableMotor = true;
			pjd.lowerTranslation = -10_fx;
			pjd.upperTranslation = 10_fx;
			pjd.enableLimit = true;

			m_world->CreateJoint(&pjd);

			m_speed = 3_fx;
		}

		// Create a payload
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 8_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox("0.75"_fx, "0.75"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.friction = "0.6"_fx;
			fd.density = 2_fx;

			body->CreateFixture(&fd);
		}
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_D:
			m_platform->SetType(b2_dynamicBody);
			break;

		case GLFW_KEY_S:
			m_platform->SetType(b2_staticBody);
			break;

		case GLFW_KEY_K:
			m_platform->SetType(b2_kinematicBody);
			m_platform->SetLinearVelocity(b2Vec2(-m_speed, 0_fx));
			m_platform->SetAngularVelocity(0_fx);
			break;
		}
	}

	void Step(Settings* settings)
	{
		// Drive the kinematic body.
		if (m_platform->GetType() == b2_kinematicBody)
		{
			b2Vec2 p = m_platform->GetTransform().p;
			b2Vec2 v = m_platform->GetLinearVelocity();

			if ((p.x < -10_fx && v.x < 0_fx) ||
				(p.x > 10_fx && v.x > 0_fx))
			{
				v.x = -v.x;
				m_platform->SetLinearVelocity(v);
			}
		}

		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Keys: (d) dynamic, (s) static, (k) kinematic");
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new BodyTypes;
	}

	b2Body* m_attachment;
	b2Body* m_platform;
	float32 m_speed;
};

#endif
