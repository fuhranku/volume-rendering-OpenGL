#include "main.h"

// Shader object
Shader *shader;
// Index (GPU) of the geometry buffer
unsigned int VBO;
// Index (GPU) vertex array object
unsigned int VAO;
// Index (GPU) of the texture
unsigned int textureID;
unsigned int volTexID;


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

	// Initialize the window, and the glad components
	if (!initWindow() || !initGlad())
		return false;
	// Initialize the opengl context
	initGL();
	// Loads the shader
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	// Loads all the geometry into the GPU
	buildGeometry();
	// Loads the texture into the GPU
	textureID = loadTexture("assets/textures/bricks2.jpg");
	// Loads 3D texture into GPU
	volTexID = load3DTexture("assets/volumes/lobster.raw");

	return true;
}

void buildGeometry(){

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
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, XDIM, YDIM, ZDIM, 0, GL_RGBA, GL_UNSIGNED_BYTE, pVolume);
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
    }
}

void render(){

    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /** Draws code goes here **/
    // Use the shader
    shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, volTexID);
    // Binds the vertex array to be drawn
    glBindVertexArray(VAO);
    // Renders the triangle gemotry
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    // Swap the buffer
    glfwSwapBuffers(window);
}

void update(){

    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window)){
        // Checks for keyboard inputs
        processKeyboardInput(window);

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
