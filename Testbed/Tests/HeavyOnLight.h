/*
 * Copyright (c) 2008-2014 Erin Catto http://www.box2d.org
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

#ifndef HEAVY_ON_LIGHT_H
#define HEAVY_ON_LIGHT_H

class HeavyOnLight : public Test
{
public:
    
	HeavyOnLight()
	{
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);
            
			b2EdgeShape shape;
			shape.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			ground->CreateFixture(&shape, 0_fx);
		}
        
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(0_fx, "0.5"_fx);
		b2Body* body = m_world->CreateBody(&bd);
        
		b2CircleShape shape;
		shape.m_radius = "0.5"_fx;
        body->CreateFixture(&shape, 10_fx);
        
        bd.position.Set(0_fx, 6_fx);
        body = m_world->CreateBody(&bd);
        shape.m_radius = 5_fx;
        body->CreateFixture(&shape, 10_fx);
	}
    
	static Test* Create()
	{
		return new HeavyOnLight;
	}
};

#endif
