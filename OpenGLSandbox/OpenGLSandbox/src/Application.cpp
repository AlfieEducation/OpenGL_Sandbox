#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "OpenGL Initial", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f, //0
			 0.5f, -0.5f, //1
			 0.5f,  0.5f, //2
			-0.5f,  0.5f, //3
		};

		// Indices elements must be unsigned
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.addBuffer(vb, layout);

		IndexBuffer ib(indices, 6);
		
		Shader shader = Shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.8f, 0.3f, 1.0f);


		va.UnBind();
		shader.Unbind();
		vb.UnBind();
		ib.UnBind();
	
		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.8f, 0.3f, 1.0f);

			va.Bind();
			ib.Bind();

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/*  Render primitives from array data
			first parameter - Specifies what kind of primitives to render.
			second parameter - Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
			third parameter - Specifies an offset of the first index in the array in the data store of the buffer currently bound to the GL_ELEMENT_ARRAY_BUFFER target.*/
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //6 - number of indices. Must be UNSIGNED INT

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}