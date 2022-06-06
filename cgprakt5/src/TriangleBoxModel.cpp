//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

//

#include "TriangleBoxModel.h"

// Index und Vertex buffer
// Index Buffer erhält Index des Vertex im Vertex Buffer

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
    float x = Width * 0.5f;
    float y = Height * 0.5f;
    float z = Depth * 0.5f;

    Vector A = Vector(-x, -y, -z);
    Vector B = Vector(x, -y, -z);
    Vector C = Vector(-x, -y, z);
    Vector D = Vector(x, -y, z);

    Vector F = Vector(-x, y, -z);
    Vector G = Vector(x, y, -z);
    Vector H = Vector(-x, y, z);
    Vector I = Vector(x, y, z);

    VB.begin();

    // Erst Eigenschaften des Vertex, dann die Koordinaten
    // 1. TexCoord, 2. Normale, 3. Cord
    // Jeden Punkt 3 mal hinzufügen
    // Triangle 1
    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(1, 0);
    VB.addVertex(G);

    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(1, 1);
    VB.addVertex(B);

    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(0, 1);
    VB.addVertex(A);

    // Triangle 2
    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(0, 0);
    VB.addVertex(F);

    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(1, 0);
    VB.addVertex(G);

    VB.addNormal(0, 0, 1);
    VB.addTexcoord0(0, 1);
    VB.addVertex(A);

    // Triangle 3
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(G);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 0);
    VB.addVertex(I);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 1);
    VB.addVertex(B);

    // Triangle 4
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 0);
    VB.addVertex(I);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 1);
    VB.addVertex(D);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 1);
    VB.addVertex(B);

    // Triangle 5
    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(F);

    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(1, 0);
    VB.addVertex(H);

    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(1, 1);
    VB.addVertex(I);

    // Triangle 6
    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(F);

    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(1, 1);
    VB.addVertex(I);

    VB.addNormal(0, 1, 0);
    VB.addTexcoord0(0, 1);
    VB.addVertex(G);

    // Triangle 7 
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 1);
    VB.addVertex(A);


    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(H);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 0);
    VB.addVertex(F);

    // Triangle 8 
    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(H);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(1, 1);
    VB.addVertex(A);

    VB.addNormal(1, 0, 0);
    VB.addTexcoord0(0, 1);
    VB.addVertex(C);


    // Triangle 9 
    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(1, 1);
    VB.addVertex(C);

    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(0, 0);
    VB.addVertex(I);

    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(1, 0);
    VB.addVertex(H);

    // Triangle 10 
    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(1, 1);
    VB.addVertex(C);

    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(0, 1);
    VB.addVertex(D);

    VB.addNormal(0, 0, -1);
    VB.addTexcoord0(0, 0);
    VB.addVertex(I);

    // Triangle 11 
    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(0, 1);
    VB.addVertex(A);

    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(1, 0);
    VB.addVertex(D);

    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(1, 1);
    VB.addVertex(C);

    // Triangle 12
    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(0, 1);
    VB.addVertex(A);

    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(B);

    VB.addNormal(0, -1, 0);
    VB.addTexcoord0(1, 0);
    VB.addVertex(D);

    VB.end();

    IB.begin();

    for (int i = 0; i < 36; i++)
    {
        IB.addIndex(i);
    }

    IB.end();
}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);
    VB.activate();
    IB.activate();
    glDrawElements(GL_TRIANGLES, IB.indexCount(), GL_UNSIGNED_SHORT, 0);
    IB.deactivate();
    VB.deactivate();
}
