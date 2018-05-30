#include <fstream>
#include <sstream>
#include <vector>
#include <mythos\shaders\shader.h>

#include <iostream>


GLuint  __attachShader(GLuint shaderProg, GLenum shaderType, const char* shaderPath) {

	GLuint shader = glCreateShader(shaderType);

	std::string shaderSource;
	std::ifstream shaderStream(shaderPath, std::ios::in);
	
	if (shaderStream.is_open()) {

		std::stringstream shaderSStream;
		shaderSStream << shaderStream.rdbuf();
		shaderSource = shaderSStream.str();
		shaderStream.close();
	}
	else {

		std::string errorMessage = std::string("Unable to open shader file ") + std::string(shaderPath);
		throw MythosError(errorMessage);
	}

	const char* shaderCSource = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderCSource, NULL);
	glCompileShader(shader);

	GLint compileStatus = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {

		int shaderInfoLogLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderInfoLogLen);

		if (shaderInfoLogLen > 0) {

			char* shaderInfoLog = new char[shaderInfoLogLen + 1];
			glGetShaderInfoLog(shader, shaderInfoLogLen + 1, NULL, shaderInfoLog);

			switch (shaderType) {

				case GL_VERTEX_SHADER:
					throw MythosError("Vertex Shader Compile Error", shaderInfoLog);

				case GL_FRAGMENT_SHADER:
					throw MythosError("Fragment Shader Compile Error", shaderInfoLog);
			}
		}
	}

	glAttachShader(shaderProg, shader);

	return shader;
}

GLuint  __attachShaderRaw(GLuint shaderProg, GLenum shaderType, const char* shaderSource) {

	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	GLint compileStatus = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {

		int shaderInfoLogLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderInfoLogLen);

		if (shaderInfoLogLen > 0) {

			char* shaderInfoLog = new char[shaderInfoLogLen + 1];
			glGetShaderInfoLog(shader, shaderInfoLogLen + 1, NULL, shaderInfoLog);

			switch (shaderType) {

				case GL_VERTEX_SHADER:
					throw MythosError("Vertex Shader Compile Error", shaderInfoLog);

				case GL_FRAGMENT_SHADER:
					throw MythosError("Fragment Shader Compile Error", shaderInfoLog);
			}
		}
	}

	glAttachShader(shaderProg, shader);

	return shader;
}


MythosShader::~MythosShader() {

	if (mShaderLoaded)
		glDeleteProgram(mShader);
}

void MythosShader::load(const char* vertShaderPath, const char* fragShaderPath) {

	GLuint mShader = glCreateProgram();
	mShaderLoaded = true;

	try {

		GLuint vertShader = __attachShader(mShader, GL_VERTEX_SHADER, vertShaderPath);
		GLuint fragShader = __attachShader(mShader, GL_FRAGMENT_SHADER, fragShaderPath);

		glLinkProgram(mShader);

		GLint programLinkStatus = GL_FALSE;
		glGetProgramiv(mShader, GL_LINK_STATUS, &programLinkStatus);

		if (programLinkStatus != GL_TRUE) {

			int programInfoLogLen;
			glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &programInfoLogLen);

			if (programInfoLogLen > 0) {

				char* programInfoLog = new char[programInfoLogLen + 1];
				glGetShaderInfoLog(mShader, programInfoLogLen + 1, NULL, programInfoLog);
				throw MythosError("Shader Link Error", programInfoLog);
			}
		}

		glDetachShader(mShader, vertShader);
		glDetachShader(mShader, fragShader);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}
	catch (MythosError e) {

		e.display();
	}
}

void MythosShader::loadRaw(const char* vertShaderSource, const char* fragShaderSource) {

	if (mShaderLoaded) {

		glDeleteProgram(mShader);
		mShaderLoaded = false;
	}

	GLuint ProgramID;

	try {

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		char const * VertexSourcePointer = vertShaderSource;
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (Result == GL_FALSE && InfoLogLength > 0) {

			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			throw MythosError("Vertex Shader Compile Error", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		char const * FragmentSourcePointer = fragShaderSource;
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (Result == GL_FALSE && InfoLogLength > 0) {

			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			throw MythosError("Fragment Shader Compile Error", &FragmentShaderErrorMessage[0]);
		}



		// Link the program
		printf("Linking program\n");
		ProgramID = glCreateProgram();
		mShaderLoaded = true;
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (Result == GL_FALSE && InfoLogLength > 0) {

			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			throw MythosError("Shader Link Error", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		
		/*
		GLuint vertShader = __attachShaderRaw(mShader, GL_VERTEX_SHADER, vertShaderSource);
		GLuint fragShader = __attachShaderRaw(mShader, GL_FRAGMENT_SHADER, fragShaderSource);

		glLinkProgram(mShader);

		GLint programLinkStatus = GL_FALSE;
		glGetProgramiv(mShader, GL_LINK_STATUS, &programLinkStatus);

		if (programLinkStatus != GL_TRUE) {

			int programInfoLogLen;
			glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &programInfoLogLen);

			if (programInfoLogLen > 0) {

				char* programInfoLog = new char[programInfoLogLen + 1];
				glGetShaderInfoLog(mShader, programInfoLogLen + 1, NULL, programInfoLog);
				mythosDisplayError("Shader Link Error", programInfoLog);
			}
		}

		glDetachShader(mShader, vertShader);
		glDetachShader(mShader, fragShader);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		*/
	}
	catch (MythosError e) {

		e.display();

		if (mShaderLoaded) {

			glDeleteProgram(ProgramID);
			mShaderLoaded = false;
		}
		return;
	}

	mShader = ProgramID;
	mShaderLoaded = true;
}

GLuint MythosShader::getAttribLocation(const char* attribName) {

	return glGetAttribLocation(mShader, attribName);
}

GLuint MythosShader::getUniformLocation(const char* uniformName) {

	return glGetUniformLocation(mShader, uniformName);
}

void MythosShader::use() {

	glUseProgram(mShader);
}



MythosShader __mythosSolidShader;
MythosShader __mythosImageShader;

void __mythosShaderInit() {

	__mythosSolidShader.loadRaw(__mythosGetSolidShaderVertexSource(), __mythosGetSolidShaderFragmentSource());

	__mythosImageShader.loadRaw(__mythosGetImageShaderVertexSource(), __mythosGetImageShaderFragmentSource());
}

MYTHOS_CORE_API MythosShader* mythosGetSolidShader() {

	return &__mythosSolidShader;
}

MYTHOS_CORE_API MythosShader* mythosGetImageShader() {

	return &__mythosImageShader;
}