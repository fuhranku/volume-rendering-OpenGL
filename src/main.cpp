#include "main.h"

void resize(GLFWwindow *window, int width, int height){
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
}

bool initWindow(){
    // Initialize glfw
    glfwInit();
    // Sets the Opegl context to Opengl 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates the window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

    // The window couldn't be created
    if (!window){
        std::cout << "Failed to create the glfw window" << std::endl;
        glfwTerminate(); // Stops the glfw program
        return false;
    }

    // Creates the glfwContext, this has to be made before calling initGlad()
    glfwMakeContextCurrent(window);

    // Window resize callback
    glfwSetFramebufferSizeCallback(window, resize);
    return true;
}

bool initGlad(){

    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status){
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

void initGL(){
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

bool init() {

	// Initialize the window, glad components and framebuffer
	if (!initWindow() || !initGlad() || !setFrameBuffer())
		return false;
	// Initialize the opengl context
	initGL();
	// Loads the shader
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	cubeShader = new Shader("assets/shaders/cubeShader.vert", "assets/shaders/cubeShader.frag");
	frameBufferDebug = new Shader("assets/shaders/frameBufferDebug.vert", "assets/shaders/frameBufferDebug.frag");
	// Loads all the geometry into the GPU
	buildGeometry();
	// Loads the texture into the GPU
	textureID = loadTexture("assets/textures/bricks2.jpg");
	// Loads 3D texture into GPU
	volTexID = load3DTexture("assets/volumes/bonsai_256x256x256_uint8.raw");

	return true;
}

void buildGeometry(){
	// Quad for debug purposes:
	float quadVertices[] = {
		// positions        // Color   		   // texture Coords
		-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f, 0.0f,
	};
	// Setup plane VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// ---------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------------------

	// Cube where volume will be:
	float cubeVertices[] = {
    -0.5f,-0.5f,-0.5f, 
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f, 
	 0.5f, 0.5f,-0.5f, 
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f, 
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f
	};
	// Setup plane VAO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


}

unsigned int load3DTexture(const char* path) {
	unsigned int id;
	const int size = XDIM * YDIM * ZDIM;
	FILE *pFile;
	GLubyte* pVolume = new GLubyte[size];
	errno_t err;
	if ((err = fopen_s(&pFile, path, "rb")) != 0) {
		std::cout << "ERROR:: Unable to load texture " << path << std::endl;
		glDeleteTextures(1, &id);
	}else{
		fread(pVolume, sizeof(GLubyte), size, pFile);
		fclose(pFile);

		// Load data into a 3D texture
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_3D, id);

		// Set the texture parameters
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, XDIM, YDIM, ZDIM, 0, GL_RED, GL_UNSIGNED_BYTE, pVolume);
		glGenerateMipmap(GL_TEXTURE_3D);
		delete[] pVolume;
	}

	return id;
}

unsigned int loadTexture(const char *path){
    unsigned int id;
    // Creates the texture on GPU
    glGenTextures(1, &id);
    // Loads the texture
    int textureWidth, textureHeight, numberOfChannels;
    // Flips the texture when loads it because in opengl the texture coordinates are flipped
    stbi_set_flip_vertically_on_load(true);
    // Loads the texture file data
    unsigned char *data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
    if (data){
        // Gets the texture channel format
        GLenum format;
        switch (numberOfChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        // Binds the texture
        glBindTexture(GL_TEXTURE_2D, id);
        // Creates the texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        // Creates the texture mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set the filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "ERROR:: Unable to load texture " << path << std::endl;
        glDeleteTextures(1, &id);
    }
    // We dont need the data texture anymore because is loaded on the GPU
    stbi_image_free(data);

    return id;
}

void processMouseMovement(GLFWwindow* window) {
	double xpos, ypos;
	if (cameraMode) {
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec2 mousePosition(xpos, ypos);
		camera.mouseUpdate(mousePosition);
	}
}

void processKeyboardInput(GLFWwindow *window){
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        // Reloads the shader
        delete shader;
        shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		delete cubeShader;
		cubeShader = new Shader("assets/shaders/cubeShader.vert", "assets/shaders/cubeShader.frag");
		delete frameBufferDebug;
		frameBufferDebug = new Shader("assets/shaders/frameBufferDebug.vert", "assets/shaders/frameBufferDebug.frag");
    }

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && step < 1.0f){
		step += 1.0f / 256;
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && step > 0.0f) {
		step -= 1.0f / 256;
	}

	// Camera's movement input
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cameraMode)
		cameraMode = true;
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && cameraMode)
		cameraMode = false;

	if (cameraMode) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.moveForward(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.moveLeft(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.moveRight(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.moveBackward(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.moveUp(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.moveDown(deltaTime);
	}


}

void drawTransferFunction() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight*0.15);
	// Use the shader
	shader->use();
	shader->setFloat("step", step);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, volTexID);
	//Draw QUAD for debug purposes
	//Binds the vertex array to be drawn
	glBindVertexArray(VAO);
	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

bool setFrameBuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL (Which is done with last "0")
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Depth buffer
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	// Frame buffer configuration
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture,0);

	// Set list of draw buffers
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::Framebuffer configuration went wrong" << std::endl;
		return false;
	}
	return true;
}

void getVolumesBackface() {

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// Set full-screen viewport
	glViewport(0, 0, windowWidth, windowHeight);

	// View matrix
	View = camera.getWorldToViewMatrix();
	// Projection matrix
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// World transformation
	ModelMatrix = glm::mat4(1.0f);
	// Shader uniform parameters
	cubeShader->use();
	cubeShader->setMat4("model", ModelMatrix);
	cubeShader->setMat4("view", View);
	cubeShader->setMat4("projection", Projection);
	//Binds the vertex array to be drawn
	glBindVertexArray(cubeVAO);
	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glCullFace(GL_BACK);

	/* For DEBUG purposes only */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Use the shader
	frameBufferDebug->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	//Draw QUAD for debug purposes
	//Binds the vertex array to be drawn
	glBindVertexArray(VAO);
	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

}

void drawVolume(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, windowHeight*0.15, windowWidth, windowHeight*0.85);
	// View matrix
	View = camera.getWorldToViewMatrix();
	// Projection matrix
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// World transformation
	ModelMatrix = glm::mat4(1.0f);
	// Shader uniform parameters
	cubeShader->use();
	cubeShader->setMat4("model", ModelMatrix);
	cubeShader->setMat4("view", View);
	cubeShader->setMat4("projection", Projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, volTexID);
	//Binds the vertex array to be drawn
	glBindVertexArray(cubeVAO);
	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void render(){

    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Drawing transfer function
	//drawTransferFunction();

	// Draw front-faced culling volume
	getVolumesBackface();

	// Drawing volume
	//drawVolume();

	// Set default viewport size
	glViewport(0, 0, windowWidth, windowHeight);

	// Swap the buffer
    glfwSwapBuffers(window);
}

void update(){

    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window)){
        // Checks for keyboard inputs
        processKeyboardInput(window);
		// Checks for mouse position
		processMouseMovement(window);
        // Renders everything
        render();

        // Check and call events
        glfwPollEvents();

    }
}

int main(int argc, char const *argv[]){

    // Initialize all the app components
    if (!init()){
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;
    // Starts the app main loop
    update();

    // Deletes the texture from the gpu
    glDeleteTextures(1, &textureID);
    // Deletes the vertex array from the GPU
    glDeleteVertexArrays(1, &VAO);
    // Deletes the vertex object from the GPU
    glDeleteBuffers(1, &VBO);
    // Destroy the shader
    delete shader;

    // Stops the glfw program
    glfwTerminate();

    return 0;
}
