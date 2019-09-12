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

#ifndef TIME_OF_IMPACT_H
#define TIME_OF_IMPACT_H

class TimeOfImpact : public Test
{
public:
	TimeOfImpact()
	{
		m_shapeA.SetAsBox(25_fx, 5_fx);
		m_shapeB.SetAsBox("2.5"_fx, "2.5"_fx);
	}

	static Test* Create()
	{
		return new TimeOfImpact;
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		b2Sweep sweepA;
		sweepA.c0.Set(24_fx, -60_fx);
		sweepA.a0 = "2.95"_fx;
		sweepA.c = sweepA.c0;
		sweepA.a = sweepA.a0;
		sweepA.localCenter.SetZero();

		b2Sweep sweepB;
		sweepB.c0.Set("53.474274"_fx, -"50.252514"_fx);
		sweepB.a0 = "513.36676"_fx; // - 162_fx * b2_pi;
		sweepB.c.Set("54.595478"_fx, -"51.083473"_fx);
		sweepB.a = "513.62781"_fx; //  - 162_fx * b2_pi;
		sweepB.localCenter.SetZero();

		//sweepB.a0 -= 300_fx * b2_pi;
		//sweepB.a -= 300_fx * b2_pi;

		b2TOIInput input;
		input.proxyA.Set(&m_shapeA, 0);
		input.proxyB.Set(&m_shapeB, 0);
		input.sweepA = sweepA;
		input.sweepB = sweepB;
		input.tMax = 1_fx;

		b2TOIOutput output;

		b2TimeOfImpact(&output, &input);

		g_debugDraw.DrawString(5, m_textLine, "toi = %g", (float)output.t);
		m_textLine += DRAW_STRING_NEW_LINE;

		extern int32 b2_toiMaxIters, b2_toiMaxRootIters;
		g_debugDraw.DrawString(5, m_textLine, "max toi iters = %d, max root iters = %d", b2_toiMaxIters, b2_toiMaxRootIters);
		m_textLine += DRAW_STRING_NEW_LINE;

		b2Vec2 vertices[b2_maxPolygonVertices];

		b2Transform transformA;
		sweepA.GetTransform(&transformA, 0_fx);
		for (int32 i = 0; i < m_shapeA.m_count; ++i)
		{
			vertices[i] = b2Mul(transformA, m_shapeA.m_vertices[i]);
		}
		g_debugDraw.DrawPolygon(vertices, m_shapeA.m_count, b2Color("0.9"_fx, "0.9"_fx, "0.9"_fx));

		b2Transform transformB;
		sweepB.GetTransform(&transformB, 0_fx);
		
		//b2Vec2 localPoint(2_fx, -"0.1"_fx);

		for (int32 i = 0; i < m_shapeB.m_count; ++i)
		{
			vertices[i] = b2Mul(transformB, m_shapeB.m_vertices[i]);
		}
		g_debugDraw.DrawPolygon(vertices, m_shapeB.m_count, b2Color("0.5"_fx, "0.9"_fx, "0.5"_fx));

		sweepB.GetTransform(&transformB, output.t);
		for (int32 i = 0; i < m_shapeB.m_count; ++i)
		{
			vertices[i] = b2Mul(transformB, m_shapeB.m_vertices[i]);
		}
		g_debugDraw.DrawPolygon(vertices, m_shapeB.m_count, b2Color("0.5"_fx, "0.7"_fx, "0.9"_fx));

		sweepB.GetTransform(&transformB, 1_fx);
		for (int32 i = 0; i < m_shapeB.m_count; ++i)
		{
			vertices[i] = b2Mul(transformB, m_shapeB.m_vertices[i]);
		}
		g_debugDraw.DrawPolygon(vertices, m_shapeB.m_count, b2Color("0.9"_fx, "0.5"_fx, "0.5"_fx));

#if 0
		for (float32 t = 0_fx; t < 1_fx; t += "0.1"_fx)
		{
			sweepB.GetTransform(&transformB, t);
			for (int32 i = 0; i < m_shapeB.m_count; ++i)
			{
				vertices[i] = b2Mul(transformB, m_shapeB.m_vertices[i]);
			}
			g_debugDraw.DrawPolygon(vertices, m_shapeB.m_count, b2Color("0.9"_fx, "0.5"_fx, "0.5"_fx));
		}
#endif
	}

	b2PolygonShape m_shapeA;
	b2PolygonShape m_shapeB;
};

#endif
