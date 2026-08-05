#include "MeshFactory.h"

MeshFactory::MeshFactory()
{
}

MeshFactory::~MeshFactory()
{
}

std::unique_ptr<MeshComponent> MeshFactory::CreateQuadGrid(unsigned int resolution, float size)
{
    std::vector<VERTEX> vertices;
    std::vector<GLuint> indices;

    for (unsigned int y = 0; y <= resolution; y++)
    {
        for (unsigned int x = 0; x <= resolution; x++)
        {
            float u = float(x) / resolution;
            float v = float(y) / resolution;

            VERTEX vert;

            vert.position =
            {
                (u - 0.5f) * size,
                0.0f,
                (v - 0.5f) * size
            };

            vert.normal = { 0,1,0 };
            vert.texCoord = { u,v };

            vertices.push_back(vert);
        }
    }

    unsigned int stride = resolution + 1;

    for (unsigned int y = 0; y < resolution; y++)
    {
        for (unsigned int x = 0; x < resolution; x++)
        {
            unsigned int v0 = y * stride + x;
            unsigned int v1 = v0 + 1;
            unsigned int v3 = v0 + stride;
            unsigned int v2 = v3 + 1;

            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }

    std::string name = "QuadGrid";

    std::unique_ptr<MeshComponent> quad = std::make_unique<MeshComponent>(name, vertices, indices);

	return quad;
}
