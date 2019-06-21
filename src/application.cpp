#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//struct for returning two strings in ParseShader
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

//Parses shader from shader file to a struct of a Vertex and Fragment shader
static ShaderProgramSource ParseShader(const std::string& filepath)
{
	//creates a new file stream, allows for us to read files
	std::fstream stream(filepath);

	//creates an enum for setting the mode of shader we are currently writing
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line; //variable for getting the next line of the file
	std::stringstream ss[2]; //stringstream for concatinating the lines, array since we are making two different shaders
	ShaderType type = ShaderType::NONE; //creating an instance of the enum to be able to set the type
	while (getline(stream, line)) //runs for every line in the file, sets line to the current line in the stream
	{
		if (line.find("#shader") != std::string::npos) //check if the line contains a definition for a shader
		{
			if (line.find("vertex") != std::string::npos) //if so, does it contain a definition for a vertex
				type = ShaderType::VERTEX; //change which stream to write into
			else if (line.find("fragment") != std::string::npos) //if not, does it contain one for a fragment
				type = ShaderType::FRAGMENT; //change which stream to write into
		}
		else //if it is not a line definition then it should be added to the current stream
		{
			ss[(int)type] << line << '\n'; //adds the current line to the stream followed by a new line character as we want every line to be separated in the string
		}
	}
	return {ss[0].str(), ss[1].str()}; //returns the struct of the VertexSource and FragmentSource
}

static unsigned int CompileShader(unsigned int type, const std::string& source) //function for compiling shaders
{
	unsigned int id = glCreateShader(type); //create a shader and get an ID used to access said shader
	const char* src = source.c_str(); //create a pointer to the code that should be compiled
	glShaderSource(id, 1, &src, nullptr);  //puts the source code in the shader object (ID to the shader we created, how many length and strings we have, pointer to pointer to code, pointer to array or string lengths)
	glCompileShader(id); //compiles said shader after code has been attached

	int result; //create result integer to see if compilation was successfull
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //gets data from shader (ID to created shader, what we want to access in this case the status, pointer to variable to store the result in)
	if (result == GL_FALSE) //returns true if compilation failed
	{
		int length; //creates length variable needed for message
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //gets the length of the error message
		char* message = (char*)alloca(length* sizeof(char)); //allocates memory for a char array of the same length as specified
		glGetShaderInfoLog(id, length, &length, message); //puts the message into the message variable
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl; //prints info on what failed
		std::cout << message << std::endl; //prints error message
		glDeleteShader(id); //deletes the failed shader
		return 0; //return 0 to indicate error (cannot return -1 as the function is an unsigned int)
	}
	return id; //return the ID to the created shader which then will be able to be used to access it
}

//creates a program to run on the GPU including a vertex and a fragment shader
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //creates a new program and assigns an ID to the program to the variable program
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //uses the compile shader function to get the ID of a vertex shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); //uses the compile shader function to get the ID of a fragment shader

	glAttachShader(program, vs); //attaches the vertex shader to the created program
	glAttachShader(program, fs); //attaches the fragment shader to the created program
	glLinkProgram(program); //links the files together (just like linking binaries in C++)
	glValidateProgram(program); //checks if the program can be run properly, result will be stored in info log 
	
    //deletes the shaders as the program is now created and they are no longer needed
	glDeleteShader(vs); 
	glDeleteShader(fs);

	return program; //return the ID to the newly created program
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) //initialize glew
	{
		std::cout << "could not init GLEW" << std::endl;
		std::getchar();
	}

	std::cout << glGetString(GL_VERSION) << std::endl; //print openGL version
	
	float positions[] = { //create a array of positions that can be used to render objects
		-0.5f, -0.5f, //0
		 0.5f, -0.5f, //1
		 0.5f,  0.5f, //2 
		-0.5f,  0.5f  //3
	};

	unsigned int indencies[] = { //create groups of these positions that can be used to represent two triangles, these can then represent a square
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer; //create a variable to get buffer ID
	glGenBuffers(1, &buffer); //create one buffer and assign the ID of the buffer to the variable "buffer"
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //bind the buffer to openGL
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW); //explains the content and size of the buffer data
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //deinfes the array of data
	glEnableVertexAttribArray(0); //enables the array

	unsigned int ibo; //creates a variable to get the indecies ID's
	glGenBuffers(1, &ibo); //gets a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //binds teh buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indencies, GL_STATIC_DRAW); //adds data to buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //deinfes the array of data
	glEnableVertexAttribArray(0); //enables the array

	ShaderProgramSource source = ParseShader("../resources/shaders/Basic.shader"); //creates a variable from the shader code

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); //calls the CreateShader function with the code from the source variable
	glUseProgram(shader); //sets the created program as active

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //clears the screen

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //draws triangles with a total of 6 vertecies, with indencies of the type unsigned int and with no offset
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader); //delete the now unecessary shader program

	glfwTerminate(); //terminate glfw, duh
	return 0;
}
