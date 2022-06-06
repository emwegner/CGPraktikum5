//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

// LineBoxModel(2,3,4);
LineBoxModel::LineBoxModel(float Width, float Height, float Depth) {
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

	// 12 Seiten
	VB.addVertex(A);
	VB.addVertex(B);

	VB.addVertex(A);
	VB.addVertex(C);

	VB.addVertex(A);
	VB.addVertex(F);

	VB.addVertex(B);
	VB.addVertex(D);

	VB.addVertex(B);
	VB.addVertex(G);

	VB.addVertex(C);
	VB.addVertex(D);

	VB.addVertex(D);
	VB.addVertex(I);

	VB.addVertex(F);
	VB.addVertex(G);

	VB.addVertex(C);
	VB.addVertex(H);

	VB.addVertex(H);
	VB.addVertex(I);

	VB.addVertex(F);
	VB.addVertex(H);

	VB.addVertex(G);
	VB.addVertex(I);

	VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam) {
	BaseModel::draw(Cam);
	VB.activate();
	glDrawArrays(GL_LINES, 0, VB.vertexCount());
	VB.deactivate();
}