/*
* Copyright (c) 2011 Erin Catto http://box2d.org
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

#ifndef ROPE_H
#define ROPE_H

///
class Rope : public Test
{
public:
	Rope()
	{
		const int32 N = 40;
		b2Vec2 vertices[N];
		float32 masses[N];

		for (int32 i = 0; i < N; ++i)
		{
			vertices[i].Set(0_fx, 20_fx - "0.25"_fx * i);
			masses[i] = 1_fx;
		}
		masses[0] = 0_fx;
		masses[1] = 0_fx;

		b2RopeDef def;
		def.vertices = vertices;
		def.count = N;
		def.gravity.Set(0_fx, -10_fx);
		def.masses = masses;
		def.damping = "0.1"_fx;
		def.k2 = 1_fx;
		def.k3 = "0.5"_fx;

		m_rope.Initialize(&def);

		m_angle = 0_fx;
		m_rope.SetAngle(m_angle);
	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'q':
			m_angle = b2Max(-b2_pi, m_angle - "0.05"_fx * b2_pi);
			m_rope.SetAngle(m_angle);
			break;

		case 'e':
			m_angle = b2Min(b2_pi, m_angle + "0.05"_fx * b2_pi);
			m_rope.SetAngle(m_angle);
			break;
		}
	}

	void Step(Settings* settings)
	{
		float32 dt = settings->hz > 0_fx ? 1_fx / settings->hz : 0_fx;

		if (settings->pause == 1 && settings->singleStep == 0)
		{
			dt = 0_fx;
		}

		m_rope.Step(dt, 1);

		Test::Step(settings);

		m_rope.Draw(&m_debugDraw);

		m_debugDraw.DrawString(5, m_textLine, "Press (q,e) to adjust target angle");
		m_textLine += DRAW_STRING_NEW_LINE;
		m_debugDraw.DrawString(5, m_textLine, "Target angle = %g degrees", m_angle * 180_fx / b2_pi);
		m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
		return new Rope;
	}

	b2Rope m_rope;
	float32 m_angle;
};

#endif
