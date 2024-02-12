#include"Utils.h"
#include <iostream>
#include <fstream>
#include <SOIL2/SOIL2.h>

#include <fstream>
#include <sstream>


using namespace std;

GLuint Utils::loadTexture(const char* texImagePath)
{
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texImagePath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) cout << "could not find texture file" << texImagePath << endl;
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	return textureID;
}

string Utils::readShaderSource(const wchar_t* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint Utils::createShaderProgram(const char* vp, const char* fp)
{
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	//const char* vshaderSource =
	//	"#version 430 \n"
	//	"void main(void) \n"
	//	"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
	//const char* fshaderSource =
	//	"#version 430 \n"
	//	"out vec4 color; \n"
	//	"void main(void) \n"
	//	//"{ color = vec4(1.0, 1.0, 1.0, 1.0); }";
	//	"{ if (gl_FragCoord.x < 295) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0); }";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vp, NULL);
	glShaderSource(fShader, 1, &fp, NULL);

	// catch errors while compiling shaders
	glCompileShader(vShader);
	Utils::checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		Utils::printShaderLog(vShader);
	}
	glCompileShader(fShader);
	Utils::checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		Utils::printShaderLog(fShader);
	}
	///////////////////////////////////////
	// catch errors while linking shaders
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	Utils::checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		Utils::printProgramLog(vfProgram);
	}

	///////////////////////////////////////
	return vfProgram;
}

void Utils::printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}
void Utils::printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}
bool Utils::checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

//////////////////

// ------------ Imported Model class
ImportedModel::ImportedModel(const char* filePath) {
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath); // uses modelImporter to get vertex information
	numVertices = modelImporter.getNumVertices();
	std::vector<float> verts = modelImporter.getVertices();
	std::vector<float> tcs = modelImporter.getTextureCoordinates();
	std::vector<float> normals = modelImporter.getNormals();
	for (int i = 0; i < numVertices; i++) {
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
}
int ImportedModel::getNumVertices() { return numVertices; } // accessors
std::vector<glm::vec3> ImportedModel::getVertices() { return vertices; }
std::vector<glm::vec2> ImportedModel::getTextureCoords() { return texCoords; }
std::vector<glm::vec3> ImportedModel::getNormals() { return normalVecs; }
// -------------- Model Importer class
ModelImporter::ModelImporter() {}
void ModelImporter::parseOBJ(const char* filePath) {
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0) { // vertex position ("v" case)
			stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z; // extract the vertex position values
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
		}
		
		if (line.compare(0, 2, "vt") == 0) { // texture coordinates ("vt" case)
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; // extract texture coordinate values
			stVals.push_back(x);
			stVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0) { // vertex normals ("vn" case)
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z; // extract the normal vector values
			normVals.push_back(x);
			normVals.push_back(y);
			normVals.push_back(z);
		}
		if (line.compare(0, 1, "f") == 0) { // triangle faces ("f" case)
			string oneCorner, v, t, n;
			stringstream ss(line.erase(0, 2));
			for (int i = 0; i < 3; i++) {
				getline(ss, oneCorner, ' '); // extract triangle face references
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');
				int vertRef = (stoi(v) - 1) * 3; // "stoi" converts string to integer
				int tcRef = (stoi(t) - 1) * 2;
				int normRef = (stoi(n) - 1) * 3;
				triangleVerts.push_back(vertVals[vertRef]); // build vector of vertices
				triangleVerts.push_back(vertVals[vertRef + 1]);
				triangleVerts.push_back(vertVals[vertRef + 2]);
				textureCoords.push_back(stVals[tcRef]); // build vector of texture coords
				textureCoords.push_back(stVals[tcRef + 1]);
				normals.push_back(normVals[normRef]); //… and normals
				normals.push_back(normVals[normRef + 1]);
				normals.push_back(normVals[normRef + 2]);
			}
		}
	}
}

int ModelImporter::getNumVertices() { return (triangleVerts.size() / 3); } // accessors
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTextureCoordinates() { return textureCoords; }
std::vector<float> ModelImporter::getNormals() { return normals; }