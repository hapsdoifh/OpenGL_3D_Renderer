
#include <glm/glm.hpp>
#include "ShapeGenerator.h"
#include <Vertex.h>
#include <stb_image.h>


ShapeData ShapeGenerator::makeTriangle() {
	ShapeData res;
	Vertex thisTri[] = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),
	};
	res.numVertices = sizeof(thisTri) / sizeof(*thisTri);
	res.vertices = new Vertex[res.numVertices];
	memcpy(res.vertices, thisTri, sizeof(thisTri));

	GLushort indices[]{ 0,1,2 };
	res.numIndices = sizeof(indices) / sizeof(*indices);
	res.indices = new GLushort[res.numIndices];
	memcpy(res.indices, indices, sizeof(indices));
	return res;
}

//ShapeData ShapeGenerator::makeCube() {
//	ShapeData res;
//	Vertex StackVerts[] = {
//		glm::vec3(-1.0f, 1.0f, -1.0f), //Top Left 0
//		glm::vec3(1.0f, 0.0f, 0.0f),
//
//		glm::vec3(1.0f, 1.0f, -1.0f), //Top Right 1 
//		glm::vec3(0.0f, 1.0f, 0.0f),
//
//		glm::vec3(-1.0f, -1.0f, -1.0f), //Bottom Left 2
//		glm::vec3(0.0f, 0.0f, 1.0f),
//
//		glm::vec3(1.0f, -1.0f, -1.0f), //Bottom Right 3
//		glm::vec3(1.0f, 0.0f, 0.0f),
//
//		glm::vec3(-1.0f, 1.0f, 1.0f), //Top Left - Back 4
//		glm::vec3(1.0f, 0.0f, 1.0f),
//
//		glm::vec3(1.0f, 1.0f, 1.0f), //Top Right - Back 5
//		glm::vec3(0.0f, 1.0f, 1.0f),
//
//		glm::vec3(-1.0f, -1.0f, 1.0f), //Bottom Left - Back 6
//		glm::vec3(1.0f, 1.0f, 1.0f),
//
//		glm::vec3(1.0f, -1.0f, 1.0f), //Bottom Right - Back 7
//		glm::vec3(0.0f, 0.9f, 0.5f),
//	};
//	res.numVertices = sizeof(StackVerts) / sizeof(*StackVerts);
//	res.vertices = new Vertex[res.numVertices];
//	memcpy(res.vertices, StackVerts, sizeof(StackVerts));
//
//	GLushort indices[]{ 0,1,2, 2,1,3, 1,5,3, 5,7,3, 4,5,7, 4,7,6, 0,4,6, 0,6,2, 0,4,1, 4,5,1, 2,6,7, 2,7,3};
//	res.numIndices = sizeof(indices) / sizeof(*indices);
//	res.indices = new GLushort[res.numIndices];
//	memcpy(res.indices, indices, sizeof(indices));
//	return res;
//}

ShapeData ShapeGenerator::makeCube() {
	ShapeData res;

	// Define vertices for the cube with position and color attributes
	Vertex StackVerts[] = {
		// Front face
		glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,1.0f), // 0: Top-left
		glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,1.0f), // 1: Top-right
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f), // 2: Bottom-left
		glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,0.0f), // 3: Bottom-right

		// Back face
		glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,1.0f), // 4: Top-left
		glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,1.0f), // 5: Top-right
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f), // 6: Bottom-left
		glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,0.0f), // 7: Bottom-right

		// Top face
		glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,0.0f), // 8->0: Top-left
		glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,0.0f), // 9->1: Top-right
		glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,1.0f), // 10->4: Top-left
		glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,1.0f), // 11->5: Top-right

		// Bottom face
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f,1.0f), // 12->2: Bottom-left
		glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,1.0f), // 13->3: Bottom-right
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f), // 14->6: Bottom-left
		glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,0.0f), // 15->7: Bottom-right

		// Left face
		glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,0.0f), // 8->0: Top-left
		glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,0.0f), // 9->1: Top-right
		glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,1.0f), // 10->4: Top-left
		glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,1.0f), // 11->5: Top-right

		// Right face
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f,1.0f), // 12->2: Bottom-left
		glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,1.0f), // 13->3: Bottom-right
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f), // 14->6: Bottom-left
		glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,0.0f), // 15->7: Bottom-right
	};

	res.numVertices = sizeof(StackVerts) / sizeof(*StackVerts);
	res.vertices = new Vertex[res.numVertices];
	memcpy(res.vertices, StackVerts, sizeof(StackVerts));

	// Define indices for CCW triangles (two per face)
	GLushort indices[] = {
		// Front face
		0, 1, 2,  // Triangle 1
		2, 1, 3,  // Triangle 2

		// Back face
		4, 6, 5,  // Triangle 1 (CCW when viewed from back)
		5, 6, 7,  // Triangle 2

		// Top face
		//0, 4, 1,  // Triangle 1
		//1, 4, 5,  // Triangle 2
		8, 10, 9,  // Triangle 1
		9, 10, 11,  // Triangle 2

		// Bottom face
		//2, 3, 6,  // Triangle 1
		//6, 3, 7,  // Triangle 2
		14, 12, 15,  // Triangle 1
		15, 12, 13,  // Triangle 2

		// Left face
		0, 2, 4,  // Triangle 1
		4, 2, 6,  // Triangle 2

		// Right face
		1, 5, 3,  // Triangle 1
		3, 5, 7,  // Triangle 2
	};

	res.numIndices = sizeof(indices) / sizeof(*indices);
	res.indices = new GLushort[res.numIndices];
	memcpy(res.indices, indices, sizeof(indices));

	return res;
}