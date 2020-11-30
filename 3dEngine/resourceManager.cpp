#include "ResourceManager.h"

#include "Logger.h"

std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name) {
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
    return shaders[name];
}


void ResourceManager::clear() {
    //delete shader and textures
    for (auto iter : shaders) {
        glDeleteProgram(iter.second.ID);
    }
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile) {
    std::string name = vShaderFile;
    name = name.substr(name.find_last_of("/") + 1, name.find_last_of(".") - name.find_last_of("/") - 1);
    std::string message = "Generating " + name + " shader";
    Logger::logMessage("RES", message.c_str());
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // optional geometry shader
        if (gShaderFile != nullptr) {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        Logger::logSuccess("RES", message.c_str());
    }
    catch (std::exception e) {
        Logger::logError("RES", message.c_str());
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

