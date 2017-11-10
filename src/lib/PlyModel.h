#include <vector>
#include <unordered_set>
#include "ply_io.h"
#include "hairCommon.h"

using namespace std;

struct PlyVertex {
	float x,y,z;
	float nx,ny,nz;
	unsigned char r, g, b;
};

struct PlyFace {
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
};

class PlyModel{
public:
	PlyModel();
	~PlyModel();
	
    bool loadPly(const char *fn, bool & normal, bool &color);
    void createBuffer();

    void calculateVertexNormal();

    vector<glm::vec3> xyz;
    vector<glm::vec3> normals;
    vector<glm::vec3> colors;

    vector<vector<int>> triangles;

    vector<glm::vec3> xyz_buffer;
    vector<glm::vec3> normal_buffer;
    vector<glm::vec3> color_buffer;

};
