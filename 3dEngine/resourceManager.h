#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "stb_image.h"
#include "Shader.h"

class ResourceManager {
private:
    ResourceManager();

    static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile);

public:
    static std::map<std::string, Shader> shaders;

    static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
    static Shader getShader(std::string name);

    static void clear();

};
