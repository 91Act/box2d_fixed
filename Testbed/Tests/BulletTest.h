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

#ifndef BULLET_TEST_H
#define BULLET_TEST_H

class BulletTest : public Test
{
public:

	BulletTest()
	{
		{
			b2BodyDef bd;
			bd.position.Set(0_fx, 0_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2EdgeShape edge;

			edge.Set(b2Vec2(-10_fx, 0_fx), b2Vec2(10_fx, 0_fx));
			body->CreateFixture(&edge, 0_fx);

			b2PolygonShape shape;
			shape.SetAsBox("0.2"_fx, 1_fx, b2Vec2("0.5"_fx, 1_fx), 0_fx);
			body->CreateFixture(&shape, 0_fx);
		}

		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 4_fx);

			b2PolygonShape box;
			box.SetAsBox(2_fx, "0.1"_fx);

			m_body = m_world->CreateBody(&bd);
			m_body->CreateFixture(&box, 1_fx);

			box.SetAsBox("0.25"_fx, "0.25"_fx);

			//m_x = RandomFloat(-1_fx, 1_fx);
			m_x = "0.20352793"_fx;
			bd.position.Set(m_x, 10_fx);
			bd.bullet = true;

			m_bullet = m_world->CreateBody(&bd);
			m_bullet->CreateFixture(&box, 100_fx);

			m_bullet->SetLinearVelocity(b2Vec2(0_fx, -50_fx));
		}
	}

	void Launch()
	{
		m_body->SetTransform(b2Vec2(0_fx, 4_fx), 0_fx);
		m_body->SetLinearVelocity(b2Vec2_zero);
		m_body->SetAngularVelocity(0_fx);

		m_x = RandomFloat(-1_fx, 1_fx);
		m_bullet->SetTransform(b2Vec2(m_x, 10_fx), 0_fx);
		m_bullet->SetLinearVelocity(b2Vec2(0_fx, -50_fx));
		m_bullet->SetAngularVelocity(0_fx);

		extern int32 b2_gjkCalls, b2_gjkIters, b2_gjkMaxIters;
		extern int32 b2_toiCalls, b2_toiIters, b2_toiMaxIters;
		extern int32 b2_toiRootIters, b2_toiMaxRootIters;

		b2_gjkCalls = 0;
		b2_gjkIters = 0;
		b2_gjkMaxIters = 0;

		b2_toiCalls = 0;
		b2_toiIters = 0;
		b2_toiMaxIters = 0;
		b2_toiRootIters = 0;
		b2_toiMaxRootIters = 0;
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		extern int32 b2_gjkCalls, b2_gjkIters, b2_gjkMaxIters;
		extern int32 b2_toiCalls, b2_toiIters;
		extern int32 b2_toiRootIters, b2_toiMaxRootIters;

		if (b2_gjkCalls > 0)
		{
			g_debugDraw.DrawString(5, m_textLine, "gjk calls = %d, ave gjk iters = %3.1f, max gjk iters = %d",
				b2_gjkCalls, b2_gjkIters / (float)b2_gjkCalls, b2_gjkMaxIters);
			m_textLine += DRAW_STRING_NEW_LINE;
		}

		if (b2_toiCalls > 0)
		{
			g_debugDraw.DrawString(5, m_textLine, "toi calls = %d, ave toi iters = %3.1f, max toi iters = %d",
				b2_toiCalls, b2_toiIters / (float)b2_toiCalls, b2_toiMaxRootIters);
			m_textLine += DRAW_STRING_NEW_LINE;

			g_debugDraw.DrawString(5, m_textLine, "ave toi root iters = %3.1f, max toi root iters = %d",
                b2_toiRootIters / (float)b2_toiCalls, b2_toiMaxRootIters);
			m_textLine += DRAW_STRING_NEW_LINE;
		}

		if (m_stepCount % 60 == 0)
		{
			Launch();
		}
	}

	static Test* Create()
	{
		return new BulletTest;
	}

	b2Body* m_body;
	b2Body* m_bullet;
	float32 m_x;
};

#endif
