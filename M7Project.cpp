#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // Image loading Utility functions

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnOpenGL/camera.h> // Camera class

using namespace std; // Standard namespace
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Module 6 Milestone"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbo;         // Handles for the vertex buffer objects
        GLuint vbos[2];
        GLuint nVertices;   // Number of vertices of the mesh
        GLuint nIndices;
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    GLMesh planeMesh;
    GLMesh hammerHeadMesh;
    GLMesh hammerHandleMesh;
    GLMesh rulerMesh;
    GLMesh tapeRollMesh;
    GLMesh screwdriverHeadMesh;
    GLMesh screwdriverShaftMesh;
    GLMesh screwdriverHandleMesh;
    // Texture ids
    GLuint gTextureId;
    GLuint gTextureIdLWood;
    GLuint gTextureIdMWood;
    GLuint gTextureIdWRubber;
    GLuint gTextureIdMetal;
    GLuint gTextureIdTapeLines;
    GLuint gTextureIdPlastic;
    GLuint gTextureIdRed;
    glm::vec2 gUVScale(0.9f, 0.9f);
    GLint gTexWrapMode = GL_REPEAT;

    // Shader program
    GLuint gObjectProgramId;
    GLuint gKeyLightProgramId;
    GLuint gFillLightProgramId;

    // camera
    Camera gCamera(glm::vec3(0.0f, 4.0f, 7.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // camera
    glm::vec3 gCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 gCameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 gCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // To track the current projection mode
    bool gIsOrthographic = false;

    // To track the current rendering mode
    bool gIsWireMode = false;

    // Plane Position and Scale
    glm::vec3 gPlanePos(0.0f, -0.5f, 0.0f);
    glm::vec3 gPlaneScale(6.0f, 0.0f, 4.0f);

    // Hammer Head Position and Scale
    glm::vec3 gHammerHeadPos(-2.0f, 0.0f, -1.0f);
    float gHammerHeadRotAngle(glm::radians(-20.0f));
    glm::vec3 gHammerHeadRotAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 gHammerHeadScale(0.5f, 0.5f, 1.0f);

    // Hammer Handle Position, Rotations, and Scale
    glm::vec3 gHammerHandlePos(-2.0f, 0.0f, -1.825f);
    float gHammerHandleRotAngle1(glm::radians(-110.0f));
    glm::vec3 gHammerHandleRotAxis1(0.0f, 1.0f, 0.0f);
    float gHammerHandleRotAngle2(glm::radians(5.0f));
    glm::vec3 gHammerHandleRotAxis2(1.0f, 0.0f, 0.0f);
    glm::vec3 gHammerHandleScale(0.2f, 0.2f, 0.8f);

    // Screwdriver Handle Position, Rotations, and Scale
    glm::vec3 gScrewdriverHandlePos(-0.12f, -0.37f, 1.2f);
    float gScrewdriverHandleRotAngle(glm::radians(-80.0f));
    glm::vec3 gScrewdriverHandleRotAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 gScrewdriverHandleScale(0.15f, 0.15f, 0.8f);

    // Screwdriver Shaft Position, Rotations and Scale
    glm::vec3 gScrewdriverShaftPos(-1.12f, -0.37f, 1.38f);
    float gScrewdriverShaftRotAngle(glm::radians(-80.0f));
    glm::vec3 gScrewdriverShaftRotAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 gScrewdriverShaftScale(0.05f, 0.05f, 1.5f);

    // Screwdriver Head Position, Rotations, and Scale
    glm::vec3 gScrewdriverHeadPos(-1.356f, -0.37f, 1.4215);
    float gScrewdriverHeadRotAngle(glm::radians(100.0f));
    glm::vec3 gScrewdriverHeadRotAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 gScrewdriverHeadScale(0.125f, 0.1f, 0.24f);

    // Ruler Position, Rotations, and Scale
    glm::vec3 gRulerPos(-4.0f, -0.5f, 2.5f);
    float gRulerRotAngle(glm::radians(50.0f));
    glm::vec3 gRulerRotAxis(0.0f, 1.0f, 0.0f);

    // Tape Roll Position, Rotations, and Scale
    glm::vec3 gTapeRollPos(2.0f, 0.1f, 0.0f);
    float gTapeRollRotAngle(glm::radians(-90.0f));
    glm::vec3 gTapeRollRotAxis(1.0f, 0.0f, 0.0f);
    glm::vec3 gTapeRollScale(1.0f, 1.0f, 0.6f);

    // Subject and Light colors
    glm::vec3 gObjectColor(1.0f, 1.0f, 1.0f);      // White
    glm::vec3 gKeyLightColor(1.0f, 1.0f, 0.9f);    // Yellow (slight) off-white
    glm::vec3 gFillLightColor(1.0f, 1.0f, 0.8f);   // Yellow (slight) off-white

    // Light positions and scale
    glm::vec3 gKeyLight1Pos(-2.0f, 4.0f, 2.0f);     // Above the scene
    glm::vec3 gFillLight1Pos(4.0f, 1.5f, 1.5f);   // Around and slightly above the scene
    glm::vec3 gFillLight2Pos(0.0f, 1.5f, -4.0f);
    glm::vec3 gLightScale(0.2f);
}


/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void UCreateCylinderMesh(GLMesh& mesh, float height, int slices);
void UCreateTorusMesh(GLMesh& mesh, float height, int slices);
void UCreatePlane(GLMesh& mesh);
void UCreateRuler(GLMesh& mesh);
void UCreateScrewdriverHeadMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender();
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);


/* Cylinder Vertex Shader Source Code*/
const GLchar* cylinderVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Cylinder Fragment Shader Source Code*/
const GLchar* cylinderFragmentShaderSource = GLSL(440,

    in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cylinder color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColorKey;
uniform vec3 lightColorFill;
uniform vec3 lightPosKey1;
uniform vec3 lightPosFill1;
uniform vec3 lightPosFill2;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
uniform vec2 uvScale;

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate Ambient lighting*/
    float ambientStrength = 0.2f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * (lightColorKey + lightColorFill * 0.25) / 1.25; // Generate ambient light color

    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    // Key Light 1
    // -----------
    //Calculate Diffuse lighting*/
    vec3 lightDirection = normalize(lightPosKey1 - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cylinder
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColorKey; // Generate diffuse light color

    //Calculate Specular lighting*/
    float specularIntensity = 0.4f; // Set specular light strength to 20%
    float highlightSize = 24.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColorKey;

    // Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;


    // Fill Light 1
    // ------------
    //Calculate Diffuse lighting*/
    lightDirection = normalize(lightPosFill1 - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cylinder
    impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    diffuse = impact * lightColorFill; // Generate diffuse light color

    //Calculate Specular lighting*/
    specularIntensity = 1.0f; // Set specular light strength to 5%
    highlightSize = 12.0f; // Set specular highlight size (half)
    reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    //Calculate specular component
    specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    specular = specularIntensity * specularComponent * lightColorFill;

    // Calculate phong result
    phong += (ambient + diffuse + specular) * textureColor.xyz/4;

    // Fill Light 2
    // ------------
    //Calculate Diffuse lighting*/
    lightDirection = normalize(lightPosFill2 - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cylinder
    impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    diffuse = impact * lightColorFill; // Generate diffuse light color

    //Calculate Specular lighting*/
    reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    //Calculate specular component
    specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    specular = specularIntensity * specularComponent * lightColorFill;

    // Calculate phong result
    phong += (ambient + diffuse + specular) * textureColor.xyz/5;


    fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
    //fragmentColor = texture(uTexture, vertexTextureCoordinate * uvScale); // Sends texture to the GPU for rendering
}
);

/* Key Light vertex Shader Source Code*/
const GLchar* lightVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);


/* Key Light Fragment Shader Source Code*/
const GLchar* lightFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color to the GPU

void main()
{
    fragmentColor = vec4(1.0f, 1.0, 1.0f, 1.0f); // Set color to black (0.0f,0.0f,0.0f) with alpha 1.0
}
);

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the Plane mesh
    UCreatePlane(planeMesh); // Calls the function to create the Vertex Buffer Object

    // Create the Ruler mesh
    UCreateRuler(rulerMesh);

    // Create the Hammer Handle Cylinder mesh
    UCreateCylinderMesh(hammerHandleMesh, 5.0f, 40);

    // Create the Hammer Head Cylinder mesh
    UCreateCylinderMesh(hammerHeadMesh, 1.5f, 48);

    // Create the Screwdriver Handle Cylinder mesh
    UCreateCylinderMesh(screwdriverHandleMesh, 1.5f, 6);

    // Create the Screwdriver Shaft Cylinder mesh
    UCreateCylinderMesh(screwdriverShaftMesh, 1.0f, 4);

    // Create the Screwdriver Head Cylinder mesh
    UCreateScrewdriverHeadMesh(screwdriverHeadMesh);

    // Create the Tape Roll Torus mesh
    UCreateTorusMesh(tapeRollMesh, 1.0f, 64);

    // Create the shader programs
    if (!UCreateShaderProgram(cylinderVertexShaderSource, cylinderFragmentShaderSource, gObjectProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(lightVertexShaderSource, lightFragmentShaderSource, gKeyLightProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(lightVertexShaderSource, lightFragmentShaderSource, gFillLightProgramId))
        return EXIT_FAILURE;

    // Load textures (relative to project's directory)
    const char* texFilename = "../resources/textures/lightWood.jpg";
    if (!UCreateTexture(texFilename, gTextureIdLWood))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    texFilename = "../resources/textures/mediumWood.jpg";
    if (!UCreateTexture(texFilename, gTextureIdMWood))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    texFilename = "../resources/textures/whiteRubber.jpg";
    if (!UCreateTexture(texFilename, gTextureIdWRubber))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    texFilename = "../resources/textures/metal.jpg";
    if (!UCreateTexture(texFilename, gTextureIdMetal))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    texFilename = "../resources/textures/tapeLines.jpg";
    if (!UCreateTexture(texFilename, gTextureIdTapeLines))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gObjectProgramId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gObjectProgramId, "uTexture"), 0);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(planeMesh);
    UDestroyMesh(hammerHeadMesh);
    UDestroyMesh(hammerHandleMesh);
    UDestroyMesh(rulerMesh);
    UDestroyMesh(screwdriverHandleMesh);
    UDestroyMesh(screwdriverShaftMesh);
    UDestroyMesh(screwdriverHeadMesh);

    // Release texture
    UDestroyTexture(gTextureIdLWood);
    UDestroyTexture(gTextureIdMWood);
    UDestroyTexture(gTextureIdWRubber);
    UDestroyTexture(gTextureIdMetal);
    UDestroyTexture(gTextureIdTapeLines);

    // Release shader program
    UDestroyShaderProgram(gObjectProgramId);
    UDestroyShaderProgram(gKeyLightProgramId);
    UDestroyShaderProgram(gFillLightProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetKeyCallback(*window, UKeyCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.Position += gCamera.Up * gCamera.MovementSpeed * gDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.Position -= gCamera.Up * gCamera.MovementSpeed * gDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//glfw: for processing single keypress events
void UKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) return; //only handle press events
    if (key == GLFW_KEY_P) gIsOrthographic = !gIsOrthographic;
    if (key == GLFW_KEY_X) gIsWireMode = !gIsWireMode;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float gCameraSpeedFactor = 0.1f;

    // Adjust the camera speed based on the scroll offset
    float speedMultiplier = 1.0f + static_cast<float>(yoffset) * gCameraSpeedFactor;

    gCamera.MovementSpeed *= speedMultiplier;
}

// Function called to render a frame
void URender()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gIsWireMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    // PLANE: draw plane
    // //---------------
    // Set the shader to be used
    // Activate the cube VAO (used by plane)
    glBindVertexArray(planeMesh.vao);
    glUseProgram(gObjectProgramId);

    // Apply transformations to the model matrix for planeMesh
    glm::mat4 modelPlaneMesh = glm::translate(gPlanePos);  // Identity matrix for planeMesh

    // Set up view matrix
    glm::mat4 view = gCamera.GetViewMatrix();

    // Toggle between perspective and orthographic projections
    glm::mat4 projection;
    if (gIsOrthographic) {
        float orthoSize = 5.0f;
        projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gObjectProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gObjectProgramId, "view");
    GLint projLoc = glGetUniformLocation(gObjectProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlaneMesh));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Cylinder Shader program for the cylinder color, light color, light position, and camera position
    GLint objectColorLoc = glGetUniformLocation(gObjectProgramId, "objectColor");
    GLint lightColorKeyLoc = glGetUniformLocation(gObjectProgramId, "lightColorKey");
    GLint lightPosKey1Loc = glGetUniformLocation(gObjectProgramId, "lightPosKey1");
    GLint lightColorFillLoc = glGetUniformLocation(gObjectProgramId, "lightColorFill");
    GLint lightPosFill1Loc = glGetUniformLocation(gObjectProgramId, "lightPosFill1");
    GLint lightPosFill2Loc = glGetUniformLocation(gObjectProgramId, "lightPosFill2");
    GLint viewPositionLoc = glGetUniformLocation(gObjectProgramId, "viewPosition");

    // Pass color, light, and camera data to the Cylinder Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorKeyLoc, gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);
    glUniform3f(lightPosKey1Loc, gKeyLight1Pos.x, gKeyLight1Pos.y, gKeyLight1Pos.z);
    glUniform3f(lightColorFillLoc, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(lightPosFill1Loc, gFillLight1Pos.x, gFillLight1Pos.y, gFillLight1Pos.z);
    glUniform3f(lightPosFill2Loc, gFillLight2Pos.x, gFillLight2Pos.y, gFillLight2Pos.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(gObjectProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdMWood);

    // Draw the first mesh (plane)
    glDrawArrays(GL_TRIANGLES, 0, planeMesh.nVertices);


    // CYLINDER: draw hammer head
    //---------------------------
    // Render the second mesh (cylinderMesh)
    glBindVertexArray(hammerHeadMesh.vao);

    // Apply transformations to the model matrix for cylinderMesh
    glm::mat4 modelHammerHeadMesh = glm::translate(gHammerHeadPos) 
        * glm::rotate(gHammerHeadRotAngle, gHammerHeadRotAxis)
        * glm::scale(gHammerHeadScale);

    // Reference matrix uniform from the shader program
    GLint modelLocHammerHeadMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocHammerHeadMesh, 1, GL_FALSE, glm::value_ptr(modelHammerHeadMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdWRubber);

    // Draw the cylinder mesh
    glDrawElements(GL_TRIANGLES, hammerHeadMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);


    // CYLINDER: draw hammer handle
    //-----------------------------
    // Render the second instance of cylinderMesh
    glBindVertexArray(hammerHandleMesh.vao);

    glm::mat4 modelHammerHandleMesh = glm::translate(gHammerHandlePos)
        * glm::rotate(gHammerHandleRotAngle1, gHammerHandleRotAxis1)
        * glm::rotate(gHammerHandleRotAngle2, gHammerHandleRotAxis2)
        * glm::scale(gHammerHandleScale);

    // Reference matrix uniform from the shader program
    GLint modelLocHammerHandleMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocHammerHandleMesh, 1, GL_FALSE, glm::value_ptr(modelHammerHandleMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdLWood);

    // Draw the cylinder mesh
    glDrawElements(GL_TRIANGLES, hammerHandleMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);


    // CYLINDER: draw screwdriver handle
    //----------------------------------
    // Render the screwdriver handle's mesh
    glBindVertexArray(screwdriverHandleMesh.vao);

    glm::mat4 modelScrewdriverHandleMesh = glm::translate(gScrewdriverHandlePos)
        * glm::rotate(gScrewdriverHandleRotAngle, gScrewdriverHandleRotAxis)
        * glm::scale(gScrewdriverHandleScale);

    // Reference matrix uniform from the shader program
    GLint modelLocScrewdriverHandleMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocScrewdriverHandleMesh, 1, GL_FALSE, glm::value_ptr(modelScrewdriverHandleMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdWRubber);

    // Draw the cylinder mesh
    glDrawElements(GL_TRIANGLES, screwdriverHandleMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);


    // CYLINDER: draw screwdriver shaft
    //---------------------------------
    // Render the screwdriver handle's mesh
    glBindVertexArray(screwdriverShaftMesh.vao);

    glm::mat4 modelScrewdriverShaftMesh = glm::translate(gScrewdriverShaftPos)
        * glm::rotate(gScrewdriverShaftRotAngle, gScrewdriverShaftRotAxis)
        * glm::scale(gScrewdriverShaftScale);

    // Reference matrix uniform from the shader program
    GLint modelLocScrewdriverShaftMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocScrewdriverShaftMesh, 1, GL_FALSE, glm::value_ptr(modelScrewdriverShaftMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdMetal);

    // Draw the cylinder mesh
    glDrawElements(GL_TRIANGLES, screwdriverShaftMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);


    // CYLINDER: draw screwdriver head
    //--------------------------------
    // Render the screwdriver handle's mesh
    glBindVertexArray(screwdriverHeadMesh.vao);

    glm::mat4 modelScrewdriverHeadMesh = glm::translate(gScrewdriverHeadPos)
        * glm::rotate(gScrewdriverHeadRotAngle, gScrewdriverHeadRotAxis)
        * glm::scale(gScrewdriverHeadScale);

    // Reference matrix uniform from the shader program
    GLint modelLocScrewdriverHeadMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocScrewdriverHeadMesh, 1, GL_FALSE, glm::value_ptr(modelScrewdriverHeadMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdMetal);

    // Draw the ruler
    glDrawArrays(GL_TRIANGLES, 0, screwdriverHeadMesh.nVertices);


    // RULER: draw ruler
    //------------------
    // Render the ruler's mesh
    glBindVertexArray(rulerMesh.vao);

    // Apply transformations to the model matrix for the ruler's mesh
    glm::mat4 modelRulerMesh = glm::translate(gRulerPos)
        * glm::rotate(gRulerRotAngle, gRulerRotAxis);

    // Reference matrix uniform from the shader program
    GLint modelLocRulerMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocRulerMesh, 1, GL_FALSE, glm::value_ptr(modelRulerMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdMetal);

    // Draw the ruler
    glDrawArrays(GL_TRIANGLES, 0, rulerMesh.nVertices);


    // TORUS: draw tape roll
    //----------------------
    // Render the torus mesh
    glBindVertexArray(tapeRollMesh.vao);

    // Apply transformations to the model matrix for cylinderMesh
    glm::mat4 modelTapeRollMesh = glm::translate(gTapeRollPos)
        * glm::rotate(gTapeRollRotAngle, gTapeRollRotAxis)
        * glm::scale(gTapeRollScale);

    // Reference matrix uniform from the shader program
    GLint modelLocTapeRollMesh = glGetUniformLocation(gObjectProgramId, "model");
    // Pass matrix data to the shader program's matrix uniform
    glUniformMatrix4fv(modelLocTapeRollMesh, 1, GL_FALSE, glm::value_ptr(modelTapeRollMesh));

    // bind textures on corresponding texture units
    glBindTexture(GL_TEXTURE_2D, gTextureIdTapeLines);

    // Draw the cylinder mesh
    glDrawElements(GL_TRIANGLES, tapeRollMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);


    // Deactivate the Vertex Array Object and shader program
    glBindVertexArray(0);
    glUseProgram(0);

    // Swap buffers and poll IO events
    glfwSwapBuffers(gWindow);   // Flips the the back buffer with the front buffer every frame.
}


void UCreateCylinderMesh(GLMesh& mesh, float height, int slices)
{
    const int numSlices = slices;
    float sliceDegrees = 360.0 / numSlices; // for rotation of triangles on cylinder
    glm::f32 sliceDegRad = glm::radians(sliceDegrees);
    float texCoordInc = 1.0f / numSlices;
    float texCoord = 0.0f;

    // Define vertex data for entire cylinder
    vector<GLfloat> cylinderVertices;
    vector<GLushort> cylinderIndices;

    // builds array for slice rotations
    for (int i = 0; i < numSlices; i++)
    {
        float incDeg = sliceDegrees * i;
        float x = cos(glm::radians(incDeg));
        float y = sin(glm::radians(incDeg));
        float incx = cos(glm::radians(incDeg + sliceDegrees));
        float incy = sin(glm::radians(incDeg + sliceDegrees));

        // Vectors / Math for normals
        glm::vec3 v0(x, y, 0.0f);
        glm::vec3 v1(incx, incy, 0.0f);
        glm::vec3 v2(x, y, -height);
        glm::vec3 normal(cross((v2 - v0), (v1 - v0)));
        normal.z = (0.0f);

        // Vertices for the slice
        GLfloat sliceVertices[] = {
            // vert 0
            0.0f, 0.0f, 0.0f,   // position data
            0.0f, 0.0f, 1.0f,   // normals data
            0.5f, 1.0f,         // texture data

               x,    y, 0.0f,       // vert 1
            normal.x, normal.y, normal.z,
            0.86f, texCoord,

               x,    y, 0.0f,       // vert 2
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f,

            incx, incy, 0.0,        // vert 3
            normal.x, normal.y, normal.z,
            0.86f,texCoord + texCoordInc,

            incx, incy, 0.0,        // vert 4
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f,

               x,    y, -height,    // vert 5
            normal.x, normal.y, normal.z,
            0.0f,texCoord,

               x,    y, -height,    // vert 6
            0.0f, 0.0f, -1.0f,
            1.0f, 0.0f,

            incx, incy, -height,    // vert 7
            normal.x, normal.y, normal.z,
            0.0f,texCoord + texCoordInc,

            incx, incy, -height,    // vert 8
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f,

            0.0f, 0.0f, -height,    // vert 9
            0.0f, 0.0f, -1.0f,
            0.5f, 1.0f
        };

        // Indices for the current slice
        GLushort sliceIndices[] = {
            i * 10, i * 10 + 2, i * 10 + 4,     // Top face of slice     Triangle 1 (front face)
            i * 10 + 1, i * 10 + 3, i * 10 + 5, // Half of side          Triangle 2
            i * 10 + 3, i * 10 + 5, i * 10 + 7, // Other half of side    Triangle 3
            i * 10 + 6, i * 10 + 8, i * 10 + 9, // Bottom of slice       Triangle 4 (back face)
        };

        // Append the vertices and indices to the main arrays
        cylinderVertices.insert(cylinderVertices.end(), std::begin(sliceVertices), std::end(sliceVertices));
        cylinderIndices.insert(cylinderIndices.end(), std::begin(sliceIndices), std::end(sliceIndices));

        texCoord += texCoordInc;
    }

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(2, mesh.vbos);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(GLfloat), cylinderVertices.data(), GL_STATIC_DRAW);

    mesh.nIndices = cylinderIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(GLushort), cylinderIndices.data(), GL_STATIC_DRAW);

    GLint floatsPerVertex = 3;
    GLint floatsPerNormal = 3;
    GLint floatsPerUV = 2;
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void UCreatePlane(GLMesh& mesh) {
    // Position and Color data
    GLfloat verts[] = {
        // Vertex Positions (x, y, z)  // Normals           // Texture Coordinates

        // Half of plane
         6.0f, 0.0f,  4.0f,            0.0f, 1.0f, 0.0f,    1.0f, 1.0f, // Vertex 0
         6.0f, 0.0f, -4.0f,            0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Vertex 1
        -6.0f, 0.0f,  4.0f,            0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // Vertex 2

        // Other half of plane
         6.0f, 0.0f, -4.0f,            0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Vertex 3
        -6.0f, 0.0f,  4.0f,            0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // Vertex 4
        -6.0f, 0.0f, -4.0f,            0.0f, 1.0f, 0.0f,    0.0f, 0.0f  // Vertex 5
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create VBO
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}


void UCreateTorusMesh(GLMesh& mesh, float height, int slices) {
    const int numSlices = slices;
    const float thickness = 0.8f;
    float sliceDegrees = 360.0 / numSlices; // for rotation of triangles on cylinder
    glm::f32 sliceDegRad = glm::radians(sliceDegrees);
    float texCoordInc = 1.0f / numSlices;
    float texCoord = 0.0f;

    // Define vertex data for entire cylinder
    vector<GLfloat> cylinderVertices;
    vector<GLushort> cylinderIndices;

    // builds array for slice rotations
    for (int i = 0; i < numSlices; i++)
    {
        // Outside
        float incDeg = sliceDegrees * i;
        float xOut = cos(glm::radians(incDeg));
        float yOut = sin(glm::radians(incDeg));
        float incxOut = cos(glm::radians(incDeg + sliceDegrees));
        float incyOut = sin(glm::radians(incDeg + sliceDegrees));
        // Inside
        float xIn = xOut * thickness;
        float yIn = yOut * thickness;
        float incxIn = incxOut * thickness;
        float incyIn = incyOut * thickness;

        // Vectors / Math for normals
        glm::vec3 v0(xOut, yOut, 0.0f);
        glm::vec3 v1(incxOut, incyOut, 0.0f);
        glm::vec3 v2(xOut, yOut, -height);
        glm::vec3 normal(cross((v2 - v0), (v1 - v0)));

        // Vertices for the slice
        GLfloat sliceVertices[] = {
            // Inside
               xIn,    yIn, 0.0f,       // vert 0   // position data
            -normal.x, -normal.y, normal.z,         // normals data
            0.0f, texCoord,                         // texture data

            incxIn, incyIn, 0.0,        // vert 1
            -normal.x, -normal.y, normal.z,
            0.0f,texCoord + texCoordInc,

               xIn,    yIn, -height,    // vert 2
            -normal.x, -normal.y, normal.z,
            1.0f,texCoord,

            incxIn, incyIn, -height,    // vert 3
            -normal.x, -normal.y, normal.z,
            1.0f,texCoord + texCoordInc,


            // Top
            xIn, yIn, 0.0f,             // vert 4
            0.0f, 0.0f, 1.0f,
            0.0f, texCoord,

            incxIn, incyIn, 0.0f,       // vert 5
            0.0f, 0.0f, 1.0f,
            0.0f, texCoord + texCoordInc,

               xOut,    yOut, 0.0f,     // vert 6
            0.0f, 0.0f, 1.0f,
            1.0f, texCoord,

            incxOut, incyOut, 0.0f,     // vert 7
            0.0f, 0.0f, 1.0f,
            1.0f, texCoord + texCoordInc,


            // Outside
               xOut,    yOut, 0.0f,     // vert 8
            normal.x, normal.y, normal.z,
            0.0f, texCoord,

            incxOut, incyOut, 0.0,      // vert 9
            normal.x, normal.y, normal.z,
            0.0f,texCoord + texCoordInc,

               xOut,    yOut, -height,  // vert 10
            normal.x, normal.y, normal.z,
            1.0f,texCoord,

            incxOut, incyOut, -height,  // vert 11
            normal.x, normal.y, normal.z,
            1.0f,texCoord + texCoordInc,
        };

        // Indices for the current slice
        GLushort sliceIndices[] = {
            i * 12,     i * 12 + 1,  i * 12 + 2,   // Inside   Triangle 1
            i * 12 + 1, i * 12 + 2,  i * 12 + 3,   //          Triangle 2
            i * 12 + 4, i * 12 + 5,  i * 12 + 6,   // Top      Triangle 3
            i * 12 + 5, i * 12 + 6,  i * 12 + 7,   //          Triangle 4
            i * 12 + 8, i * 12 + 9,  i * 12 + 10,  // Outside  Triangle 3
            i * 12 + 9, i * 12 + 10, i * 12 + 11   //          Triangle 6
        };

        // Append the vertices and indices to the main arrays
        cylinderVertices.insert(cylinderVertices.end(), std::begin(sliceVertices), std::end(sliceVertices));
        cylinderIndices.insert(cylinderIndices.end(), std::begin(sliceIndices), std::end(sliceIndices));

        texCoord += texCoordInc;
    }

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(2, mesh.vbos);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(GLfloat), cylinderVertices.data(), GL_STATIC_DRAW);

    mesh.nIndices = cylinderIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(GLushort), cylinderIndices.data(), GL_STATIC_DRAW);

    GLint floatsPerVertex = 3;
    GLint floatsPerNormal = 3;
    GLint floatsPerUV = 2;
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void UCreateRuler(GLMesh& mesh) {
    GLfloat verts[] = {
        // Vertex Positions (x, y, z)  // Normals           // Texture Coordinates
        
        // Half of top face
         6.0f, 0.1f, 0.5f,            0.0f, 1.0f, 0.0f,    1.0f, 1.0f, // Vertex 0
         6.0f, 0.1f, 0.0f,            0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Vertex 1
         0.0f, 0.1f, 0.5f,            0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // Vertex 2

        // Other half of top face
         6.0f, 0.1f, 0.0f,            0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Vertex 3
         0.0f, 0.1f, 0.5f,            0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // Vertex 4
         0.0f, 0.1f, 0.0f,            0.0f, 1.0f, 0.0f,    0.0f, 0.0f, // Vertex 5
         
        // Half of side
         6.0f, 0.1f, 0.5f,            0.0f, 0.0f, -1.0f,    1.0f, 1.0f, // Vertex 6
         6.0f, 0.1f, 0.0f,            0.0f, 0.0f, -1.0f,    1.0f, 0.0f, // Vertex 7
         6.0f, 0.0f, 0.5f,            0.0f, 0.0f, -1.0f,    0.0f, 1.0f, // Vertex 8
        
        // Other half of side
         6.0f, 0.1f, 0.0f,            0.0f, 0.0f, -1.0f,    1.0f, 0.0f, // Vertex 9
         6.0f, 0.0f, 0.5f,            0.0f, 0.0f, -1.0f,    0.0f, 1.0f, // Vertex 10
         6.0f, 0.0f, 0.0f,            0.0f, 0.0f, -1.0f,    0.0f, 0.0f, // Vertex 11
         
        // Half of side
         0.0f, 0.1f, 0.5f,            0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Vertex 12
         0.0f, 0.1f, 0.0f,            0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Vertex 13
         0.0f, 0.0f, 0.5f,            0.0f, 0.0f, 1.0f,    0.0f, 1.0f, // Vertex 14

        // Other half of side
         0.0f, 0.1f, 0.0f,            0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Vertex 15
         0.0f, 0.0f, 0.5f,            0.0f, 0.0f, 1.0f,    0.0f, 1.0f, // Vertex 16
         0.0f, 0.0f, 0.0f,            0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // Vertex 17
         
         // Half of front
         6.0f, 0.1f, 0.5f,            0.0f, 0.0f, -1.0f,    1.0f, 1.0f, // Vertex 6
         6.0f, 0.0f, 0.5f,            0.0f, 0.0f, -1.0f,    1.0f, 0.0f, // Vertex 7
         0.0f, 0.1f, 0.5f,            0.0f, 0.0f, -1.0f,    0.0f, 1.0f, // Vertex 8

        // Other half of front
         6.0f, 0.0f, 0.5f,            0.0f, 0.0f, -1.0f,    1.0f, 0.0f, // Vertex 9
         0.0f, 0.1f, 0.5f,            0.0f, 0.0f, -1.0f,    0.0f, 1.0f, // Vertex 10
         0.0f, 0.0f, 0.5f,            0.0f, 0.0f, -1.0f,    0.0f, 0.0f, // Vertex 11

        // Half of back
         6.0f, 0.1f, 0.0f,            0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Vertex 12
         6.0f, 0.0f, 0.0f,            0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Vertex 13
         0.0f, 0.1f, 0.0f,            0.0f, 0.0f, 1.0f,    0.0f, 1.0f, // Vertex 14

        // Other half of back
         6.0f, 0.0f, 0.0f,            0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Vertex 15
         0.0f, 0.1f, 0.0f,            0.0f, 0.0f, 1.0f,    0.0f, 1.0f, // Vertex 16
         0.0f, 0.0f, 0.0f,            0.0f, 0.0f, 1.0f,    0.0f, 0.0f  // Vertex 17
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create VBO
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void UCreateScrewdriverHeadMesh(GLMesh& mesh) {
    // Vectors / Math for normals
    glm::vec3 v0(-0.25f, 0.0f, 0.0f);
    glm::vec3 v1(0.25f, 0.0f, 0.0f);
    glm::vec3 v2(-0.4f, 0.2f, 0.4f);
    glm::vec3 v3(-0.4f, -0.2f, 0.4f);
    glm::vec3 normalTop(cross((v2 - v0), (v1 - v0)));
    glm::vec3 normalSide(cross((v3 - v0), (v2 - v0)));

    GLfloat verts[] = {
        // Vertex Positions     // Normals                          // Texture Coordinates

        // Half of top face
        -0.25f, 0.0f, 0.0f,     0.0f, normalTop.y, normalTop.z,     1.0f, 0.0f, // Vertex 0
         0.25f, 0.0f, 0.0f,     0.0f, normalTop.y, normalTop.z,     1.0f, 1.0f, // Vertex 1
        -0.4f,  0.2f, 0.4f,     0.0f, normalTop.y, normalTop.z,     0.944f, 0.0f, // Vertex 2

        // Other half of top face
         0.25f, 0.0f, 0.0f,     0.0f, normalTop.y, normalTop.z,     1.0f, 1.0f, // Vertex 3
        -0.4f,  0.2f, 0.4f,     0.0f, normalTop.y, normalTop.z,     0.944f, 0.0f, // Vertex 4
         0.4f,  0.2f, 0.4f,     0.0f, normalTop.y, normalTop.z,     0.944f, 1.0f, // Vertex 5

        // Half of bottom face
        -0.25f, 0.0f, 0.0f,     0.0f, -normalTop.y, normalTop.z,    1.0f, 0.0f, // Vertex 6
         0.25f, 0.0f, 0.0f,     0.0f, -normalTop.y, normalTop.z,    1.0f, 1.0f, // Vertex 7
        -0.4f, -0.2f, 0.4f,     0.0f, -normalTop.y, normalTop.z,    0.944f, 0.0f, // Vertex 8

        // Other half of bottom face
         0.25f, 0.0f, 0.0f,     0.0f, -normalTop.y, normalTop.z,    1.0f, 1.0f, // Vertex 9
        -0.4f, -0.2f, 0.4f,     0.0f, -normalTop.y, normalTop.z,    0.944f, 0.0f, // Vertex 10
         0.4f, -0.2f, 0.4f,     0.0f, -normalTop.y, normalTop.z,    0.944f, 1.0f, // Vertex 11

        // Left side
        -0.25f, 0.0f, 0.0f,     normalSide.x, 0.0f, normalTop.z,    1.0f, 0.5f, // Vertex 12
        -0.4f,  0.2f, 0.4f,     normalSide.x, 0.0f, normalTop.z,    0.944f, 1.0f, // Vertex 13
        -0.4f, -0.2f, 0.4f,     normalSide.x, 0.0f, normalTop.z,    0.944f, 0.0f, // Vertex 14

        // Right side
         0.25f, 0.0f, 0.0f,     -normalSide.x, 0.0f, normalTop.z,   1.0f, 0.5f, // Vertex 15
         0.4f,  0.2f, 0.4f,     -normalSide.x, 0.0f, normalTop.z,   0.944f, 1.0f, // Vertex 16
         0.4f, -0.2f, 0.4f,     -normalSide.x, 0.0f, normalTop.z,   0.944f, 0.0f, // Vertex 17

        // Top extension
         0.4f,  0.2f, 0.4f,     0.0f, normalTop.y, normalTop.z,     0.944f, 1.0f, // Vertex 18
        -0.4f,  0.2f, 0.4f,     0.0f, normalTop.y, normalTop.z,     0.944f, 0.0f, // Vertex 19
         0.0f,  0.5f, 1.0f,     0.0f, normalTop.y, normalTop.z,     0.86f, 0.5f, // Vertex 20

        // Bottom extension
         0.4f, -0.2f, 0.4f,     0.0f, -normalTop.y, normalTop.z,    0.944f, 1.0f, // Vertex 21
        -0.4f, -0.2f, 0.4f,     0.0f, -normalTop.y, normalTop.z,    0.944f, 0.0f, // Vertex 22
         0.0f, -0.5f, 1.0f,     0.0f, -normalTop.y, normalTop.z,    0.86f, 0.5f, // Vertex 23

        // Left extension top
        -0.4f,  0.2f, 0.4f,     -0.5f,  0.5f, 0.0f,                 0.944f, 0.125f, // Vertex 24
         0.0f,  0.5f, 1.0f,     -0.5f,  0.5f, 0.0f,                 0.86f, 0.0f, // Vertex 25
        -0.4f,  0.0f, 1.0f,     -0.5f,  0.5f, 0.0f,                 0.86f, 0.25f, // Vertex 26

        // Left extension middle
        -0.4f,  0.2f, 0.4f,     -1.0f, 0.0f, 0.0f,                  0.944f, 1.0f, // Vertex 17
        -0.4f, -0.2f, 0.4f,     -1.0f, 0.0f, 0.0f,                  0.944f, 0.0f, // Vertex 18
        -0.4f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,                  0.86f, 0.5f, // Vertex 29

        // Left extension bottom
        -0.4f, -0.2f, 0.4f,     -0.5f, -0.5f, 0.0f,                 0.944f, 0.375f, // Vertex 30
         0.0f, -0.5f, 1.0f,     -0.5f, -0.5f, 0.0f,                 0.86f, 0.25f, // Vertex 31
        -0.4f,  0.0f, 1.0f,     -0.5f, -0.5f, 0.0f,                 0.86f, 0.5f, // Vertex 32

        // Right extension top
         0.4f,  0.2f, 0.4f,     0.5f,  0.5f, 0.0f,                  0.944f, 0.625f, // Vertex 33
         0.0f,  0.5f, 1.0f,     0.5f,  0.5f, 0.0f,                  0.86f, 0.5f, // Vertex 34
         0.4f,  0.0f, 1.0f,     0.5f,  0.5f, 0.0f,                  0.86f, 0.75f, // Vertex 35

        // Right extension middle
         0.4f,  0.2f, 0.4f,     1.0f, 0.0f, 0.0f,                   0.944f, 1.0f, // Vertex 36
         0.4f, -0.2f, 0.4f,     1.0f, 0.0f, 0.0f,                   0.944f, 0.0f, // Vertex 37
         0.4f,  0.0f, 1.0f,     1.0f, 0.0f, 0.0f,                   0.86f, 0.5f, // Vertex 38

        // Right extension bottom
         0.4f, -0.2f, 0.4f,     0.5f, -0.5f, 0.0f,                  0.944f, 0.875f, // Vertex 39
         0.0f, -0.5f, 1.0f,     0.5f, -0.5f, 0.0f,                  0.86f, 0.75f, // Vertex 40
         0.4f,  0.0f, 1.0f,     0.5f, -0.5f, 0.0f,                  0.86f, 1.0f, // Vertex 41
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create VBO
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}


/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}

// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}