/*
* Copyright (c) 2007-2009 Erin Catto http://www.box2d.org
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

#ifndef PULLEYS_H
#define PULLEYS_H

class Pulleys : public Test
{
public:
	Pulleys()
	{
		float32 y = 16_fx;
		float32 L = 12_fx;
		float32 a = 1_fx;
		float32 b = 2_fx;

		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape edge;
			edge.Set(b2Vec2(-40_fx, 0_fx), b2Vec2(40_fx, 0_fx));
			//ground->CreateFixture(&shape, 0_fx);

			b2CircleShape circle;
			circle.m_radius = 2_fx;

			circle.m_p.Set(-10_fx, y + b + L);
			ground->CreateFixture(&circle, 0_fx);

			circle.m_p.Set(10_fx, y + b + L);
			ground->CreateFixture(&circle, 0_fx);
		}

		{

			b2PolygonShape shape;
			shape.SetAsBox(a, b);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			//bd.fixedRotation = true;
			bd.position.Set(-10_fx, y);
			b2Body* body1 = m_world->CreateBody(&bd);
			body1->CreateFixture(&shape, 5_fx);

			bd.position.Set(10_fx, y);
			b2Body* body2 = m_world->CreateBody(&bd);
			body2->CreateFixture(&shape, 5_fx);

			b2PulleyJointDef pulleyDef;
			b2Vec2 anchor1(-10_fx, y + b);
			b2Vec2 anchor2(10_fx, y + b);
			b2Vec2 groundAnchor1(-10_fx, y + b + L);
			b2Vec2 groundAnchor2(10_fx, y + b + L);
			pulleyDef.Initialize(body1, body2, groundAnchor1, groundAnchor2, anchor1, anchor2, "1.5"_fx);

			m_joint1 = (b2PulleyJoint*)m_world->CreateJoint(&pulleyDef);
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		float32 ratio = m_joint1->GetRatio();
		float32 L = m_joint1->GetCurrentLengthA() + ratio * m_joint1->GetCurrentLengthB();
		g_debugDraw.DrawString(5, m_textLine, "L1 + %4.2f * L2 = %4.2f", (float) ratio, (float) L);
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new Pulleys;
	}

	b2PulleyJoint* m_joint1;
};

#endif
