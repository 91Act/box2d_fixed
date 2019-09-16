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

#ifndef DOMINOS_H
#define DOMINOS_H

class Dominos : public Test
{
public:

	Dominos()
	{
		b2Body* b1;
		{
			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));

			b2BodyDef bd;
			b1 = m_world->CreateBody(&bd);
			b1->CreateFixture(&shape, 0_fx);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(6_fx, "0.25"_fx);

			b2BodyDef bd;
			bd.position.Set(-"1.5"_fx, 10_fx);
			b2Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0_fx);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox("0.1"_fx, 1_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20_fx;
			fd.friction = "0.1"_fx;

			for (int i = 0; i < 10; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-6_fx + 1_fx * (float32)i, "11.25"_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);
			}
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(7_fx, "0.25"_fx, b2Vec2_zero, "0.3"_fx);

			b2BodyDef bd;
			bd.position.Set(1_fx, 6_fx);
			b2Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0_fx);
		}

		b2Body* b2;
		{
			b2PolygonShape shape;
			shape.SetAsBox("0.25"_fx, "1.5"_fx);

			b2BodyDef bd;
			bd.position.Set(-7_fx, 4_fx);
			b2 = m_world->CreateBody(&bd);
			b2->CreateFixture(&shape, 0_fx);
		}

		b2Body* b3;
		{
			b2PolygonShape shape;
			shape.SetAsBox(6_fx, "0.125"_fx);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-"0.9"_fx, 1_fx);
			bd.angle = -"0.15"_fx;

			b3 = m_world->CreateBody(&bd);
			b3->CreateFixture(&shape, 10_fx);
		}

		b2RevoluteJointDef jd;
		b2Vec2 anchor;

		anchor.Set(-2_fx, 1_fx);
		jd.Initialize(b1, b3, anchor);
		jd.collideConnected = true;
		m_world->CreateJoint(&jd);

		b2Body* b4;
		{
			b2PolygonShape shape;
			shape.SetAsBox("0.25"_fx, "0.25"_fx);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-10_fx, 15_fx);
			b4 = m_world->CreateBody(&bd);
			b4->CreateFixture(&shape, 10_fx);
		}

		anchor.Set(-7_fx, 15_fx);
		jd.Initialize(b2, b4, anchor);
		m_world->CreateJoint(&jd);

		b2Body* b5;
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set("6.5"_fx, 3_fx);
			b5 = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			b2FixtureDef fd;

			fd.shape = &shape;
			fd.density = 10_fx;
			fd.friction = "0.1"_fx;

			shape.SetAsBox(1_fx, "0.1"_fx, b2Vec2(0_fx, -"0.9"_fx), 0_fx);
			b5->CreateFixture(&fd);

			shape.SetAsBox("0.1"_fx, 1_fx, b2Vec2(-"0.9"_fx, 0_fx), 0_fx);
			b5->CreateFixture(&fd);

			shape.SetAsBox("0.1"_fx, 1_fx, b2Vec2("0.9"_fx, 0_fx), 0_fx);
			b5->CreateFixture(&fd);
		}

		anchor.Set(6_fx, 2_fx);
		jd.Initialize(b1, b5, anchor);
		m_world->CreateJoint(&jd);

		b2Body* b6;
		{
			b2PolygonShape shape;
			shape.SetAsBox(1_fx, "0.1"_fx);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set("6.5"_fx, "4.1"_fx);
			b6 = m_world->CreateBody(&bd);
			b6->CreateFixture(&shape, 30_fx);
		}

		anchor.Set("7.5"_fx, 4_fx);
		jd.Initialize(b5, b6, anchor);
		m_world->CreateJoint(&jd);

		b2Body* b7;
		{
			b2PolygonShape shape;
			shape.SetAsBox("0.1"_fx, 1_fx);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set("7.4"_fx, 1_fx);

			b7 = m_world->CreateBody(&bd);
			b7->CreateFixture(&shape, 10_fx);
		}

		b2DistanceJointDef djd;
		djd.bodyA = b3;
		djd.bodyB = b7;
		djd.localAnchorA.Set(6_fx, 0_fx);
		djd.localAnchorB.Set(0_fx, -1_fx);
		b2Vec2 d = djd.bodyB->GetWorldPoint(djd.localAnchorB) - djd.bodyA->GetWorldPoint(djd.localAnchorA);
		djd.length = d.Length();
		m_world->CreateJoint(&djd);

		{
			float32 radius = "0.2"_fx;

			b2CircleShape shape;
			shape.m_radius = radius;

			for (int32 i = 0; i < 4; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set("5.9"_fx + 2_fx * radius * (float32)i, "2.4"_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 10_fx);
			}
		}
	}

	static Test* Create()
	{
		return new Dominos;
	}
};

#endif
