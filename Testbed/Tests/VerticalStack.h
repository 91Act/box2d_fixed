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

#ifndef VERTICAL_STACK_H
#define VERTICAL_STACK_H

extern bool g_blockSolve;

class VerticalStack : public Test
{
public:

	enum
	{
		e_columnCount = 1,
		e_rowCount = 15
		//e_columnCount = 1,
		//e_rowCount = 1
	};

	VerticalStack()
	{
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);

			shape.Set(b2Vec2(20_fx, 0_fx), b2Vec2(20_fx, 20_fx));
			ground->CreateFixture(&shape, 0_fx);
		}

		float32 xs[5] = {0_fx, -10_fx, -5_fx, 5_fx, 10_fx};

		for (int32 j = 0; j < e_columnCount; ++j)
		{
			b2PolygonShape shape;
			shape.SetAsBox("0.5"_fx, "0.5"_fx);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1_fx;
			fd.friction = "0.3"_fx;

			for (int i = 0; i < e_rowCount; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;

				int32 n = j * e_rowCount + i;
				b2Assert(n < e_rowCount * e_columnCount);
				m_indices[n] = n;
				bd.userData = m_indices + n;

				float32 x = 0_fx;
				//float32 x = RandomFloat(-"0.02"_fx, "0.02"_fx);
				//float32 x = i % 2 == 0 ? -"0.01"_fx : "0.01"_fx;
				bd.position.Set(xs[j] + x, "0.55"_fx + "1.1"_fx * (float32)i);
				b2Body* body = m_world->CreateBody(&bd);

				m_bodies[n] = body;

				body->CreateFixture(&fd);
			}
		}

		m_bullet = NULL;
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_COMMA:
			if (m_bullet != NULL)
			{
				m_world->DestroyBody(m_bullet);
				m_bullet = NULL;
			}

			{
				b2CircleShape shape;
				shape.m_radius = "0.25"_fx;

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.density = 20_fx;
				fd.restitution = "0.05"_fx;

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.bullet = true;
				bd.position.Set(-31_fx, 5_fx);

				m_bullet = m_world->CreateBody(&bd);
				m_bullet->CreateFixture(&fd);

				m_bullet->SetLinearVelocity(b2Vec2(400_fx, 0_fx));
			}
			break;
                
        case GLFW_KEY_B:
            g_blockSolve = !g_blockSolve;
            break;
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		g_debugDraw.DrawString(5, m_textLine, "Press: (,) to launch a bullet.");
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "Blocksolve = %d", g_blockSolve);
		//if (m_stepCount == 300)
		//{
		//	if (m_bullet != NULL)
		//	{
		//		m_world->DestroyBody(m_bullet);
		//		m_bullet = NULL;
		//	}

		//	{
		//		b2CircleShape shape;
		//		shape.m_radius = "0.25"_fx;

		//		b2FixtureDef fd;
		//		fd.shape = &shape;
		//		fd.density = 20_fx;
		//		fd.restitution = "0.05"_fx;

		//		b2BodyDef bd;
		//		bd.type = b2_dynamicBody;
		//		bd.bullet = true;
		//		bd.position.Set(-31_fx, 5_fx);

		//		m_bullet = m_world->CreateBody(&bd);
		//		m_bullet->CreateFixture(&fd);

		//		m_bullet->SetLinearVelocity(b2Vec2(400_fx, 0_fx));
		//	}
		//}
	}

	static Test* Create()
	{
		return new VerticalStack;
	}

	b2Body* m_bullet;
	b2Body* m_bodies[e_rowCount * e_columnCount];
	int32 m_indices[e_rowCount * e_columnCount];
};

#endif
