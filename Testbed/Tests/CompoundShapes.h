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

#ifndef COMPOUND_SHAPES_H
#define COMPOUND_SHAPES_H

// TODO_ERIN test joints on compounds.
class CompoundShapes : public Test
{
public:
	CompoundShapes()
	{
		{
			b2BodyDef bd;
			bd.position.Set(0_fx, 0_fx);
			b2Body* body = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(50_fx, 0_fx), b2Vec2(-50_fx, 0_fx));

			body->CreateFixture(&shape, 0_fx);
		}

		{
			b2CircleShape circle1;
			circle1.m_radius = "0.5"_fx;
			circle1.m_p.Set(-"0.5"_fx, "0.5"_fx);

			b2CircleShape circle2;
			circle2.m_radius = "0.5"_fx;
			circle2.m_p.Set("0.5"_fx, "0.5"_fx);

			for (int i = 0; i < 10; ++i)
			{
				float32 x = RandomFloat(-"0.1"_fx, "0.1"_fx);
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(x + 5_fx, "1.05"_fx + "2.5"_fx * (float32)i);
				bd.angle = RandomFloat(-b2_pi, b2_pi);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&circle1, 2_fx);
				body->CreateFixture(&circle2, 0_fx);
			}
		}

		{
			b2PolygonShape polygon1;
			polygon1.SetAsBox("0.25"_fx, "0.5"_fx);

			b2PolygonShape polygon2;
			polygon2.SetAsBox("0.25"_fx, "0.5"_fx, b2Vec2(0_fx, -"0.5"_fx), "0.5"_fx * b2_pi);

			for (int i = 0; i < 10; ++i)
			{
				float32 x = RandomFloat(-"0.1"_fx, "0.1"_fx);
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(x - 5_fx, "1.05"_fx + "2.5"_fx * (float32)i);
				bd.angle = RandomFloat(-b2_pi, b2_pi);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&polygon1, 2_fx);
				body->CreateFixture(&polygon2, 2_fx);
			}
		}

		{
			b2Transform xf1;
			xf1.q.Set("0.3524"_fx * b2_pi);
			xf1.p = xf1.q.GetXAxis();

			b2Vec2 vertices[3];

			b2PolygonShape triangle1;
			vertices[0] = b2Mul(xf1, b2Vec2(-1_fx, 0_fx));
			vertices[1] = b2Mul(xf1, b2Vec2(1_fx, 0_fx));
			vertices[2] = b2Mul(xf1, b2Vec2(0_fx, "0.5"_fx));
			triangle1.Set(vertices, 3);

			b2Transform xf2;
			xf2.q.Set(-"0.3524"_fx * b2_pi);
			xf2.p = -xf2.q.GetXAxis();

			b2PolygonShape triangle2;
			vertices[0] = b2Mul(xf2, b2Vec2(-1_fx, 0_fx));
			vertices[1] = b2Mul(xf2, b2Vec2(1_fx, 0_fx));
			vertices[2] = b2Mul(xf2, b2Vec2(0_fx, "0.5"_fx));
			triangle2.Set(vertices, 3);

			for (int32 i = 0; i < 10; ++i)
			{
				float32 x = RandomFloat(-"0.1"_fx, "0.1"_fx);
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(x, "2.05"_fx + "2.5"_fx * (float32)i);
				bd.angle = 0_fx;
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&triangle1, 2_fx);
				body->CreateFixture(&triangle2, 2_fx);
			}
		}

		{
			b2PolygonShape bottom;
			bottom.SetAsBox( "1.5"_fx, "0.15"_fx );

			b2PolygonShape left;
			left.SetAsBox("0.15"_fx, "2.7"_fx, b2Vec2(-"1.45"_fx, "2.35"_fx), "0.2"_fx);

			b2PolygonShape right;
			right.SetAsBox("0.15"_fx, "2.7"_fx, b2Vec2("1.45"_fx, "2.35"_fx), -"0.2"_fx);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set( 0_fx, 2_fx );
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&bottom, 4_fx);
			body->CreateFixture(&left, 4_fx);
			body->CreateFixture(&right, 4_fx);
		}
	}

	static Test* Create()
	{
		return new CompoundShapes;
	}
};

#endif
