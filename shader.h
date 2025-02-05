#pragma once
#ifndef SHADER_H
#define SHADER_H


#include<glad/glad.h>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>


class shader {
public:
	unsigned int ID;

	shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//open file by using filepath
			fShaderFile.open(fragmentPath);
			vShaderFile.open(vertexPath);
			std::stringstream fShaderStream, vShaderStream;
			//read the files
			fShaderStream << fShaderFile.rdbuf();
			vShaderStream << vShaderFile.rdbuf();
			//close the files
			fShaderFile.close();
			vShaderFile.close();
			//convert the string to stream
			fragmentCode = fShaderStream.str();
			vertexCode = vShaderStream.str();
			//end of the try
		}
		catch (std::ifstream::failure e) {
			std::cerr << "ERROR::SHADER::FILE CANT BE READ" << std::endl;
		}
		const char* vertexShader = vertexCode.c_str();
		const char* fragmentShader = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int succses;
		char infoLog[512];

		//create vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShader, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &succses);
		if (!succses) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "ERROR::FAIL_TO_COMPILE_VERTEX_SHADER" << std::endl;
		}
		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShader, NULL);
		glad_glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &succses);
		if (!succses) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "ERROR::FAIL_TO_COMPILE_FRAGMENT_SHADER" << std::endl;
		}
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void use() {
		glUseProgram(ID);
	}
	void deletePRograme() {
		glDeleteProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, const glm::mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};
#endif

