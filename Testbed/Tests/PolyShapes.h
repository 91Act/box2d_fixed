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

#ifndef POLY_SHAPES_H
#define POLY_SHAPES_H

/// This tests stacking. It also shows how to use b2World::Query
/// and b2TestOverlap.

/// This callback is called by b2World::QueryAABB. We find all the fixtures
/// that overlap an AABB. Of those, we use b2TestOverlap to determine which fixtures
/// overlap a circle. Up to 4 overlapped fixtures will be highlighted with a yellow border.
class PolyShapesCallback : public b2QueryCallback
{
public:
	
	enum
	{
		e_maxCount = 4
	};

	PolyShapesCallback()
	{
		m_count = 0;
	}

	void DrawFixture(b2Fixture* fixture)
	{
		b2Color color("0.95"_fx, "0.95"_fx, "0.6"_fx);
		const b2Transform& xf = fixture->GetBody()->GetTransform();

		switch (fixture->GetType())
		{
		case b2Shape::e_circle:
			{
				b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

				b2Vec2 center = b2Mul(xf, circle->m_p);
				float32 radius = circle->m_radius;

				g_debugDraw->DrawCircle(center, radius, color);
			}
			break;

		case b2Shape::e_polygon:
			{
				b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
				int32 vertexCount = poly->m_count;
				b2Assert(vertexCount <= b2_maxPolygonVertices);
				b2Vec2 vertices[b2_maxPolygonVertices];

				for (int32 i = 0; i < vertexCount; ++i)
				{
					vertices[i] = b2Mul(xf, poly->m_vertices[i]);
				}

				g_debugDraw->DrawPolygon(vertices, vertexCount, color);
			}
			break;
				
		default:
			break;
		}
	}

	/// Called for each fixture found in the query AABB.
	/// @return false to terminate the query.
	bool ReportFixture(b2Fixture* fixture) override
	{
		if (m_count == e_maxCount)
		{
			return false;
		}

		b2Body* body = fixture->GetBody();
		b2Shape* shape = fixture->GetShape();

		bool overlap = b2TestOverlap(shape, 0, &m_circle, 0, body->GetTransform(), m_transform);

		if (overlap)
		{
			DrawFixture(fixture);
			++m_count;
		}

		return true;
	}

	b2CircleShape m_circle;
	b2Transform m_transform;
	b2Draw* g_debugDraw;
	int32 m_count;
};

class PolyShapes : public Test
{
public:

	enum
	{
		e_maxBodies = 256
	};

	PolyShapes()
	{
		// Ground body
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);
		}

		{
			b2Vec2 vertices[3];
			vertices[0].Set(-"0.5"_fx, 0_fx);
			vertices[1].Set("0.5"_fx, 0_fx);
			vertices[2].Set(0_fx, "1.5"_fx);
			m_polygons[0].Set(vertices, 3);
		}
		
		{
			b2Vec2 vertices[3];
			vertices[0].Set(-"0.1"_fx, 0_fx);
			vertices[1].Set("0.1"_fx, 0_fx);
			vertices[2].Set(0_fx, "1.5"_fx);
			m_polygons[1].Set(vertices, 3);
		}

		{
			float32 w = 1_fx;
			float32 b = w / (2_fx + b2Sqrt(2_fx));
			float32 s = b2Sqrt(2_fx) * b;

			b2Vec2 vertices[8];
			vertices[0].Set("0.5"_fx * s, 0_fx);
			vertices[1].Set("0.5"_fx * w, b);
			vertices[2].Set("0.5"_fx * w, b + s);
			vertices[3].Set("0.5"_fx * s, w);
			vertices[4].Set(-"0.5"_fx * s, w);
			vertices[5].Set(-"0.5"_fx * w, b + s);
			vertices[6].Set(-"0.5"_fx * w, b);
			vertices[7].Set(-"0.5"_fx * s, 0_fx);

			m_polygons[2].Set(vertices, 8);
		}

		{
			m_polygons[3].SetAsBox("0.5"_fx, "0.5"_fx);
		}

		{
			m_circle.m_radius = "0.5"_fx;
		}

		m_bodyIndex = 0;
		memset(m_bodies, 0, sizeof(m_bodies));
	}

	void Create(int32 index)
	{
		if (m_bodies[m_bodyIndex] != NULL)
		{
			m_world->DestroyBody(m_bodies[m_bodyIndex]);
			m_bodies[m_bodyIndex] = NULL;
		}

		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		float32 x = RandomFloat(-2_fx, 2_fx);
		bd.position.Set(x, 10_fx);
		bd.angle = RandomFloat(-b2_pi, b2_pi);

		if (index == 4)
		{
			bd.angularDamping = "0.02"_fx;
		}

		m_bodies[m_bodyIndex] = m_world->CreateBody(&bd);

		if (index < 4)
		{
			b2FixtureDef fd;
			fd.shape = m_polygons + index;
			fd.density = 1_fx;
			fd.friction = "0.3"_fx;
			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}
		else
		{
			b2FixtureDef fd;
			fd.shape = &m_circle;
			fd.density = 1_fx;
			fd.friction = "0.3"_fx;

			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}

		m_bodyIndex = (m_bodyIndex + 1) % e_maxBodies;
	}

	void DestroyBody()
	{
		for (int32 i = 0; i < e_maxBodies; ++i)
		{
			if (m_bodies[i] != NULL)
			{
				m_world->DestroyBody(m_bodies[i]);
				m_bodies[i] = NULL;
				return;
			}
		}
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
		case GLFW_KEY_5:
			Create(key - GLFW_KEY_1);
			break;

		case GLFW_KEY_A:
			for (int32 i = 0; i < e_maxBodies; i += 2)
			{
				if (m_bodies[i])
				{
					bool active = m_bodies[i]->IsActive();
					m_bodies[i]->SetActive(!active);
				}
			}
			break;

		case GLFW_KEY_D:
			DestroyBody();
			break;
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		PolyShapesCallback callback;
		callback.m_circle.m_radius = 2_fx;
		callback.m_circle.m_p.Set(0_fx, "1.1"_fx);
		callback.m_transform.SetIdentity();
		callback.g_debugDraw = &g_debugDraw;

		b2AABB aabb;
		callback.m_circle.ComputeAABB(&aabb, callback.m_transform, 0);

		m_world->QueryAABB(&callback, aabb);

		b2Color color("0.4"_fx, "0.7"_fx, "0.8"_fx);
		g_debugDraw.DrawCircle(callback.m_circle.m_p, callback.m_circle.m_radius, color);

		g_debugDraw.DrawString(5, m_textLine, "Press 1-5 to drop stuff");
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "Press 'a' to (de)activate some bodies");
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "Press 'd' to destroy a body");
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new PolyShapes;
	}

	int32 m_bodyIndex;
	b2Body* m_bodies[e_maxBodies];
	b2PolygonShape m_polygons[4];
	b2CircleShape m_circle;
};

#endif