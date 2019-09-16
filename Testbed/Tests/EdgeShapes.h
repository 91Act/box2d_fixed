/*
* Copyright (c) 2006-2010 Erin Catto http://www.box2d.org
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

#ifndef EDGE_SHAPES_H
#define EDGE_SHAPES_H

class EdgeShapesCallback : public b2RayCastCallback
{
public:
	EdgeShapesCallback()
	{
		m_fixture = NULL;
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
						  const b2Vec2& normal, float32 fraction) override
	{
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;

		return fraction;
	}

	b2Fixture* m_fixture;
	b2Vec2 m_point;
	b2Vec2 m_normal;
};

class EdgeShapes : public Test
{
public:

	enum
	{
		e_maxBodies = 256
	};

	EdgeShapes()
	{
		// Ground body
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			float32 x1 = -20_fx;
			float32 y1 = 2_fx * (x1 / 10_fx * b2_pi).cos();
			for (int32 i = 0; i < 80; ++i)
			{
				float32 x2 = x1 + "0.5"_fx;
				float32 y2 = 2_fx * (x2 / 10_fx * b2_pi).cos();

				b2EdgeShape shape;
				shape.Set(b2Vec2(x1, y1), b2Vec2(x2, y2));
				ground->CreateFixture(&shape, 0_fx);

				x1 = x2;
				y1 = y2;
			}
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

		m_angle = 0_fx;
	}

	void Create(int32 index)
	{
		if (m_bodies[m_bodyIndex] != NULL)
		{
			m_world->DestroyBody(m_bodies[m_bodyIndex]);
			m_bodies[m_bodyIndex] = NULL;
		}

		b2BodyDef bd;

		float32 x = RandomFloat(-10_fx, 10_fx);
		float32 y = RandomFloat(10_fx, 20_fx);
		bd.position.Set(x, y);
		bd.angle = RandomFloat(-b2_pi, b2_pi);
		bd.type = b2_dynamicBody;

		if (index == 4)
		{
			bd.angularDamping = "0.02"_fx;
		}

		m_bodies[m_bodyIndex] = m_world->CreateBody(&bd);

		if (index < 4)
		{
			b2FixtureDef fd;
			fd.shape = m_polygons + index;
			fd.friction = "0.3"_fx;
			fd.density = 20_fx;
			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}
		else
		{
			b2FixtureDef fd;
			fd.shape = &m_circle;
			fd.friction = "0.3"_fx;
			fd.density = 20_fx;
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

		case GLFW_KEY_D:
			DestroyBody();
			break;
		}
	}

	void Step(Settings* settings)
	{
		bool advanceRay = settings->pause == 0 || settings->singleStep;

		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Press 1-5 to drop stuff");
		m_textLine += DRAW_STRING_NEW_LINE;

		float32 L = 25_fx;
		b2Vec2 point1(0_fx, 10_fx);
		b2Vec2 d(L * m_angle.cos(), -L * b2Abs(m_angle.sin()));
		b2Vec2 point2 = point1 + d;

		EdgeShapesCallback callback;

		m_world->RayCast(&callback, point1, point2);

		if (callback.m_fixture)
		{
			g_debugDraw.DrawPoint(callback.m_point, 5_fx, b2Color("0.4"_fx, "0.9"_fx, "0.4"_fx));

			g_debugDraw.DrawSegment(point1, callback.m_point, b2Color("0.8"_fx, "0.8"_fx, "0.8"_fx));

			b2Vec2 head = callback.m_point + "0.5"_fx * callback.m_normal;
			g_debugDraw.DrawSegment(callback.m_point, head, b2Color("0.9"_fx, "0.9"_fx, "0.4"_fx));
		}
		else
		{
			g_debugDraw.DrawSegment(point1, point2, b2Color("0.8"_fx, "0.8"_fx, "0.8"_fx));
		}

		if (advanceRay)
		{
			m_angle += "0.25"_fx * b2_pi / 180_fx;
		}
	}

	static Test* Create()
	{
		return new EdgeShapes;
	}

	int32 m_bodyIndex;
	b2Body* m_bodies[e_maxBodies];
	b2PolygonShape m_polygons[4];
	b2CircleShape m_circle;

	float32 m_angle;
};

#endif
