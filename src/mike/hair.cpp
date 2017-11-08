#include "hair.h"
#include "errorchecker.h"

/*
 * @file hair.cpp
 *
 * Stores the vertices representing a single simulated hair
 */

Hair::Hair(int numSegments, double length, glm::vec3 location, glm::vec3 dir, glm::vec3 normal)
{
    if (numSegments < 1)
        cerr << "Number of hair segments should be at least 1" << endl;

    if (length <= 0)
        cerr << "Hair length should be positive" << endl;

    dir = glm::normalize(dir);

    // Calculate basis vectors for plane orthogonal to dir.
    m_triangleFace[0] = glm::normalize(glm::vec3(-normal.y, normal.x, 0));
    m_triangleFace[1] = glm::cross(normal, m_triangleFace[0]);

    m_numSegments = numSegments;
    m_length = length;

    double stepSize = (double) length / numSegments;
    for (int i = 0; i < numSegments + 1; ++i)
    {
        HairVertex *newVert = new HairVertex(glm::vec3(location.x, location.y, location.z) + dir * (float) (stepSize * i));
        if (i > 0)
        {
            HairVertex *oldVert = m_vertices.at(i - 1);
            double dot = CLAMP(glm::dot(oldVert->position - newVert->position, glm::vec3(0, -1, 0)), -1.0, 1.0);
            double det = CLAMP(glm::dot(oldVert->position - newVert->position, glm::vec3(1, 0, 0)), -1.0, 1.0);
            newVert->theta = -atan2(det, dot);
            newVert->pointVector = dir;
        }
        newVert->segLen = stepSize;
        m_vertices.append(newVert);

    }

    GLfloat data[] = {-.5, +.5, 0,
                      +.5, +.5, 0,
                      -.5, -.5, 0,
                      +.5, -.5, 0};
    m_patch.create();
    m_patch.setVertexData(data, sizeof(data), 4);
    m_patch.setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

// Contructor for USC dataset
Hair::Hair(std::vector<glm::vec3> strand) {
    m_numSegments = strand.size();
    float length = 0;
    for (int i = 0; i < strand.size(); ++i) {
        HairVertex *newVert = new HairVertex(strand.at(i));
        if (i > 0)
        {
            HairVertex *oldVert = m_vertices.at(i - 1);
            double dot = CLAMP(glm::dot(oldVert->position - newVert->position, glm::vec3(0, -1, 0)), -1.0, 1.0);
            double det = CLAMP(glm::dot(oldVert->position - newVert->position, glm::vec3(1, 0, 0)), -1.0, 1.0);
            newVert->theta = -atan2(det, dot);
            newVert->pointVector = oldVert->position - newVert->position;
        }

        length += glm::length(newVert->pointVector);
        newVert->segLen = glm::length(newVert->pointVector);
        m_vertices.append(newVert);
    }
    m_length = length;
    //m_triangleFace[0] = glm::vec3(0.0f);
    //m_triangleFace[1] = glm::vec3(0.0f);

    GLfloat data[] = {-.5, +.5, 0,
                      +.5, +.5, 0,
                      -.5, -.5, 0,
                      +.5, -.5, 0};
    m_patch.create();
    m_patch.setVertexData(data, sizeof(data), 4);
    m_patch.setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

Hair::Hair(std::vector<glm::vec3> strand, std::vector<glm::vec3> colors){
    m_numSegments = strand.size();
    float length = 0;
    for (int i = 0; i < strand.size(); ++i) {
        HairVertex *newVert = new HairVertex(strand.at(i));
        if (i > 0)
        {
            HairVertex *oldVert = m_vertices.at(i - 1);
            double dot = CLAMP(glm::dot(oldVert->position - newVert->position, glm::vec3(0, -1, 0)), -1.0, 1.0);
            double det = CLAMP(glm::dot(oldVert->position - newVert->position, glm::vec3(1, 0, 0)), -1.0, 1.0);
            newVert->theta = -atan2(det, dot);
            newVert->pointVector = oldVert->position - newVert->position;
        }
        newVert->pointColor = colors.at(i);
        length += glm::length(newVert->pointVector);
        newVert->segLen = glm::length(newVert->pointVector);
        m_vertices.append(newVert);
    }
    m_length = length;
    //m_triangleFace[0] = glm::vec3(0.0f);
    //m_triangleFace[1] = glm::vec3(0.0f);

    GLfloat data[] = {-.5, +.5, 0,
                      +.5, +.5, 0,
                      -.5, -.5, 0,
                      +.5, -.5, 0};
    m_patch.create();
    m_patch.setVertexData(data, sizeof(data), 4);
    m_patch.setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

Hair::~Hair()
{
    m_patch.destroy();
    for (int i = 0; i < m_vertices.size(); ++i)
        delete m_vertices.at(i);
}

void Hair::update(float time)
{

}

void Hair::paint(ShaderProgram *_program)
{
    _program->uniforms.triangleFace[0] = m_triangleFace[0];
    _program->uniforms.triangleFace[1] = m_triangleFace[1];
    _program->uniforms.numHairVertices = MIN(m_vertices.size(), MAX_HAIR_VERTICES);
    _program->uniforms.length = m_length;
    for (int i = 0; i < _program->uniforms.numHairVertices; i++){
        _program->uniforms.vertexData[i] = m_vertices.at(i)->position;
        _program->uniforms.colorData[i] = m_vertices.at(i)->pointColor;
    }
    _program->setPerDrawUniforms();

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    m_patch.draw(GL_PATCHES);

}


