#include "objmesh.h"
#include "PlyModel.h"
#include "errorchecker.h"
#include "objloader.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/random.hpp>
#include "QTime"

#include "meshocttree.h"

#define _ELLIPSOID_COLISIONS_ true

extern float X_angle;
extern float Y_angle;
extern float Z_angle;
ObjMesh::ObjMesh()
{
}

void ObjMesh::init(const char *objFile, float scale)
{
    glm::mat4 transformation = glm::translate(glm::vec3(0.0,1.7,0.0)) *
            glm::rotate(X_angle,glm::vec3(1,0,0)) *
            glm::rotate(Y_angle,glm::vec3(0,1,0)) *
            glm::rotate(Z_angle,glm::vec3(0,0,1)) *
            glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-1.7,0.0));

    if(strstr(objFile,".obj") || strstr(objFile,".OBJ")){
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        bool loaded = OBJLoader::loadOBJ(objFile, vertices, uvs, normals);

        for(int i=0;i<vertices.size();++i){
            glm::vec4 temp = transformation * glm::vec4(vertices[i],1.0);
            vertices[i].x = temp.x;
            vertices[i].y = temp.y;
            vertices[i].z = temp.z;
        }
        if (!loaded) {
            printf("Failed to load OBJ: %s\n", objFile);
            exit(1);
        }

        cout<<vertices.size()<<endl;
        // Initialize m_triangles
        for (unsigned int i=0; i < vertices.size()/3; i++) {
            unsigned int i1 = i*3, i2 = i1+1, i3=i2+1;
            Triangle t(vertices[i1] * scale, vertices[i2] * scale, vertices[i3] * scale,
                       uvs[i1], uvs[i2], uvs[i3],
                       normals[i1], normals[i2], normals[i3]);
            triangles.push_back(t);
        }

        // Initialize vbo
        std::vector<GLfloat> vboData;
        for (unsigned int i=0; i < vertices.size(); i++) {
            vboData.push_back(vertices[i].x);
            vboData.push_back(vertices[i].y);
            vboData.push_back(vertices[i].z);
            vboData.push_back(uvs[i].x);
            vboData.push_back(1 - uvs[i].y);
            vboData.push_back(normals[i].x);
            vboData.push_back(normals[i].y);
            vboData.push_back(normals[i].z);

            // Set object boundaries.
            m_min = glm::min(m_min, scale * vertices[i]);
            m_max = glm::max(m_max, scale * vertices[i]);
        }

        m_shape.create();
        m_shape.setVertexData(&vboData[0], sizeof(GLfloat) * vboData.size(), vertices.size());
        m_shape.setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
        m_shape.setAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 3 * sizeof(GLfloat));
        m_shape.setAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 5 * sizeof(GLfloat));

    }
    else
        if(strstr(objFile,".ply") || strstr(objFile,".PLY")){
            bool normal = true,color = true;
            PlyModel plymodel;
            bool loaded = plymodel.loadPly(objFile,normal,color);
            for(int i=0;i<plymodel.xyz.size();++i){
                glm::vec4 temp = transformation * glm::vec4(plymodel.xyz[i],1.0);
                plymodel.xyz[i].x = temp.x;
                plymodel.xyz[i].y = temp.y;
                plymodel.xyz[i].z = temp.z;
            }
            plymodel.createBuffer();
          //  cout<<plymodel.xyz_buffer[0][0]<<","<<plymodel.xyz_buffer[0][1]<<","<<plymodel.xyz_buffer[0][2]<<endl;
          //  cout<<plymodel.xyz_buffer[1][0]<<","<<plymodel.xyz_buffer[1][1]<<","<<plymodel.xyz_buffer[1][2]<<endl;
          //  cout<<plymodel.xyz_buffer[2][0]<<","<<plymodel.xyz_buffer[2][1]<<","<<plymodel.xyz_buffer[2][2]<<endl;

            if (!loaded) {
                printf("Failed to load PLY: %s\n", objFile);
                exit(1);
            }

            cout<<"Load Ply done."<<endl;
            // Initialize m_triangles
            for (unsigned int i=0; i < plymodel.xyz_buffer.size()/3; i++) {
                unsigned int i1 = i*3, i2 = i1+1, i3=i2+1;
               Triangle t(plymodel.xyz_buffer[i1] * scale, plymodel.xyz_buffer[i2] * scale, plymodel.xyz_buffer[i3] * scale,
                          glm::vec2(0.0f),glm::vec2(0.0f),glm::vec2(0.0f),
                           plymodel.normal_buffer[i1], plymodel.normal_buffer[i2], plymodel.normal_buffer[i3],
                          plymodel.color_buffer[i1],plymodel.color_buffer[i2],plymodel.color_buffer[i3]);
               triangles.push_back(t);
            }

            // Initialize vbo
            std::vector<GLfloat> vboData;
            for (unsigned int i=0; i < plymodel.xyz_buffer.size(); i++) {
                vboData.push_back(plymodel.xyz_buffer[i][0]);
                vboData.push_back(plymodel.xyz_buffer[i][1]);
                vboData.push_back(plymodel.xyz_buffer[i][2]);
                vboData.push_back(0.0);
                vboData.push_back(0.0);
                vboData.push_back(plymodel.normal_buffer[i][0]);
                vboData.push_back(plymodel.normal_buffer[i][1]);
                vboData.push_back(plymodel.normal_buffer[i][2]);
                vboData.push_back(plymodel.color_buffer[i][0]);
                vboData.push_back(plymodel.color_buffer[i][1]);
                vboData.push_back(plymodel.color_buffer[i][2]);

                // Set object boundaries.
                m_min = glm::min(m_min, scale * plymodel.xyz_buffer[i]);
                m_max = glm::max(m_max, scale * plymodel.xyz_buffer[i]);
            }

            m_shape.create();
            m_shape.setVertexData(&vboData[0], sizeof(GLfloat) * vboData.size(), plymodel.xyz_buffer.size());
            m_shape.setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, 0);
            m_shape.setAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, 3 * sizeof(GLfloat));
            m_shape.setAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, 5 * sizeof(GLfloat));
            m_shape.setAttribute(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, 8 * sizeof(GLfloat));
        }
        else{
            cerr<<"unknown mesh type."<<endl;
        }

}

void ObjMesh::draw()
{
    m_shape.draw(GL_TRIANGLES);
}

/*
 * Shoot a ray from the point and if it intersects an even number of triangles, it is outside
 * If it intersects an odd number of triangles it is inside
 * TODO: Generate a random vector more elegantly
 */
bool ObjMesh::contains(glm::vec3 &normal, glm::vec3 ro, float &insideDist)
{
#if _ELLIPSOID_COLISIONS_
#define _SQV_(v) glm::pow(v, glm::vec3(2.f))
#define _SUM_(v) glm::dot(v, glm::vec3(1.f))


    normal = glm::normalize(ro / _SQV_(m_max));

    float phi = acos( glm::dot(normal, glm::vec3(0, 0, 1)) );
    float theta = acos( glm::dot( glm::vec3(normal.x, normal.y, 0), glm::vec3(1, 0, 0) ) );

    glm::vec3 v = glm::vec3(cos(theta) * sin(phi), sin(theta) * cos(phi), cos(phi));
    float r = 1.f / glm::sqrt(_SUM_(_SQV_( v / m_max )));
    insideDist = r - glm::length(ro);

    return _SUM_(_SQV_(ro/m_max)) < 1;

#undef _SUM_
#undef _SQV_
#else
    // Return false if point is outside bounding cube.
    if (glm::any(glm::lessThan(ro, m_min)) || glm::any(glm::greaterThan(ro, m_max)))
        return false;

    int numIntersections = 0;
    double rand1 = rand() % 1000; double rand2 = rand() % 1000; double rand3 = rand() % 1000;
    glm::vec3 randDir = glm::normalize(glm::vec3(rand1, rand2, rand3));
    randDir = glm::normalize(ro);

//    OctTree, I think it works but not sure of performance
//    QList<Triangle> reducedTriangles;
//    m_octTree->getRelevantTriangles(reducedTriangles, ro, randDir);

    for (unsigned int i = 0; i < triangles.size(); ++i)
    {
        Triangle currTriangle = triangles.at(i);
        glm::vec3 intersectionPoint = glm::vec3(0.0);

        if (currTriangle.intersect(intersectionPoint, ro, randDir))
        {
            normal = (currTriangle.n1 + currTriangle.n2 + currTriangle.n3) / 3.0f;
            numIntersections++;
        }
    }
    return (numIntersections % 2);
#endif
}
