#pragma once
#include "PlyModel.h"
#include <iostream>

using namespace std;

float calcArea(glm::vec3 p1,glm::vec3 p2, glm::vec3 p3){
    glm::vec3 edge1 = p1 - p2;
    glm::vec3 edge2 = p1 - p3;
    glm::vec3 crossdot = glm::cross(edge1,edge2);

    return glm::length(crossdot) * 0.5f;
}

glm::vec3 calcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
    glm::vec3 edge1 = p1 - p2;
    glm::vec3 edge2 = p1 - p3;
    return glm::normalize(glm::cross(edge1,edge2));
}

PlyProperty vert_props[] = { // list of property information for a vertex
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,z), 0, 0, 0, 0},
  {"nx", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,nx), 0, 0, 0, 0},
  {"ny", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,ny), 0, 0, 0, 0},
  {"nz", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,nz), 0, 0, 0, 0},
  {"red", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVertex,r), 0, 0, 0, 0},
  {"green", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVertex,g), 0, 0, 0, 0},
  {"blue", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVertex,b), 0, 0, 0, 0},
};

PlyProperty face_props[] = { /* list of property information for a face */
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(PlyFace,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(PlyFace,nverts)},
};

PlyModel::PlyModel(){
}

PlyModel::~PlyModel(){
}

bool PlyModel::loadPly(const char *fn, bool & normal, bool & color) {

	FILE *f = fopen(fn, "rb");
	if (f == NULL) {
		cerr << "couldn't open " << fn << endl;
		return false;
	}

	int nelems;
	char **elemNames;
	PlyFile *ply = ply_read(f, &nelems, &elemNames);
	if (ply == NULL) {
		cerr << "couldn't read ply file " << fn << endl;
		return false;
	}

    normal = true;
    color = true;
	// go through each kind of element that we learned is in the file 
	// and read them
	char *elemName;
	PlyProperty **plist;
	int numElems, nprops;
	int i, j;
	for (i = 0; i < nelems; i++) {
		// get the description of the first element
		elemName = elemNames[i];
		plist = ply_get_element_description(ply, elemName, &numElems, &nprops);

		// if we're on vertex elements, read them in
		if (strcmp("vertex", elemName) == 0) {
			// set up for getting vertex elements
			ply_get_property (ply, elemName, &vert_props[0]);
			ply_get_property (ply, elemName, &vert_props[1]);
			ply_get_property (ply, elemName, &vert_props[2]);
            if (normal) {
				normal = (ply_get_property (ply, elemName, &vert_props[3]) != 0 && 
				ply_get_property (ply, elemName, &vert_props[4]) != 0 &&
				ply_get_property (ply, elemName, &vert_props[5]) != 0);
            }
			if (color) {
                color = (ply_get_property (ply, elemName, &vert_props[6]) != 0 &&
                ply_get_property (ply, elemName, &vert_props[7]) != 0 &&
                ply_get_property (ply, elemName, &vert_props[8]) !=0);
			}

			// grab all the vertex elements
			for (j = 0; j < numElems; j++) {
				// grab an element from the file
				PlyVertex vert;
				ply_get_element (ply, &vert);
                xyz.push_back(glm::vec3(vert.x,vert.y,vert.z));
				if (color){
                    colors.push_back(glm::vec3(vert.r,vert.g,vert.b)/255.0f);
				}
				if (normal){
                    normals.push_back(glm::normalize(glm::vec3(vert.nx,vert.ny,vert.nz)));
				}
		
			}
		}

		// if we're on face elements, read them in
		if (strcmp("face", elemName) == 0) {
			// set up for getting face elements
			ply_get_property (ply, elemName, &face_props[0]);
      
			// grab all the face elements
			for (j = 0; j < numElems; j++) {
				// grab an element from the file
				PlyFace face;
				ply_get_element(ply, &face);

				vector<int> tri;
				tri.push_back(face.verts[0]);
				tri.push_back(face.verts[1]);
				tri.push_back(face.verts[2]);
				triangles.push_back(tri);

				free(face.verts);
			}
		}
	}

	ply_close(ply);

//	cout << "loaded " << curTris.size() << " vertices, " << m_tris.size() << " triangles" << endl;

	return true;
}

void PlyModel::calculateVertexNormal(){
    vector<float> surfaceArea (triangles.size(),0.0f);
    vector<glm::vec3> surfaceNormal (triangles.size(),glm::vec3(0.0f));

    vector<unordered_set<int>> neFaceInds (xyz.size());
    normals.clear();
    normals.resize(xyz.size());

    for(int i =0;i<triangles.size();++i){
        surfaceArea[i] = calcArea(xyz[triangles[i][0]],xyz[triangles[i][1]],xyz[triangles[i][2]]);
        surfaceNormal[i] = calcNormal(xyz[triangles[i][0]],xyz[triangles[i][1]],xyz[triangles[i][2]]);
        if(neFaceInds[triangles[i][0]].find(i)==neFaceInds[triangles[i][0]].end()){
           neFaceInds[triangles[i][0]].insert(i);
        }
        if(neFaceInds[triangles[i][1]].find(i)==neFaceInds[triangles[i][1]].end()){
           neFaceInds[triangles[i][1]].insert(i);
        }
        if(neFaceInds[triangles[i][2]].find(i)==neFaceInds[triangles[i][2]].end()){
           neFaceInds[triangles[i][2]].insert(i);
        }

    }

    for(int i =0;i<xyz.size();++i){
        for(auto it = neFaceInds[i].begin(); it != neFaceInds[i].end();++ it){
            normals[i] += surfaceNormal[*it] * surfaceArea[*it];
        }
        normals[i] = glm::normalize(normals[i]);
    }


}

void PlyModel::createBuffer(){

    for(int i=0;i<triangles.size();i++){
        xyz_buffer.push_back(xyz[triangles[i][0]]);
        xyz_buffer.push_back(xyz[triangles[i][1]]);
        xyz_buffer.push_back(xyz[triangles[i][2]]);
    }

    if(normals.size()==0){
        calculateVertexNormal();
    }
    for(int i=0;i<triangles.size();i++){
        normal_buffer.push_back(normals[triangles[i][0]]);
        normal_buffer.push_back(normals[triangles[i][1]]);
        normal_buffer.push_back(normals[triangles[i][2]]);
    }

    if(colors.size()>0){
        for(int i=0;i<triangles.size();i++){
            color_buffer.push_back(colors[triangles[i][0]]);
            color_buffer.push_back(colors[triangles[i][1]]);
            color_buffer.push_back(colors[triangles[i][2]]);
        }
    }
    else{
        for(int i =0; i<triangles.size();++i){
            color_buffer.push_back(glm::vec3(1.0f));
            color_buffer.push_back(glm::vec3(1.0f));
            color_buffer.push_back(glm::vec3(1.0f));
        }
    }
}
