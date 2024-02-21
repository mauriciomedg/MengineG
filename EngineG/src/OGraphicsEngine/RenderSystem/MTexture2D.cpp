#include "MTexture2D.h"
#include <SOIL2/SOIL2.h>
#include <GL/glew.h>
#include <iostream>

using namespace MG;

MTexture2D::MTexture2D(const char* file_path)
{
	m_textureID = SOIL_load_OGL_texture(file_path,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (m_textureID == 0) std::cout << "could not find texture file" << file_path << std::endl;

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}
}

MTexture2D::~MTexture2D()
{
}

ui32 MTexture2D::getId()
{
	return m_textureID;
}
