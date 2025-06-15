#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include"shader.h"

#include <iostream>

static unsigned int ss_id = 0;
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) {
    int pixelChannel = 3;
    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
    GLubyte * pixels = new GLubyte [totalPixelSize];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
    std::ofstream fout(file_name);
    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t cur = pixelChannel * ((height - i - 1) * width + j);
            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
        }
        fout << std::endl;
    }
    ss_id ++;
    delete [] pixels;
    fout.flush();
    fout.close();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("default.vert", "default.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
        // position           // colour
        // back face
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

        // front face
        -0.5f, -0.5f,  0.5f,  1.0, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0, 0.0f, 1.0f,

        // left face
        -0.5f,  0.5f,  0.5f,  1.0, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0, 0.0f, 0.0f,

        // right face
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

        // bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

        // top face
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

    
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate shader
        ourShader.use();

        //rotatation variables
        //sun
        float day = 100;
        float get_sun_rotate_angle_around_itself(glm::radians((360.0f/27.0) * day));
        //earth
        float get_earth_rotate_angle_around_sun(glm::radians((360.0f/365.0) * day));
        float get_earth_rotate_angle_around_itself(glm::radians((360.0f) * day));
        //moon
        float get_moon_rotate_angle_around_earth(glm::radians((360.0f/28.0) * day));
        float get_moon_rotate_angle_around_itself(glm::radians((360.0f/28.0) * day));

        // sun transformations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glm::mat4 sun         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        sun = glm::scale(sun, glm::vec3(12.0f, 12.0f, 12.0f));
        glm::vec3 rotationAxisSun(0.0f, 1.0f, 0.0f);  // Rotate around the y-axis
        sun = glm::rotate(sun, get_sun_rotate_angle_around_itself, rotationAxisSun);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sun));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);  

        // earth transformations
        glm::mat4 earth         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // Define the radius of the orbit
        float orbitRadius = 24.0f;
        // Calculate the position of the earth as it orbits around the sun
        float xPosition = orbitRadius * cos(get_earth_rotate_angle_around_sun);
        float zPosition = orbitRadius * sin(get_earth_rotate_angle_around_sun);
        // Apply orbiting transformation (e.g., around the outer cube)
        earth = glm::translate(earth, glm::vec3(xPosition, 0.0f, zPosition));
        // Scale
        earth = glm::scale(earth, glm::vec3(6.0f, 6.0f, 6.0f));
        // revolving axis 23.4
        earth = glm::rotate(earth, glm::radians(23.40f), glm::vec3(0.0f, 0.0f, -1.0f));
        // Apply self-rotation (e.g., around its own center)
        earth = glm::rotate(earth, get_earth_rotate_angle_around_itself, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earth));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // moon transformations
        glm::mat4 moon         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        // Define the radius of the orbit
        float orbitRadiusMoon = 12.0f;
        // Calculate the position of the moon as it orbits around the earth 
        float xPositionMoon = orbitRadiusMoon * cos(get_moon_rotate_angle_around_earth);
        float zPositionMoon = orbitRadiusMoon * sin(get_moon_rotate_angle_around_earth);
        // Apply orbiting transformation (e.g., around the outer cube)
        moon = glm::translate(moon, glm::vec3(xPositionMoon + xPosition, 0.0f, zPositionMoon + zPosition));
        // Scale
        moon = glm::scale(moon, glm::vec3(3.0f, 3.0f, 3.0f));
        // Apply self-rotation (e.g., around its own center)
        moon = glm::rotate(moon, get_moon_rotate_angle_around_itself, glm::vec3(0.0f, -1.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(moon));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        //view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // retrieve the matrix uniform locations
        //unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
 
        // pass them to the shaders (3 different ways)
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sun));

        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        view = glm::lookAt(glm::vec3(80.0f, 70.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(30.0f), 4.0f/3.0f, 0.1f, 1000.0f);
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);
        
        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_P) ==GLFW_PRESS)
    {
        std::cout << "Capture Window " << ss_id << std::endl;
        int buffer_width, buffer_height;
        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
        dump_framebuffer_to_ppm("rotate_d365", buffer_width, buffer_height);
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}