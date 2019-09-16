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

#ifndef REVOLUTE_H
#define REVOLUTE_H

class Revolute : public Test
{
public:
	Revolute()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));

			b2FixtureDef fd;
			fd.shape = &shape;
			//fd.filter.categoryBits = 2;

			ground->CreateFixture(&fd);
		}

		{
			b2CircleShape shape;
			shape.m_radius = "0.5"_fx;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			b2RevoluteJointDef rjd;

			bd.position.Set(-10_fx, 20_fx);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&shape, 5_fx);

			float32 w = 100_fx;
			body->SetAngularVelocity(w);
			body->SetLinearVelocity(b2Vec2(-8_fx * w, 0_fx));

			rjd.Initialize(ground, body, b2Vec2(-10_fx, 12_fx));
			rjd.motorSpeed = 1_fx * b2_pi;
			rjd.maxMotorTorque = 10000_fx;
			rjd.enableMotor = false;
			rjd.lowerAngle = -"0.25"_fx * b2_pi;
			rjd.upperAngle = "0.5"_fx * b2_pi;
			rjd.enableLimit = true;
			rjd.collideConnected = true;

			m_joint = (b2RevoluteJoint*)m_world->CreateJoint(&rjd);
		}

		{
			b2CircleShape circle_shape;
			circle_shape.m_radius = 3_fx;

			b2BodyDef circle_bd;
			circle_bd.type = b2_dynamicBody;
			circle_bd.position.Set(5_fx, 30_fx);

			b2FixtureDef fd;
			fd.density = 5_fx;
			fd.filter.maskBits = 1;
			fd.shape = &circle_shape;

			m_ball = m_world->CreateBody(&circle_bd);
			m_ball->CreateFixture(&fd);

			b2PolygonShape polygon_shape;
			polygon_shape.SetAsBox(10_fx, "0.2"_fx, b2Vec2 (-10_fx, 0_fx), 0_fx);

			b2BodyDef polygon_bd;
			polygon_bd.position.Set(20_fx, 10_fx);
			polygon_bd.type = b2_dynamicBody;
			polygon_bd.bullet = true;
			b2Body* polygon_body = m_world->CreateBody(&polygon_bd);
			polygon_body->CreateFixture(&polygon_shape, 2_fx);

			b2RevoluteJointDef rjd;
			rjd.Initialize(ground, polygon_body, b2Vec2(20_fx, 10_fx));
			rjd.lowerAngle = -"0.25"_fx * b2_pi;
			rjd.upperAngle = 0_fx * b2_pi;
			rjd.enableLimit = true;
			m_world->CreateJoint(&rjd);
		}

		// Tests mass computation of a small object far from the origin
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			b2Body* body = m_world->CreateBody(&bodyDef);
		
			b2PolygonShape polyShape;		
			b2Vec2 verts[3];
			verts[0].Set( "17.63"_fx, "36.31"_fx );
			verts[1].Set( "17.52"_fx, "36.69"_fx );
			verts[2].Set( "17.19"_fx, "36.36"_fx );
			polyShape.Set(verts, 3);
		
			b2FixtureDef polyFixtureDef;
			polyFixtureDef.shape = &polyShape;
			polyFixtureDef.density = (float32)1;

			body->CreateFixture(&polyFixtureDef);	//assertion hits inside here
		}

	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_L:
			m_joint->EnableLimit(!m_joint->IsLimitEnabled());
			break;

		case GLFW_KEY_M:
			m_joint->EnableMotor(!m_joint->IsMotorEnabled());
			break;
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Keys: (l) limits, (m) motor");
		m_textLine += DRAW_STRING_NEW_LINE;

		//if (m_stepCount == 360)
		//{
		//	m_ball->SetTransform(b2Vec2(0_fx, "0.5"_fx), 0_fx);
		//}

		//float32 torque1 = m_joint1->GetMotorTorque();
		//g_debugDraw.DrawString(5, m_textLine, "Motor Torque = %4.0f_fx, %4.0f_fx : Motor Force = %4.0f_fx", (float) torque1, (float) torque2, (float) force3);
		//m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new Revolute;
	}

	b2Body* m_ball;
	b2RevoluteJoint* m_joint;
};

#endif
