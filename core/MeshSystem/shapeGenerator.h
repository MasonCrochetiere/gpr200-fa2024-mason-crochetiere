//written by Reece but nearly copied from procGen example file, so credit to eric winebrenner

#ifndef SHAPEGENERATOR_H
#define SHAPEGENERATOR_H
#include <stdio.h>
#include <math.h>

#include "../ew/external/glad.h"
#include "../ew/ewMath/ewMath.h"
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"

using namespace ew;

namespace meshSystem {
    void generateCube(float size, MeshData* meshData);
    void generatePlane(float width,float height, int subDivisions, MeshData* meshData);
}




#endif //SHAPEGENERATOR_H
