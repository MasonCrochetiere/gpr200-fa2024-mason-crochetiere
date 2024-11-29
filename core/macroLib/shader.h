#ifndef SHADER_H
#define SHADER_H

// 

#include "../ew/external/glad.h" // include glad to get all the required OpenGL headers
 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

namespace macroLib
{
    class Shader
    {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(Shader& copy);
        // use/activate the shader
        void use();
        // utility uniform functions
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string name, const glm::mat4& m) const;
        void setVec3(const std::string name, const glm::vec3& value) const;
    };
}

#endif