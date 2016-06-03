#include <stdio.h>
#include <unistd.h>

#include <epoxy/gl.h>

#include "Context.h"

void DrawTriangle()
{
	const char* fs_test =
	"#version 300 es\n"
	"precision highp float;\n\n"

	"in vec4 col;\n"

	"out vec4 ocol;\n"
	"void main() {\n"
		"\tocol = col;\n"
	"}\n";

	const char* vs_test =
	"#version 300 es\n"

	"in vec2 pos;\n"
	"in vec4 in_col;\n"
	"out vec4 col;\n"

	"void main() {\n"
		"\tgl_Position = vec4(pos, 0.0, 1.0);\n"
		"\tcol = in_col;\n"
	"}\n";

	GLuint fs, vs, pgm;
	GLint stat, attr_pos, attr_col;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	vs = glCreateShader(GL_VERTEX_SHADER);
	pgm = glCreateProgram();

	glShaderSource(fs, 1, &fs_test, NULL);
	glShaderSource(vs, 1, &vs_test, NULL);

	glCompileShader(fs);
	glCompileShader(vs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &stat);
	if (!stat)
		printf("Couldn't compile fragment shader!\n");

	glGetShaderiv(vs, GL_COMPILE_STATUS, &stat);
	if (!stat)
		printf("Couldn't compile vertex shader!\n");

	glAttachShader(pgm, fs);
	glAttachShader(pgm, vs);
	glLinkProgram(pgm);

	glGetProgramiv(pgm, GL_LINK_STATUS, &stat);
	if (!stat)
		printf("Couldn't link program!\n");

	glUseProgram(pgm);

	// Get attribute locations
	attr_pos = glGetAttribLocation(pgm, "pos");
	attr_col = glGetAttribLocation(pgm, "in_col");

	glEnableVertexAttribArray(attr_pos);
	glEnableVertexAttribArray(attr_col);

	glClearColor(0.4, 0.4, 0.4, 0.0);

	const GLfloat verts[] = {
		-1, -1,
		1, -1,
		0,  1,

		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1
	};

	GLuint pos_vbo;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	for (;;)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(attr_col, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*6));

		glDrawArrays(GL_TRIANGLES, 0, 3);
		Context::Swap();
	}
}

int main()
{
	Context::Create();

	printf("Are we in desktop GL? %s\n", epoxy_is_desktop_gl() ? "Yes" : "No");
	printf("Our GL version %d\n", epoxy_gl_version());

	printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
	printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
	printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
	printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));

	DrawTriangle();

	Context::Shutdown();
}
