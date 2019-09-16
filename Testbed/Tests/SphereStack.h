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

#ifndef SPHERE_STACK_H
#define SPHERE_STACK_H

class SphereStack : public Test
{
public:

	enum
	{
		e_count = 10
	};

	SphereStack()
	{
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);
		}

		{
			b2CircleShape shape;
			shape.m_radius = 1_fx;

			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0_fx, 4_fx + 3_fx * (float32)i);

				m_bodies[i] = m_world->CreateBody(&bd);

				m_bodies[i]->CreateFixture(&shape, 1_fx);

				m_bodies[i]->SetLinearVelocity(b2Vec2(0_fx, -50_fx));
			}
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		//for (int32 i = 0; i < e_count; ++i)
		//{
		//	printf("%g ", m_bodies[i]->GetWorldCenter().y);
		//}

		//for (int32 i = 0; i < e_count; ++i)
		//{
		//	printf("%g ", m_bodies[i]->GetLinearVelocity().y);
		//}

		//printf("\n");
	}

	static Test* Create()
	{
		return new SphereStack;
	}

	b2Body* m_bodies[e_count];
};

#endif
