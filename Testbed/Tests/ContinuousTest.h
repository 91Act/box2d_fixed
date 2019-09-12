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

#ifndef CONTINUOUS_TEST_H
#define CONTINUOUS_TEST_H

class ContinuousTest : public Test
{
public:

	ContinuousTest()
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

#if 1
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 20_fx);
			//bd.angle = "0.1"_fx;

			b2PolygonShape shape;
			shape.SetAsBox(2_fx, "0.1"_fx);

			m_body = m_world->CreateBody(&bd);
			m_body->CreateFixture(&shape, 1_fx);

			m_angularVelocity = RandomFloat(-50_fx, 50_fx);
			//m_angularVelocity = "46.661274"_fx;
			m_body->SetLinearVelocity(b2Vec2(0_fx, -100_fx));
			m_body->SetAngularVelocity(m_angularVelocity);
		}
#else
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0_fx, 2_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2CircleShape shape;
			shape.m_p.SetZero();
			shape.m_radius = "0.5"_fx;
			body->CreateFixture(&shape, 1_fx);

			bd.bullet = true;
			bd.position.Set(0_fx, 10_fx);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&shape, 1_fx);
			body->SetLinearVelocity(b2Vec2(0_fx, -100_fx));
		}
#endif

		extern int32 b2_gjkCalls, b2_gjkIters, b2_gjkMaxIters;
		extern int32 b2_toiCalls, b2_toiIters;
		extern int32 b2_toiRootIters, b2_toiMaxRootIters;
		extern float32 b2_toiTime, b2_toiMaxTime;

		b2_gjkCalls = 0; b2_gjkIters = 0; b2_gjkMaxIters = 0;
		b2_toiCalls = 0; b2_toiIters = 0;
		b2_toiRootIters = 0; b2_toiMaxRootIters = 0;
		b2_toiTime = 0_fx; b2_toiMaxTime = 0_fx;
	}

	void Launch()
	{
		extern int32 b2_gjkCalls, b2_gjkIters, b2_gjkMaxIters;
		extern int32 b2_toiCalls, b2_toiIters;
		extern int32 b2_toiRootIters, b2_toiMaxRootIters;
		extern float32 b2_toiTime, b2_toiMaxTime;

		b2_gjkCalls = 0; b2_gjkIters = 0; b2_gjkMaxIters = 0;
		b2_toiCalls = 0; b2_toiIters = 0;
		b2_toiRootIters = 0; b2_toiMaxRootIters = 0;
		b2_toiTime = 0_fx; b2_toiMaxTime = 0_fx;

		m_body->SetTransform(b2Vec2(0_fx, 20_fx), 0_fx);
		m_angularVelocity = RandomFloat(-50_fx, 50_fx);
		m_body->SetLinearVelocity(b2Vec2(0_fx, -100_fx));
		m_body->SetAngularVelocity(m_angularVelocity);
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		extern int32 b2_gjkCalls, b2_gjkIters, b2_gjkMaxIters;

		if (b2_gjkCalls > 0)
		{
			g_debugDraw.DrawString(5, m_textLine, "gjk calls = %d, ave gjk iters = %3.1f, max gjk iters = %d",
				b2_gjkCalls, b2_gjkIters / (float)b2_gjkCalls, b2_gjkMaxIters);
			m_textLine += DRAW_STRING_NEW_LINE;
		}

		extern int32 b2_toiCalls, b2_toiIters;
		extern int32 b2_toiRootIters, b2_toiMaxRootIters;
		extern float32 b2_toiTime, b2_toiMaxTime;

		if (b2_toiCalls > 0)
		{
			g_debugDraw.DrawString(5, m_textLine, "toi calls = %d, ave [max] toi iters = %3.1f [%d]",
								b2_toiCalls, b2_toiIters / (float)b2_toiCalls, b2_toiMaxRootIters);
			m_textLine += DRAW_STRING_NEW_LINE;
			
			g_debugDraw.DrawString(5, m_textLine, "ave [max] toi root iters = %3.1f [%d]",
                b2_toiRootIters / (float)b2_toiCalls, b2_toiMaxRootIters);
			m_textLine += DRAW_STRING_NEW_LINE;

			g_debugDraw.DrawString(5, m_textLine, "ave [max] toi time = %.1f [%.1f] (microseconds)",
				(float)(1000_fx * b2_toiTime / float32::make(b2_toiCalls)), (float)(1000_fx * b2_toiMaxTime));
			m_textLine += DRAW_STRING_NEW_LINE;
		}

		if (m_stepCount % 60 == 0)
		{
			//Launch();
		}
	}

	static Test* Create()
	{
		return new ContinuousTest;
	}

	b2Body* m_body;
	float32 m_angularVelocity;
};

#endif
