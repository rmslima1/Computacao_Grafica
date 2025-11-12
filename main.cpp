#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "include/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/Shader.h"
#include <iostream>
#include <vector>
#include "include/scene.h"
#include "include/Cube.h"
#include "include/Cilindro.h"
#include "include/Esfera.h"
#include "include/PrismaTriangular.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float sensitivity = 0.2f;

void viraCamera(float x, float y) {
    yaw += x * sensitivity;
    pitch += y * sensitivity;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}




int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Casa 3D - Trabalho T2", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(glewInit() != GLEW_OK) {
        std::cout << "Erro ao iniciar GLEW!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shaders/vertex.glsl", "shaders/fragment.glsl");

  unsigned int wallTexture = loadTexture("res/images/wall.jpg");
    unsigned int floorTexture = loadTexture("res/images/floor.jpg");
    unsigned int woodTexture = loadTexture("res/images/wood.jpg");
    unsigned int metalTexture = loadTexture("res/images/metal.jpg");
    unsigned int wallInsideTexture = loadTexture("res/images/wall_inside.jpg");
    unsigned int cottonTexture = loadTexture("res/images/cotton.jpg");
    unsigned int pillowTexture = loadTexture("res/images/pillow.jpg");
    unsigned int porcelainTexture = loadTexture("res/images/porcelana.jpg");
    unsigned int bhTexture = loadTexture("res/images/bh.png");
    unsigned int roofTexture = loadTexture("res/images/telhado.jpg");
    unsigned int lightTexture = loadTexture("res/images/light1.jpg");
    unsigned int doorTexture = loadTexture("res/images/door.jpg");
    unsigned int tvTexture = loadTexture("res/images/tv.jpg");
    unsigned int sofaTexture = loadTexture("res/images/sofa.jpg");
    unsigned int plasticTexture = loadTexture("res/images/plastic.jpg");
    unsigned int bhFullTexture = loadTexture("res/images/bhfull.png");
    unsigned int matressTexture = loadTexture("res/images/matress.jpg");

    Scene scene = criarCena(
        wallTexture, floorTexture, woodTexture, metalTexture,
        wallInsideTexture, cottonTexture, pillowTexture,
        porcelainTexture, bhTexture, roofTexture,
        lightTexture, doorTexture, tvTexture, sofaTexture, plasticTexture, bhFullTexture, matressTexture
    );

    std::vector<Cube*>& cubes = scene.cubes;
    std::vector<Cylinder*>& cylinders = scene.cylinders;
    std::vector<Sphere*>& spheres = scene.spheres;
    std::vector<TriangularPrism*>& prisms = scene.prisms;

    for (auto* cube : scene.cubes) cube->draw();
    for (auto* sphere : scene.spheres) sphere->draw();
    for (auto* cylinder : scene.cylinders) cylinder->draw();
    for (auto* prism : scene.prisms) prism->draw();


    for (auto cube : cubes) cube->init();
    for (auto cylinder : cylinders) cylinder->init();
    for (auto sphere : spheres) sphere->init();
    for (auto prism : prisms) prism->init();

    ourShader.use();
    ourShader.setInt("texture1", 0);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                               (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                               0.1f, 100.0f);

        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (int i = 0; i < 9; i++) {
            ourShader.setMat4("model", cubes[i]->getModelMatrix());
            cubes[i]->draw();
        }

        glBindTexture(GL_TEXTURE_2D, floorTexture);
        if (cubes.size() > 8) {
            ourShader.setMat4("model", cubes[8]->getModelMatrix());
            cubes[8]->draw();
        }

        glBindTexture(GL_TEXTURE_2D, woodTexture);
        for (size_t i = 9; i < cubes.size(); i++) {
            ourShader.setMat4("model", cubes[i]->getModelMatrix());
            cubes[i]->draw();
        }

        glBindTexture(GL_TEXTURE_2D, metalTexture);
        for (auto cylinder : cylinders) {
            ourShader.setMat4("model", cylinder->getModelMatrix());
            cylinder->draw();
        }

        glBindTexture(GL_TEXTURE_2D, metalTexture);
        for (auto sphere : spheres) {
            ourShader.setMat4("model", sphere->getModelMatrix());
            sphere->draw();
        }

        glBindTexture(GL_TEXTURE_2D, woodTexture);
        for (auto prism : prisms) {
            ourShader.setMat4("model", prism->getModelMatrix());
            prism->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    for (auto cube : cubes) delete cube;
    for (auto cylinder : cylinders) delete cylinder;
    for (auto sphere : spheres) delete sphere;
    for (auto prism : prisms) delete prism;

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.03f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        viraCamera(0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        viraCamera(0.0f, -1.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        viraCamera(-1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        viraCamera(1.0f, 0.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
