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

#ifndef VARYING_RESTITUTION_H
#define VARYING_RESTITUTION_H

// Note: even with a restitution of 1.0, there is some energy change
// due to position correction.
class VaryingRestitution : public Test
{
public:

	VaryingRestitution()
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

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1_fx;

			float32 restitution[7] = {0_fx, "0.1"_fx, "0.3"_fx, "0.5"_fx, "0.75"_fx, "0.9"_fx, 1_fx};

			for (int32 i = 0; i < 7; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10_fx + 3_fx * (float32)i, 20_fx);

				b2Body* body = m_world->CreateBody(&bd);

				fd.restitution = restitution[i];
				body->CreateFixture(&fd);
			}
		}
	}

	static Test* Create()
	{
		return new VaryingRestitution;
	}
};

#endif
