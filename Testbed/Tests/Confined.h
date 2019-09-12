/*
* Copyright (c) 2009 Erin Catto http://www.box2d.org
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

#ifndef CONFINED_H
#define CONFINED_H

class Confined : public Test
{
public:

	enum
	{
		e_columnCount = 0,
		e_rowCount = 0
	};

	Confined()
	{
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;

			// Floor
			shape.Set(b2Vec2(-10_fx, 0_fx), b2Vec2(10_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);

			// Left wall
			shape.Set(b2Vec2(-10_fx, 0_fx), b2Vec2(-10_fx, 20_fx));
			ground->CreateFixture(&shape, 0_fx);

			// Right wall
			shape.Set(b2Vec2(10_fx, 0_fx), b2Vec2(10_fx, 20_fx));
			ground->CreateFixture(&shape, 0_fx);

			// Roof
			shape.Set(b2Vec2(-10_fx, 20_fx), b2Vec2(10_fx, 20_fx));
			ground->CreateFixture(&shape, 0_fx);
		}

		float32 radius = "0.5"_fx;
		b2CircleShape shape;
		shape.m_p.SetZero();
		shape.m_radius = radius;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1_fx;
		fd.friction = "0.1"_fx;

		for (int32 j = 0; j < e_columnCount; ++j)
		{
			for (int i = 0; i < e_rowCount; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10_fx + ("2.1"_fx * (float32)j + 1_fx + "0.01"_fx * (float32)i) * radius, (2_fx * (float32)i + 1_fx) * radius);
				b2Body* body = m_world->CreateBody(&bd);

				body->CreateFixture(&fd);
			}
		}

		m_world->SetGravity(b2Vec2(0_fx, 0_fx));
	}

	void CreateCircle()
	{
		float32 radius = 2_fx;
		b2CircleShape shape;
		shape.m_p.SetZero();
		shape.m_radius = radius;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1_fx;
		fd.friction = 0_fx;

		b2Vec2 p(RandomFloat(), 3_fx + RandomFloat());
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = p;
		//bd.allowSleep = false;
		b2Body* body = m_world->CreateBody(&bd);

		body->CreateFixture(&fd);
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_C:
			CreateCircle();
			break;
		}
	}

	void Step(Settings* settings)
	{
		bool sleeping = true;
		for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
		{
			if (b->GetType() != b2_dynamicBody)
			{
				continue;
			}

			if (b->IsAwake())
			{
				sleeping = false;
			}
		}

		if (m_stepCount == 180)
		{
			m_stepCount += 0;
		}

		//if (sleeping)
		//{
		//	CreateCircle();
		//}

		Test::Step(settings);

		for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
		{
			if (b->GetType() != b2_dynamicBody)
			{
				continue;
			}

			b2Vec2 p = b->GetPosition();
			if (p.x <= -10_fx || 10_fx <= p.x || p.y <= 0_fx || 20_fx <= p.y)
			{
				p.x += 0_fx;
			}
		}

		g_debugDraw.DrawString(5, m_textLine, "Press 'c' to create a circle.");
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new Confined;
	}
};

#endif
