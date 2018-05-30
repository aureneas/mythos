#pragma once

#include "shader_sources.h"

class MythosShader {

	private:

		bool		mShaderLoaded = false;

		GLuint		mShader;

	public:

		~MythosShader();

		void		load(const char*, const char*);

		void		loadRaw(const char*, const char*);

		GLuint		getAttribLocation(const char*);

		GLuint		getUniformLocation(const char*);

		void		use(void);
};


void __mythosShaderInit(void);

MYTHOS_CORE_API MythosShader* mythosGetSolidShader(void);

MYTHOS_CORE_API MythosShader* mythosGetImageShader(void);