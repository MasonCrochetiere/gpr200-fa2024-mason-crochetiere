// typed and taken from
//https://github.com/ewinebrenner/FA24-GPR-200-Example/blob/main/core/ew/procGen.cpp

#include "procGen.h"
#include <stdlib.h>

namespace ew
{
	void createSphere(float radius, int subDivisions, MeshData* mesh)
	{
		mesh->vertices.clear();
		mesh->indices.clear();
		mesh->vertices.reserve((subDivisions + 1) * (subDivisions + 1));
		mesh->indices.reserve(subDivisions * subDivisions * 6);

		//SPHERE VERTICES
		// used from example w/ changed variables to match mine
		// https://docs.google.com/document/d/1_IIt49iji72oS7ZHgCI8g23-LoVysRddWR9CosIpSTI/edit?tab=t.0#heading=h.ybqgw7sx35t6
		float thetaStep = (2 * PI) / subDivisions;
		float phiStep = PI / subDivisions;
		for (size_t row = 0; row <= subDivisions; row++)
		{
			float phi = row * phiStep;
			for (size_t col = 0; col <= subDivisions; col++)
			{
				float theta = col * thetaStep;
				glm::vec2 uv;
				uv.x = 1.0 - ((float)col / subDivisions);
				uv.y = 1.0 - ((float)row / subDivisions);
				// the setting of the xyz is done using the vertex struct
				glm::vec3 pos;
				float cosineTheta = cosf(theta);
				float sinePhi = sinf(phi);

				pos.x = cosineTheta * sinePhi;
				pos.y = sinePhi;
				pos.z = cosineTheta * sinePhi;
				
				// bc pos isn't multiplied bu radius it is already the normal, minor optimization change
				mesh->vertices.emplace_back((pos * radius), pos, uv);
			}
		}
		
		//SPHERE INDICIES
		// should be identical to the subdivided plane ones
		for (size_t row = 0; row < subDivisions; row++)
		{
			for (size_t col = 0; col < subDivisions; col++)
			{
				unsigned int bl = row * (subDivisions + 1) + col;
				unsigned int br = bl + 1;
				unsigned int tl = bl + subDivisions + 1;
				unsigned int tr = tl + 1;

				// Triangle 1
				mesh->indices.emplace_back(bl);
				mesh->indices.emplace_back(br);
				mesh->indices.emplace_back(tr);
				// Triangle 2
				mesh->indices.emplace_back(tr);
				mesh->indices.emplace_back(bl);
				mesh->indices.emplace_back(bl);			}
		}
		return;
	}
}