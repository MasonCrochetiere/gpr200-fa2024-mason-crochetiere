#include "shader.h"
namespace macroLib {

    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment; 
        int success;
        char infoLog[512]; // arbitrary number to hold error log

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER); // similar to VBO, populating vertex with a location to pull from later
        glShaderSource(vertex, 1, &vShaderCode, NULL); // apply our source code to this shader. the 1 means we're only supplying 1 shader
        glCompileShader(vertex); // same as compiling any other code. Could fail...
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // similiar for Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER); // similar to VBO, populating fragment with a location to pull from later
        glShaderSource(fragment, 1, &fShaderCode, NULL); // apply our source code to this shader. the 1 means we're only supplying 1 shader
        glCompileShader(fragment); // same as compiling any other code. Could fail...
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };


        // shader Program
        ID = glCreateProgram(); // program combines shaders
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment); // attach our vertex and color shaders
        glLinkProgram(ID); // links shaders all together
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        // shader cleanup
    }

    void Shader::use()
    {
        glUseProgram(ID); // need to make sure we use the program :)
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void Shader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setMat4(const std::string name, const glm::mat4& m) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &m[0][0]);
    }

    // combining the translate, rotate, and scale matrix into one template that we apply pos to next
    // scale first, then rotate, then translate. This is written in reverse order by multiplication apparently

    // this matrix is a model matrix; it brings an object from model space to world space
    // don't really make this in the vertex shader. it hurts performance and will not let us use this information for game logic.
    // instead the matrix is best as a uniform passed to the vertex shader

    void Shader::setVec3(const std::string name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
}