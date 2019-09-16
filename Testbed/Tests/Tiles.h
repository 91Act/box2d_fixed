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

#ifndef TILES_H
#define TILES_H

/// This stress tests the dynamic tree broad-phase. This also shows that tile
/// based collision is _not_ smooth due to Box2D not knowing about adjacency.
class Tiles : public Test
{
public:
	enum
	{
		e_count = 20
	};

	Tiles()
	{
		m_fixtureCount = 0;
		b2Timer timer;

		{
			float32 a = "0.5"_fx;
			b2BodyDef bd;
			bd.position.y = -a;
			b2Body* ground = m_world->CreateBody(&bd);

#if 1
			int32 N = 200;
			int32 M = 10;
			b2Vec2 position;
			position.y = 0_fx;
			for (int32 j = 0; j < M; ++j)
			{
				position.x = -(float32)N * a;
				for (int32 i = 0; i < N; ++i)
				{
					b2PolygonShape shape;
					shape.SetAsBox(a, a, position, 0_fx);
					ground->CreateFixture(&shape, 0_fx);
					++m_fixtureCount;
					position.x += 2_fx * a;
				}
				position.y -= 2_fx * a;
			}
#else
			int32 N = 200;
			int32 M = 10;
			b2Vec2 position;
			position.x = -N * a;
			for (int32 i = 0; i < N; ++i)
			{
				position.y = 0_fx;
				for (int32 j = 0; j < M; ++j)
				{
					b2PolygonShape shape;
					shape.SetAsBox(a, a, position, 0_fx);
					ground->CreateFixture(&shape, 0_fx);
					position.y -= 2_fx * a;
				}
				position.x += 2_fx * a;
			}
#endif
		}

		{
			float32 a = "0.5"_fx;
			b2PolygonShape shape;
			shape.SetAsBox(a, a);

			b2Vec2 x(-7_fx, "0.75"_fx);
			b2Vec2 y;
			b2Vec2 deltaX("0.5625"_fx, "1.25"_fx);
			b2Vec2 deltaY("1.125"_fx, 0_fx);

			for (int32 i = 0; i < e_count; ++i)
			{
				y = x;

				for (int32 j = i; j < e_count; ++j)
				{
					b2BodyDef bd;
					bd.type = b2_dynamicBody;
					bd.position = y;

					//if (i == 0 && j == 0)
					//{
					//	bd.allowSleep = false;
					//}
					//else
					//{
					//	bd.allowSleep = true;
					//}

					b2Body* body = m_world->CreateBody(&bd);
					body->CreateFixture(&shape, 5_fx);
					++m_fixtureCount;
					y += deltaY;
				}

				x += deltaX;
			}
		}

		m_createTime = timer.GetMilliseconds();
	}

	void Step(Settings* settings)
	{
		const b2ContactManager& cm = m_world->GetContactManager();
		int32 height = cm.m_broadPhase.GetTreeHeight();
		int32 leafCount = cm.m_broadPhase.GetProxyCount();
		int32 minimumNodeCount = 2 * leafCount - 1;
		float32 minimumHeight = (((float32)(minimumNodeCount)).log() / (2_fx).log()).ceil();
		g_debugDraw.DrawString(5, m_textLine, "dynamic tree height = %d, min = %d", height, (int)(minimumHeight));
		m_textLine += DRAW_STRING_NEW_LINE;

		Test::Step(settings);

		g_debugDraw.DrawString(5, m_textLine, "create time = %6.2f ms, fixture count = %d",
			(float)m_createTime, m_fixtureCount);
		m_textLine += DRAW_STRING_NEW_LINE;

		//b2DynamicTree* tree = &m_world->m_contactManager.m_broadPhase.m_tree;

		//if (m_stepCount == 400)
		//{
		//	tree->RebuildBottomUp();
		//}
	}

	static Test* Create()
	{
		return new Tiles;
	}

	int32 m_fixtureCount;
	float32 m_createTime;
};

#endif
