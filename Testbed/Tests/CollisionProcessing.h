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

#ifndef COLLISION_PROCESSING_H
#define COLLISION_PROCESSING_H

#include <algorithm>

// This test shows collision processing and tests
// deferred body destruction.
class CollisionProcessing : public Test
{
public:
	CollisionProcessing()
	{
		// Ground body
		{
			b2EdgeShape shape;
			shape.Set(b2Vec2(-50_fx, 0_fx), b2Vec2(50_fx, 0_fx));

			b2FixtureDef sd;
			sd.shape = &shape;;

			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&sd);
		}

		float32 xLo = -5_fx, xHi = 5_fx;
		float32 yLo = 2_fx, yHi = 35_fx;

		// Small triangle
		b2Vec2 vertices[3];
		vertices[0].Set(-1_fx, 0_fx);
		vertices[1].Set(1_fx, 0_fx);
		vertices[2].Set(0_fx, 2_fx);

		b2PolygonShape polygon;
		polygon.Set(vertices, 3);

		b2FixtureDef triangleShapeDef;
		triangleShapeDef.shape = &polygon;
		triangleShapeDef.density = 1_fx;

		b2BodyDef triangleBodyDef;
		triangleBodyDef.type = b2_dynamicBody;
		triangleBodyDef.position.Set(RandomFloat(xLo, xHi), RandomFloat(yLo, yHi));

		b2Body* body1 = m_world->CreateBody(&triangleBodyDef);
		body1->CreateFixture(&triangleShapeDef);

		// Large triangle (recycle definitions)
		vertices[0] *= 2_fx;
		vertices[1] *= 2_fx;
		vertices[2] *= 2_fx;
		polygon.Set(vertices, 3);

		triangleBodyDef.position.Set(RandomFloat(xLo, xHi), RandomFloat(yLo, yHi));

		b2Body* body2 = m_world->CreateBody(&triangleBodyDef);
		body2->CreateFixture(&triangleShapeDef);
		
		// Small box
		polygon.SetAsBox(1_fx, "0.5"_fx);

		b2FixtureDef boxShapeDef;
		boxShapeDef.shape = &polygon;
		boxShapeDef.density = 1_fx;

		b2BodyDef boxBodyDef;
		boxBodyDef.type = b2_dynamicBody;
		boxBodyDef.position.Set(RandomFloat(xLo, xHi), RandomFloat(yLo, yHi));

		b2Body* body3 = m_world->CreateBody(&boxBodyDef);
		body3->CreateFixture(&boxShapeDef);

		// Large box (recycle definitions)
		polygon.SetAsBox(2_fx, 1_fx);
		boxBodyDef.position.Set(RandomFloat(xLo, xHi), RandomFloat(yLo, yHi));
		
		b2Body* body4 = m_world->CreateBody(&boxBodyDef);
		body4->CreateFixture(&boxShapeDef);

		// Small circle
		b2CircleShape circle;
		circle.m_radius = 1_fx;

		b2FixtureDef circleShapeDef;
		circleShapeDef.shape = &circle;
		circleShapeDef.density = 1_fx;

		b2BodyDef circleBodyDef;
		circleBodyDef.type = b2_dynamicBody;
		circleBodyDef.position.Set(RandomFloat(xLo, xHi), RandomFloat(yLo, yHi));

		b2Body* body5 = m_world->CreateBody(&circleBodyDef);
		body5->CreateFixture(&circleShapeDef);

		// Large circle
		circle.m_radius *= 2_fx;
		circleBodyDef.position.Set(RandomFloat(xLo, xHi), RandomFloat(yLo, yHi));

		b2Body* body6 = m_world->CreateBody(&circleBodyDef);
		body6->CreateFixture(&circleShapeDef);
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		// We are going to destroy some bodies according to contact
		// points. We must buffer the bodies that should be destroyed
		// because they may belong to multiple contact points.
		const int32 k_maxNuke = 6;
		b2Body* nuke[k_maxNuke];
		int32 nukeCount = 0;

		// Traverse the contact results. Destroy bodies that
		// are touching heavier bodies.
		for (int32 i = 0; i < m_pointCount; ++i)
		{
			ContactPoint* point = m_points + i;

			b2Body* body1 = point->fixtureA->GetBody();
			b2Body* body2 = point->fixtureB->GetBody();
			float32 mass1 = body1->GetMass();
			float32 mass2 = body2->GetMass();

			if (mass1 > 0_fx && mass2 > 0_fx)
			{
				if (mass2 > mass1)
				{
					nuke[nukeCount++] = body1;
				}
				else
				{
					nuke[nukeCount++] = body2;
				}

				if (nukeCount == k_maxNuke)
				{
					break;
				}
			}
		}

		// Sort the nuke array to group duplicates.
		std::sort(nuke, nuke + nukeCount);

		// Destroy the bodies, skipping duplicates.
		int32 i = 0;
		while (i < nukeCount)
		{
			b2Body* b = nuke[i++];
			while (i < nukeCount && nuke[i] == b)
			{
				++i;
			}

			if (b != m_bomb)
			{
				m_world->DestroyBody(b);
			}
		}
	}

	static Test* Create()
	{
		return new CollisionProcessing;
	}
};

#endif
