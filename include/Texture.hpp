//
// Created by Yaroslav on 29.07.2020.
//

#ifndef OPENGLTEST_TEXTURE_HPP
#define OPENGLTEST_TEXTURE_HPP


#include <string>
#include <GL/glew.h>
#include <vector>

class Texture
{
private:

public:
    GLuint texture;
    const char *name;
    unsigned char *data;
    int width, height, nrChannels;
    Texture(const char *name);
    ~Texture();
    void loadTexture();
    void bind();

    unsigned int loadCubemap(std::vector<std::string> faces);
    void updateTexture();

    static Texture* createEmptyTexture(int width, int height, int nrChannels, const char *name);
};


#endif //OPENGLTEST_TEXTURE_HPP
