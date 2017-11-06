#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "hairCommon.h"
#include <map>
#include <string>

#define MAX_HAIR_VERTICES 64

struct Uniforms {
    glm::mat4 model, view, projection;

    glm::vec3 lightPosition;

    glm::mat4 eyeToLight; // Matrix for rendering shadow map (eye space --> light space).

    int numHairVertices; // Number of vertices per guide hair.

    glm::vec3 vertexData[MAX_HAIR_VERTICES]; // Vertex position data for the current guide hair.

    int numGroupHairs; // Number of single-hair-interpolated hairs per guide hair.

    int numSplineVertices; // Number of vertices rendered with a spline.

    float hairGroupSpread; // Max distance from a hair to its corresponding guide hair.

    float hairRadius; // The radius of a single hair.

    float taperExponent; // Controls how far along the hair it starts tapering at the end.

    float noiseAmplitude; // Amount of noise added to each hair vertex poistion.
    float noiseFrequency;

    glm::vec3 triangleFace[2]; // Basis vectors for the plane orthogonal to the hair's normal vector.

    glm::vec3 color;

    float length;

    // Texture uniforms
    int noiseTexture;
    int hairShadowMap;
    int meshShadowMap;
    int opacityMap;
    int hairGrowthMap;
    int depthPeelMap;

    float specIntensity;
    float diffuseIntensity;

    float opacity;
    float maxColorVariation;

    // Shadows
    bool useShadows;
    float shadowIntensity; // Controls the shadow darkness
};

class ShaderProgram
{
public:
    ShaderProgram() { }

    virtual ~ShaderProgram() { }

    virtual void create();

    virtual void bind();

    virtual void unbind();

    // Sets all uniforms that do not change between objects.
    virtual void setGlobalUniforms() { }

    // Sets all uniforms that change between objects.
    virtual void setPerObjectUniforms() { }

    // Sets all uniforms that change between draw calls (if each object uses multiple draw calls).
    virtual void setPerDrawUniforms() { }

    Uniforms uniforms;

    GLuint id;

protected:
    // Calls one of the program creation functions in ResourceLoader, and returns the program ID.
    virtual GLuint createShaderProgram() = 0;

    void setUniform1i(GLchar const *name, int value);
    void setUniform1f(GLchar const *name, float value);
    void setUniform3f(GLchar const *name, glm::vec3 &value);
    void setUniform3fv(GLchar const *name, GLsizei count, glm::vec3 *values);
    void setUniformMatrix4f(GLchar const *name, glm::mat4 &value);

    std::map<std::string, int> m_uniformLocs;

private:
    GLint getUniformLocation(GLchar const *name);

    bool m_created = false;
};

#endif // SHADERPROGRAM_H
