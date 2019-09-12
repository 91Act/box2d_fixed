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

#ifndef CANTILEVER_H
#define CANTILEVER_H

// It is difficult to make a cantilever made of links completely rigid with weld joints.
// You will have to use a high number of iterations to make them stiff.
// So why not go ahead and use soft weld joints? They behave like a revolute
// joint with a rotational spring.
class Cantilever : public Test
{
public:

	enum
	{
		e_count = 8
	};

	Cantilever()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, "0.125"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20_fx;

			b2WeldJointDef jd;

			b2Body* prevBody = ground;
			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-"14.5"_fx + 1_fx * (float32)i, 5_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(-15_fx + 1_fx * (float32)i, 5_fx);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(1_fx, "0.125"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20_fx;

			b2WeldJointDef jd;
			jd.frequencyHz = 5_fx;
			jd.dampingRatio = "0.7"_fx;

			b2Body* prevBody = ground;
			for (int32 i = 0; i < 3; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-14_fx + 2_fx * (float32)i, 15_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(-15_fx + 2_fx * (float32)i, 15_fx);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, "0.125"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20_fx;

			b2WeldJointDef jd;

			b2Body* prevBody = ground;
			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-"4.5"_fx + 1_fx * (float32)i, 5_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				if (i > 0)
				{
					b2Vec2 anchor(-5_fx + 1_fx * (float32)i, 5_fx);
					jd.Initialize(prevBody, body, anchor);
					m_world->CreateJoint(&jd);
				}

				prevBody = body;
			}
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, "0.125"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20_fx;

			b2WeldJointDef jd;
			jd.frequencyHz = 8_fx;
			jd.dampingRatio = "0.7"_fx;

			b2Body* prevBody = ground;
			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set("5.5"_fx + 1_fx * (float32)i, 10_fx);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				if (i > 0)
				{
					b2Vec2 anchor(5_fx + 1_fx * (float32)i, 10_fx);
					jd.Initialize(prevBody, body, anchor);
					m_world->CreateJoint(&jd);
				}

				prevBody = body;
			}
		}

		for (int32 i = 0; i < 2; ++i)
		{
			b2Vec2 vertices[3];
			vertices[0].Set(-"0.5"_fx, 0_fx);
			vertices[1].Set("0.5"_fx, 0_fx);
			vertices[2].Set(0_fx, "1.5"_fx);

			b2PolygonShape shape;
			shape.Set(vertices, 3);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1_fx;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-8_fx + 8_fx * (float32)i, 12_fx);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);
		}

		for (int32 i = 0; i < 2; ++i)
		{
			b2CircleShape shape;
			shape.m_radius = "0.5"_fx;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1_fx;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-6_fx + 6_fx * (float32)i, 10_fx);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);
		}
	}

	static Test* Create()
	{
		return new Cantilever;
	}

	b2Body* m_middle;
};

#endif
