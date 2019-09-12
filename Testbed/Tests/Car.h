/*
* Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
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

#ifndef CAR_H
#define CAR_H

// This is a fun demo that shows off the wheel joint
class Car : public Test
{
public:
	Car()
	{		
		m_hz = 4_fx;
		m_zeta = "0.7"_fx;
		m_speed = 50_fx;

		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 0_fx;
			fd.friction = "0.6"_fx;

			shape.Set(b2Vec2(-20_fx, 0_fx), b2Vec2(20_fx, 0_fx));
			ground->CreateFixture(&fd);

			float32 hs[10] = {"0.25"_fx, 1_fx, 4_fx, 0_fx, 0_fx, -1_fx, -2_fx, -2_fx, -"1.25"_fx, 0_fx};

			float32 x = 20_fx, y1 = 0_fx, dx = 5_fx;

			for (int32 i = 0; i < 10; ++i)
			{
				float32 y2 = hs[i];
				shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
				ground->CreateFixture(&fd);
				y1 = y2;
				x += dx;
			}

			for (int32 i = 0; i < 10; ++i)
			{
				float32 y2 = hs[i];
				shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
				ground->CreateFixture(&fd);
				y1 = y2;
				x += dx;
			}

			shape.Set(b2Vec2(x, 0_fx), b2Vec2(x + 40_fx, 0_fx));
			ground->CreateFixture(&fd);

			x += 80_fx;
			shape.Set(b2Vec2(x, 0_fx), b2Vec2(x + 40_fx, 0_fx));
			ground->CreateFixture(&fd);

			x += 40_fx;
			shape.Set(b2Vec2(x, 0_fx), b2Vec2(x + 10_fx, 5_fx));
			ground->CreateFixture(&fd);

			x += 20_fx;
			shape.Set(b2Vec2(x, 0_fx), b2Vec2(x + 40_fx, 0_fx));
			ground->CreateFixture(&fd);

			x += 40_fx;
			shape.Set(b2Vec2(x, 0_fx), b2Vec2(x, 20_fx));
			ground->CreateFixture(&fd);
		}

		// Teeter
		{
			b2BodyDef bd;
			bd.position.Set(140_fx, 1_fx);
			bd.type = b2_dynamicBody;
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape box;
			box.SetAsBox(10_fx, "0.25"_fx);
			body->CreateFixture(&box, 1_fx);

			b2RevoluteJointDef jd;
			jd.Initialize(ground, body, body->GetPosition());
			jd.lowerAngle = -8_fx * b2_pi / 180_fx;
			jd.upperAngle = 8_fx * b2_pi / 180_fx;
			jd.enableLimit = true;
			m_world->CreateJoint(&jd);

			body->ApplyAngularImpulse(100_fx, true);
		}

		// Bridge
		{
			int32 N = 20;
			b2PolygonShape shape;
			shape.SetAsBox(1_fx, "0.125"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1_fx;
			fd.friction = "0.6"_fx;

			b2RevoluteJointDef jd;

			b2Body* prevBody = ground;
			for (int32 i = 0; i < N; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(161_fx + 2_fx * (float32)i, -"0.125"_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(160_fx + 2_fx * (float32)i, -"0.125"_fx);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}

			b2Vec2 anchor(160_fx + 2_fx * (float32)N, -"0.125"_fx);
			jd.Initialize(prevBody, ground, anchor);
			m_world->CreateJoint(&jd);
		}

		// Boxes
		{
			b2PolygonShape box;
			box.SetAsBox("0.5"_fx, "0.5"_fx);

			b2Body* body = NULL;
			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			bd.position.Set(230_fx, "0.5"_fx);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, "0.5"_fx);

			bd.position.Set(230_fx, "1.5"_fx);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, "0.5"_fx);

			bd.position.Set(230_fx, "2.5"_fx);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, "0.5"_fx);

			bd.position.Set(230_fx, "3.5"_fx);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, "0.5"_fx);

			bd.position.Set(230_fx, "4.5"_fx);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, "0.5"_fx);
		}

		// Car
		{
			b2PolygonShape chassis;
			b2Vec2 vertices[8];
			vertices[0].Set(-"1.5"_fx, -"0.5"_fx);
			vertices[1].Set("1.5"_fx, -"0.5"_fx);
			vertices[2].Set("1.5"_fx, 0_fx);
			vertices[3].Set(0_fx, "0.9"_fx);
			vertices[4].Set(-"1.15"_fx, "0.9"_fx);
			vertices[5].Set(-"1.5"_fx, "0.2"_fx);
			chassis.Set(vertices, 6);

			b2CircleShape circle;
			circle.m_radius = "0.4"_fx;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 1_fx);
			m_car = m_world->CreateBody(&bd);
			m_car->CreateFixture(&chassis, 1_fx);

			b2FixtureDef fd;
			fd.shape = &circle;
			fd.density = 1_fx;
			fd.friction = "0.9"_fx;

			bd.position.Set(-1_fx, "0.35"_fx);
			m_wheel1 = m_world->CreateBody(&bd);
			m_wheel1->CreateFixture(&fd);

			bd.position.Set(1_fx, "0.4"_fx);
			m_wheel2 = m_world->CreateBody(&bd);
			m_wheel2->CreateFixture(&fd);

			b2WheelJointDef jd;
			b2Vec2 axis(0_fx, 1_fx);

			jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
			jd.motorSpeed = 0_fx;
			jd.maxMotorTorque = 20_fx;
			jd.enableMotor = true;
			jd.frequencyHz = m_hz;
			jd.dampingRatio = m_zeta;
			m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

			jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
			jd.motorSpeed = 0_fx;
			jd.maxMotorTorque = 10_fx;
			jd.enableMotor = false;
			jd.frequencyHz = m_hz;
			jd.dampingRatio = m_zeta;
			m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);
		}
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_spring1->SetMotorSpeed(m_speed);
			break;

		case GLFW_KEY_S:
			m_spring1->SetMotorSpeed(0_fx);
			break;

		case GLFW_KEY_D:
			m_spring1->SetMotorSpeed(-m_speed);
			break;

		case GLFW_KEY_Q:
			m_hz = b2Max(0_fx, m_hz - 1_fx);
			m_spring1->SetSpringFrequencyHz(m_hz);
			m_spring2->SetSpringFrequencyHz(m_hz);
			break;

		case GLFW_KEY_E:
			m_hz += 1_fx;
			m_spring1->SetSpringFrequencyHz(m_hz);
			m_spring2->SetSpringFrequencyHz(m_hz);
			break;
		}
	}

	void Step(Settings* settings)
	{
		g_debugDraw.DrawString(5, m_textLine, "Keys: left = a, brake = s, right = d, hz down = q, hz up = e");
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "frequency = %g hz, damping ratio = %g", (float)m_hz, (float)m_zeta);
		m_textLine += DRAW_STRING_NEW_LINE;

		g_camera.m_center.x = m_car->GetPosition().x;
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new Car;
	}

	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	float32 m_hz;
	float32 m_zeta;
	float32 m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;
};

#endif
