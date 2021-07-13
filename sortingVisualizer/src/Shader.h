#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
class Shader
{
private:
	string m_VFileName;
	string m_FFileName;
	unsigned int m_RendererID;
	unordered_map<string, unsigned int> m_UniformLocationCache;

	int GetUniformLocation(const string& name);
	stringstream ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
public:
	
	Shader();
	Shader(const string& VFileName,const string& FFileName);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform4f(const string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const string& name, const glm::mat4 matrix);
	void SetUniform1i(const string& name, int value);
};

