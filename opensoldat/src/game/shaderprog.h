/*
 Copyright (c) 2013, Stanislav Podolsky
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */
#ifndef SHADERPROG_H_
#define SHADERPROG_H_
#include "global.h"
class ShaderProg {
public:
	GLuint vertex_shader, fragment_shader, prog;

	template<int N>
	GLuint compile(GLuint type, char const *(&source)[N]) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, N, source, NULL);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string log(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &log[0]);
//			throw std::logic_error(log);
			return false;
		}
		return shader;
	}
public:
	template<int N, int M>
	ShaderProg(GLchar const *(&v_source)[N], GLchar const *(&f_source)[M]) {
		vertex_shader = compile(GL_VERTEX_SHADER, v_source);
		fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);
		prog = glCreateProgram();
		glAttachShader(prog, vertex_shader);
		glAttachShader(prog, fragment_shader);
		glLinkProgram(prog);
	}

	operator GLuint() {
		return prog;
	}
	void operator()() {
		glUseProgram(prog);
	}

	~ShaderProg() {
		glDeleteProgram(prog);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}
};

#endif /* SHADERPROG_H_ */
