/*
* Copyright (c) 2006-2012 Erin Catto http://www.box2d.org
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

#ifndef AddPair_H
#define AddPair_H

class AddPair : public Test
{
public:

	AddPair()
	{
		m_world->SetGravity(b2Vec2(0_fx,0_fx));
		{
			b2CircleShape shape;
			shape.m_p.SetZero();
			shape.m_radius = "0.1"_fx;

			float32 minX = -6_fx;
			float32 maxX = 0_fx;
			float32 minY = 4_fx;
			float32 maxY = 6_fx;
			
			for (int32 i = 0; i < 400; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position = b2Vec2(RandomFloat(minX,maxX),RandomFloat(minY,maxY));
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, "0.01"_fx);
			}
		}
		
		{
			b2PolygonShape shape;
			shape.SetAsBox("1.5"_fx, "1.5"_fx);
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-40_fx,5_fx);
			bd.bullet = true;
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&shape, 1_fx);
			body->SetLinearVelocity(b2Vec2(150_fx, 0_fx));
		}
	}

	static Test* Create()
	{
		return new AddPair;
	}
};

#endif
