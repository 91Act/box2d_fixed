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

#ifndef POLYCOLLISION_H
#define POLYCOLLISION_H

class PolyCollision : public Test
{
public:
	PolyCollision()
	{
		{
			m_polygonA.SetAsBox("0.2"_fx, "0.4"_fx);
			m_transformA.Set(b2Vec2(0_fx, 0_fx), 0_fx);
		}

		{
			m_polygonB.SetAsBox("0.5"_fx, "0.5"_fx);
			m_positionB.Set("19.345284"_fx, "1.5632932"_fx);
			m_angleB = "1.9160721"_fx;
			m_transformB.Set(m_positionB, m_angleB);
		}
	}

	static Test* Create()
	{
		return new PolyCollision;
	}

	void Step(Settings* settings)
	{
		B2_NOT_USED(settings);

		b2Manifold manifold;
		b2CollidePolygons(&manifold, &m_polygonA, m_transformA, &m_polygonB, m_transformB);

		b2WorldManifold worldManifold;
		worldManifold.Initialize(&manifold, m_transformA, m_polygonA.m_radius, m_transformB, m_polygonB.m_radius);

		g_debugDraw.DrawString(5, m_textLine, "point count = %d", manifold.pointCount);
		m_textLine += DRAW_STRING_NEW_LINE;

		{
			b2Color color("0.9"_fx, "0.9"_fx, "0.9"_fx);
			b2Vec2 v[b2_maxPolygonVertices];
			for (int32 i = 0; i < m_polygonA.m_count; ++i)
			{
				v[i] = b2Mul(m_transformA, m_polygonA.m_vertices[i]);
			}
			g_debugDraw.DrawPolygon(v, m_polygonA.m_count, color);

			for (int32 i = 0; i < m_polygonB.m_count; ++i)
			{
				v[i] = b2Mul(m_transformB, m_polygonB.m_vertices[i]);
			}
			g_debugDraw.DrawPolygon(v, m_polygonB.m_count, color);
		}

		for (int32 i = 0; i < manifold.pointCount; ++i)
		{
			g_debugDraw.DrawPoint(worldManifold.points[i], 4_fx, b2Color("0.9"_fx, "0.3"_fx, "0.3"_fx));
		}
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_positionB.x -= "0.1"_fx;
			break;

		case GLFW_KEY_D:
			m_positionB.x += "0.1"_fx;
			break;

		case GLFW_KEY_S:
			m_positionB.y -= "0.1"_fx;
			break;

		case GLFW_KEY_W:
			m_positionB.y += "0.1"_fx;
			break;

		case GLFW_KEY_Q:
			m_angleB += "0.1"_fx * b2_pi;
			break;

		case GLFW_KEY_E:
			m_angleB -= "0.1"_fx * b2_pi;
			break;
		}

		m_transformB.Set(m_positionB, m_angleB);
	}

	b2PolygonShape m_polygonA;
	b2PolygonShape m_polygonB;

	b2Transform m_transformA;
	b2Transform m_transformB;

	b2Vec2 m_positionB;
	float32 m_angleB;
};

#endif
