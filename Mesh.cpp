#include "Mesh.h"



std::vector<std::string> Mesh::split(std::string str, char Delimiter) {
	std::istringstream iss(str);             // istringstream에 str을 담는다.
	std::string buffer;                      // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼

	std::vector<std::string> result;

	// istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
	while (getline(iss, buffer, Delimiter)) {
		result.push_back(buffer);               // 절삭된 문자열을 vector에 저장
	}

	return result;
}


void Mesh::SaveFaceNormal()
{
	for (int i = 0; i < faces.size(); i++)
	{
		// Compute FaceNormal
		Vector3 v0 = vertices[faces[i].vertex0].position;
		Vector3 v1 = vertices[faces[i].vertex1].position;
		Vector3 v2 = vertices[faces[i].vertex2].position;

		Vector3 va = v1 - v0;
		Vector3 vb = v2 - v0;

		Vector3 vc = va.CrossProduct(vb);
		vc.Normalize();

		neighborNormal[faces[i].vertex0].push_back(vc);
		neighborNormal[faces[i].vertex1].push_back(vc);
		neighborNormal[faces[i].vertex2].push_back(vc);
	}
}


void Mesh::ComputeVertexNormal()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		Vector3 vertexNormal = Vector3(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < neighborNormal[i].size(); j++)
		{
			vertexNormal += neighborNormal[i][j];
		}

		vertexNormal.Normalize();

		vertices[i].Normal = vertexNormal;
	}
}


void Mesh::CreateMesh(std::string meshName)
{
	std::string file = "../mesh-data/";
	file += meshName;
	std::cout << file << std::endl;

	//char* meshFilePath = "../mesh-data/bunny.off";
	char* meshFilePath = new char[sizeof(file)];
	strcpy(meshFilePath, file.c_str());
	char* meshFileData = textFileRead(meshFilePath);

	if (meshFileData == NULL)
		std::cout << "Fail to read mesh data" << std::endl;
	else
		std::cout << "Success to read mesh data" << std::endl;

	std::string meshData = meshFileData;
	std::vector<std::string> result = split(meshData, '\n');
	std::vector<std::string> metaData = split(result[1], ' ');

	numVertices = stoi(metaData[0]);
	numFaces = stoi(metaData[1]);

	std::cout << numVertices << std::endl;
	std::cout << numFaces << std::endl;

	neighborNormal = new std::vector<Vector3>[numVertices];

	for (int i = 0; i < numVertices; i++)
	{
		std::vector<std::string> triangle = split(result[2 + i], ' ');

		//std::cout << triangle[0] << ' ' << triangle[1] << ' ' << triangle[2] << std::endl;

		Vertex vertex;
		vertex.position = Vector3(stof(triangle[0]), stof(triangle[1]), stof(triangle[2]));

		vertices.push_back(vertex);
	}

	for (int i = 0; i < numFaces; i++)
	{
		std::vector<std::string> vertexIndex = split(result[2 + numVertices + i], ' ');

		VertexIndex face = VertexIndex(stoul(vertexIndex[1]), stoul(vertexIndex[2]), stoul(vertexIndex[3]));
		faces.push_back(face);

		Vector3 v0 = vertices[face.vertex0].position;
		Vector3 v1 = vertices[face.vertex1].position;
		Vector3 v2 = vertices[face.vertex2].position;

		Vector3 va = v1 - v0;
		Vector3 vb = v2 - v0;

		Vector3 vc = va.CrossProduct(vb);
		vc.Normalize();

		neighborNormal[face.vertex0].push_back(vc);
		neighborNormal[face.vertex1].push_back(vc);
		neighborNormal[face.vertex2].push_back(vc);
	}

	//SaveFaceNormal();
	ComputeVertexNormal();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(VertexIndex), &faces[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glBindVertexArray(0);
}

