/*
* Copyright (c) 2006-2013 Erin Catto http://www.box2d.org
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

#include "DebugDraw.h"

#if defined(__APPLE_CC__)
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include "Testbed/glad/glad.h"
#endif

#include "Testbed/glfw/glfw3.h"
#include <stdio.h>
#include <stdarg.h>

#include "Testbed/imgui/imgui.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

DebugDraw g_debugDraw;
Camera g_camera;

struct b2Vec2_gl
{
    b2Vec2_gl() {}
    b2Vec2_gl(const b2Vec2 v)
        : x((float)v.x)
        , y((float)v.y)
    {}
    float x;
    float y;
};

struct b2Color_gl
{
    b2Color_gl() {}
    b2Color_gl(const b2Color v)
        : r((float)v.r)
        , g((float)v.g)
        , b((float)v.b)
        , a((float)v.a)
    {}
    float r;
    float g;
    float b;
    float a;
};
//
b2Vec2 Camera::ConvertScreenToWorld(const b2Vec2& ps)
{
    float32 w = float32::make(m_width);
    float32 h = float32::make(m_height);
	float32 u = ps.x / w;
	float32 v = (h - ps.y) / h;

	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25_fx, 25_fx);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	b2Vec2 pw;
	pw.x = (1_fx - u) * lower.x + u * upper.x;
	pw.y = (1_fx - v) * lower.y + v * upper.y;
	return pw;
}

//
b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2& pw)
{
	float32 w = float32::make(m_width);
	float32 h = float32::make(m_height);
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25_fx, 25_fx);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	float32 u = (pw.x - lower.x) / (upper.x - lower.x);
	float32 v = (pw.y - lower.y) / (upper.y - lower.y);

	b2Vec2 ps;
	ps.x = u * w;
	ps.y = (1_fx - v) * h;
	return ps;
}

// Convert from world coordinates to normalized device coordinates.
// http://www.songho.ca/opengl/gl_projectionmatrix.html
void Camera::BuildProjectionMatrix(float32* m, float32 zBias)
{
	float32 w = float32::make(m_width);
	float32 h = float32::make(m_height);
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25_fx, 25_fx);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	m[0] = 2_fx / (upper.x - lower.x);
	m[1] = 0_fx;
	m[2] = 0_fx;
	m[3] = 0_fx;

	m[4] = 0_fx;
	m[5] = 2_fx / (upper.y - lower.y);
	m[6] = 0_fx;
	m[7] = 0_fx;

	m[8] = 0_fx;
	m[9] = 0_fx;
	m[10] = 1_fx;
	m[11] = 0_fx;

	m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[14] = zBias;
	m[15] = 1_fx;
}

//
static void sCheckGLError()
{
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		fprintf(stderr, "OpenGL error = %d\n", errCode);
		assert(false);
	}
}

// Prints shader compilation errors
static void sPrintLog(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else
	{
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}


//
static GLuint sCreateShaderFromString(const char* source, GLenum type)
{
	GLuint res = glCreateShader(type);
	const char* sources[] = { source };
	glShaderSource(res, 1, sources, NULL);
	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE)
	{
		fprintf(stderr, "Error compiling shader of type %d!\n", type);
		sPrintLog(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}

// 
static GLuint sCreateShaderProgram(const char* vs, const char* fs)
{
	GLuint vsId = sCreateShaderFromString(vs, GL_VERTEX_SHADER);
	GLuint fsId = sCreateShaderFromString(fs, GL_FRAGMENT_SHADER);
	assert(vsId != 0 && fsId != 0);

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vsId);
	glAttachShader(programId, fsId);
    glBindFragDataLocation(programId, 0, "color");
	glLinkProgram(programId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);

	GLint status = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	assert(status != GL_FALSE);
	
	return programId;
}

//
struct GLRenderPoints
{
	void Create()
	{
		const char* vs = \
        "#version 330\n"
        "uniform mat4 projectionMatrix;\n"
        "layout(location = 0) in vec2 v_position;\n"
        "layout(location = 1) in vec4 v_color;\n"
		"layout(location = 2) in float v_size;\n"
        "out vec4 f_color;\n"
        "void main(void)\n"
        "{\n"
        "	f_color = v_color;\n"
        "	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
		"   gl_PointSize = v_size;\n"
        "}\n";
        
		const char* fs = \
        "#version 330\n"
        "in vec4 f_color;\n"
        "out vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "	color = f_color;\n"
        "}\n";
        
		m_programId = sCreateShaderProgram(vs, fs);
		m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
		m_vertexAttribute = 0;
		m_colorAttribute = 1;
		m_sizeAttribute = 2;
        
		// Generate
		glGenVertexArrays(1, &m_vaoId);
		glGenBuffers(3, m_vboIds);
        
		glBindVertexArray(m_vaoId);
		glEnableVertexAttribArray(m_vertexAttribute);
		glEnableVertexAttribArray(m_colorAttribute);
		glEnableVertexAttribArray(m_sizeAttribute);
        
		// Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
		glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices_gl), m_vertices_gl, GL_DYNAMIC_DRAW);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
		glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors_gl), m_colors_gl, GL_DYNAMIC_DRAW);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
		glVertexAttribPointer(m_sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_sizes_gl), m_sizes_gl, GL_DYNAMIC_DRAW);

		sCheckGLError();
        
		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
        
		m_count = 0;
	}
    
	void Destroy()
	{
		if (m_vaoId)
		{
			glDeleteVertexArrays(1, &m_vaoId);
			glDeleteBuffers(2, m_vboIds);
			m_vaoId = 0;
		}
        
		if (m_programId)
		{
			glDeleteProgram(m_programId);
			m_programId = 0;
		}
	}
    
	void Vertex(const b2Vec2& v, const b2Color& c, float32 size)
	{
		if (m_count == e_maxVertices)
			Flush();
        
		m_vertices[m_count] = v;
		m_colors[m_count] = c;
		m_sizes[m_count] = size;
		++m_count;
	}
    
    void Flush()
	{
        if (m_count == 0)
            return;
        
		glUseProgram(m_programId);
        
		float32 proj[16] = { 0_fx };
		g_camera.BuildProjectionMatrix(proj, 0_fx);
        
        float fproj[16] = { 0 };
        for (int i = 0; i < 16; i++) fproj[i] = (float)proj[i];

		glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, fproj);
        
		glBindVertexArray(m_vaoId);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        for (int i = 0; i < m_count; i++) m_vertices_gl[i] = m_vertices[i];
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2_gl), m_vertices_gl);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        for (int i = 0; i < m_count; i++) m_colors_gl[i] = m_colors[i];
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color_gl), m_colors_gl);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
        for (int i = 0; i < m_count; i++) m_sizes_gl[i] = (float)m_sizes[i];
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float), m_sizes_gl);

		glEnable(GL_PROGRAM_POINT_SIZE);
		glDrawArrays(GL_POINTS, 0, m_count);
        glDisable(GL_PROGRAM_POINT_SIZE);

		sCheckGLError();
        
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
        
		m_count = 0;
	}
    
	enum { e_maxVertices = 512 };
	b2Vec2 m_vertices[e_maxVertices];
	b2Color m_colors[e_maxVertices];
    float32 m_sizes[e_maxVertices];
    b2Vec2_gl m_vertices_gl[e_maxVertices];
    b2Color_gl m_colors_gl[e_maxVertices];
    float m_sizes_gl[e_maxVertices];

	int32 m_count;
    
	GLuint m_vaoId;
	GLuint m_vboIds[3];
	GLuint m_programId;
	GLint m_projectionUniform;
	GLint m_vertexAttribute;
	GLint m_colorAttribute;
	GLint m_sizeAttribute;
};

//
struct GLRenderLines
{
	void Create()
	{
		const char* vs = \
        "#version 330\n"
        "uniform mat4 projectionMatrix;\n"
        "layout(location = 0) in vec2 v_position;\n"
        "layout(location = 1) in vec4 v_color;\n"
        "out vec4 f_color;\n"
        "void main(void)\n"
        "{\n"
        "	f_color = v_color;\n"
        "	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
        "}\n";
        
		const char* fs = \
        "#version 330\n"
        "in vec4 f_color;\n"
        "out vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "	color = f_color;\n"
        "}\n";
        
		m_programId = sCreateShaderProgram(vs, fs);
		m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
		m_vertexAttribute = 0;
		m_colorAttribute = 1;
        
		// Generate
		glGenVertexArrays(1, &m_vaoId);
		glGenBuffers(2, m_vboIds);
        
		glBindVertexArray(m_vaoId);
		glEnableVertexAttribArray(m_vertexAttribute);
		glEnableVertexAttribArray(m_colorAttribute);
        
		// Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
		glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        for (int i = 0; i < e_maxVertices; i++) m_vertices_gl[i] = m_vertices[i];
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices_gl), m_vertices_gl, GL_DYNAMIC_DRAW);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
		glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        for (int i = 0; i < e_maxVertices; i++) m_colors_gl[i] = m_colors[i];
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors_gl), m_colors_gl, GL_DYNAMIC_DRAW);
        
		sCheckGLError();
        
		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
        
		m_count = 0;
	}
    
	void Destroy()
	{
		if (m_vaoId)
		{
			glDeleteVertexArrays(1, &m_vaoId);
			glDeleteBuffers(2, m_vboIds);
			m_vaoId = 0;
		}
        
		if (m_programId)
		{
			glDeleteProgram(m_programId);
			m_programId = 0;
		}
	}
    
	void Vertex(const b2Vec2& v, const b2Color& c)
	{
		if (m_count == e_maxVertices)
			Flush();
        
		m_vertices[m_count] = v;
		m_colors[m_count] = c;
		++m_count;
	}
    
    void Flush()
	{
        if (m_count == 0)
            return;
        
		glUseProgram(m_programId);
        
		float32 proj[16] = { 0_fx };
		g_camera.BuildProjectionMatrix(proj, "0.1"_fx);

        float fproj[16] = { 0 };
        for (int i = 0; i < 16; i++) fproj[i] = (float)proj[i];

		glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, fproj);
        
		glBindVertexArray(m_vaoId);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        for (int i = 0; i < m_count; i++) m_vertices_gl[i] = m_vertices[i];
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2_gl), m_vertices_gl);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        for (int i = 0; i < m_count; i++) m_colors_gl[i] = m_colors[i];
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color_gl), m_colors_gl);
        
		glDrawArrays(GL_LINES, 0, m_count);
        
		sCheckGLError();
        
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
        
		m_count = 0;
	}
    
	enum { e_maxVertices = 2 * 512 };
	b2Vec2 m_vertices[e_maxVertices];
    b2Color m_colors[e_maxVertices];
    b2Vec2_gl m_vertices_gl[e_maxVertices];
    b2Color_gl m_colors_gl[e_maxVertices];
    
	int32 m_count;
    
	GLuint m_vaoId;
	GLuint m_vboIds[2];
	GLuint m_programId;
	GLint m_projectionUniform;
	GLint m_vertexAttribute;
	GLint m_colorAttribute;
};

//
struct GLRenderTriangles
{
	void Create()
	{
		const char* vs = \
			"#version 330\n"
			"uniform mat4 projectionMatrix;\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_color;\n"
			"out vec4 f_color;\n"
			"void main(void)\n"
			"{\n"
			"	f_color = v_color;\n"
			"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
			"}\n";

		const char* fs = \
			"#version 330\n"
			"in vec4 f_color;\n"
            "out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"	color = f_color;\n"
			"}\n";

		m_programId = sCreateShaderProgram(vs, fs);
		m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
		m_vertexAttribute = 0;
		m_colorAttribute = 1;

		// Generate
		glGenVertexArrays(1, &m_vaoId);
		glGenBuffers(2, m_vboIds);

		glBindVertexArray(m_vaoId);
		glEnableVertexAttribArray(m_vertexAttribute);
		glEnableVertexAttribArray(m_colorAttribute);

		// Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
		glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices_gl), m_vertices_gl, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors_gl), m_colors_gl, GL_DYNAMIC_DRAW);

		sCheckGLError();

		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_count = 0;
	}

	void Destroy()
	{
		if (m_vaoId)
		{
			glDeleteVertexArrays(1, &m_vaoId);
			glDeleteBuffers(2, m_vboIds);
			m_vaoId = 0;
		}

		if (m_programId)
		{
			glDeleteProgram(m_programId);
			m_programId = 0;
		}
	}

	void Vertex(const b2Vec2& v, const b2Color& c)
	{
		if (m_count == e_maxVertices)
			Flush();

		m_vertices[m_count] = v;
		m_colors[m_count] = c;
		++m_count;
	}

    void Flush()
	{
        if (m_count == 0)
            return;
        
		glUseProgram(m_programId);
        
		float32 proj[16] = { 0_fx };
		g_camera.BuildProjectionMatrix(proj, "0.2"_fx);

        float fproj[16] = { 0 };
        for (int i = 0; i < 16; i++) fproj[i] = (float)proj[i];

		glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, fproj);
        
		glBindVertexArray(m_vaoId);
        
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        for (int i = 0; i < m_count; i++) m_vertices_gl[i] = m_vertices[i];
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2_gl), m_vertices_gl);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        for (int i = 0; i < m_count; i++) m_colors_gl[i] = m_colors[i];
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color_gl), m_colors_gl);
        
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, 0, m_count);
        glDisable(GL_BLEND);
        
		sCheckGLError();
        
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
        
		m_count = 0;
	}
    
	enum { e_maxVertices = 3 * 512 };
	b2Vec2 m_vertices[e_maxVertices];
    b2Color m_colors[e_maxVertices];
    b2Vec2_gl m_vertices_gl[e_maxVertices];
    b2Color_gl m_colors_gl[e_maxVertices];

	int32 m_count;

	GLuint m_vaoId;
	GLuint m_vboIds[2];
	GLuint m_programId;
	GLint m_projectionUniform;
	GLint m_vertexAttribute;
	GLint m_colorAttribute;
};

//
DebugDraw::DebugDraw()
{
	m_points = NULL;
    m_lines = NULL;
    m_triangles = NULL;
}

//
DebugDraw::~DebugDraw()
{
	b2Assert(m_points == NULL);
	b2Assert(m_lines == NULL);
	b2Assert(m_triangles == NULL);
}

//
void DebugDraw::Create()
{
	m_points = new GLRenderPoints;
	m_points->Create();
	m_lines = new GLRenderLines;
	m_lines->Create();
	m_triangles = new GLRenderTriangles;
	m_triangles->Create();
}

//
void DebugDraw::Destroy()
{
	m_points->Destroy();
	delete m_points;
	m_points = NULL;

	m_lines->Destroy();
	delete m_lines;
	m_lines = NULL;

	m_triangles->Destroy();
	delete m_triangles;
	m_triangles = NULL;
}

//
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
        b2Vec2 p2 = vertices[i];
		m_lines->Vertex(p1, color);
		m_lines->Vertex(p2, color);
        p1 = p2;
	}
}

//
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	b2Color fillColor("0.5"_fx * color.r, "0.5"_fx * color.g, "0.5"_fx * color.b, "0.5"_fx);

    for (int32 i = 1; i < vertexCount - 1; ++i)
    {
        m_triangles->Vertex(vertices[0], fillColor);
        m_triangles->Vertex(vertices[i], fillColor);
        m_triangles->Vertex(vertices[i+1], fillColor);
    }

    b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
        b2Vec2 p2 = vertices[i];
		m_lines->Vertex(p1, color);
		m_lines->Vertex(p2, color);
        p1 = p2;
	}
}

//
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16_fx;
	const float32 k_increment = 2_fx * b2_pi / k_segments;
    float32 sinInc = k_increment.sin();
    float32 cosInc = k_increment.cos();
    b2Vec2 r1(1_fx, 0_fx);
    b2Vec2 v1 = center + radius * r1;
	for (int32 i = 0; i < (float)k_segments; ++i)
	{
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
        m_lines->Vertex(v1, color);
        m_lines->Vertex(v2, color);
        r1 = r2;
        v1 = v2;
	}
}

//
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16_fx;
	const float32 k_increment = 2_fx * b2_pi / k_segments;
    float32 sinInc = k_increment.sin();
    float32 cosInc = k_increment.cos();
    b2Vec2 v0 = center;
    b2Vec2 r1(cosInc, sinInc);
    b2Vec2 v1 = center + radius * r1;
	b2Color fillColor("0.5"_fx * color.r, "0.5"_fx * color.g, "0.5"_fx * color.b, "0.5"_fx);
	for (int32 i = 0; i < (float)k_segments; ++i)
	{
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		m_triangles->Vertex(v0, fillColor);
        m_triangles->Vertex(v1, fillColor);
        m_triangles->Vertex(v2, fillColor);
        r1 = r2;
        v1 = v2;
	}

    r1.Set(1_fx, 0_fx);
    v1 = center + radius * r1;
	for (int32 i = 0; i < (float)k_segments; ++i)
	{
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
        m_lines->Vertex(v1, color);
        m_lines->Vertex(v2, color);
        r1 = r2;
        v1 = v2;
	}

    // Draw a line fixed in the circle to animate rotation.
	b2Vec2 p = center + radius * axis;
	m_lines->Vertex(center, color);
	m_lines->Vertex(p, color);
}

//
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_lines->Vertex(p1, color);
	m_lines->Vertex(p2, color);
}

//
void DebugDraw::DrawTransform(const b2Transform& xf)
{
	const float32 k_axisScale = "0.4"_fx;
    b2Color red(1_fx, 0_fx, 0_fx);
    b2Color green(0_fx, 1_fx, 0_fx);
	b2Vec2 p1 = xf.p, p2;

	m_lines->Vertex(p1, red);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	m_lines->Vertex(p2, red);

	m_lines->Vertex(p1, green);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	m_lines->Vertex(p2, green);
}

//
void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    m_points->Vertex(p, color, size);
}

//
void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(b2Vec2(float32::make(x), float32::make(y)));
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

//
void DebugDraw::DrawString(const b2Vec2& pw, const char *string, ...)
{
	b2Vec2 ps = g_camera.ConvertWorldToScreen(pw);

	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(ps);
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

//
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
    b2Vec2 p1 = aabb->lowerBound;
    b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
    b2Vec2 p3 = aabb->upperBound;
    b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);
    
    m_lines->Vertex(p1, c);
    m_lines->Vertex(p2, c);

    m_lines->Vertex(p2, c);
    m_lines->Vertex(p3, c);

    m_lines->Vertex(p3, c);
    m_lines->Vertex(p4, c);

    m_lines->Vertex(p4, c);
    m_lines->Vertex(p1, c);
}

//
void DebugDraw::Flush()
{
    m_triangles->Flush();
    m_lines->Flush();
    m_points->Flush();
}
