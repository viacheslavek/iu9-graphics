#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include <iostream>

float sizeMini = 0.2;
float degree = 0.0;
float sizeBig = 0.4;

void key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // добавляю кручение на объект
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if (key == GLFW_KEY_UP) {
            degree += 2.0f;
        }
        else if (key == GLFW_KEY_DOWN) {
            degree -= 2.0f;
        }
    }
}

void drowCubeSimple(GLFWwindow *window, float size) {
    glBegin(GL_QUADS);

    glColor3f(0.f, 0.0f, 1.f);
    // левая грань
    glVertex3f( -size / 2, -size / 2, -size / 2);
    glVertex3f( -size / 2,  size / 2, -size / 2);
    glVertex3f( -size / 2,  size / 2,  size / 2);
    glVertex3f( -size / 2, -size / 2,  size / 2);
    // правая грань
    glColor3f(1.f, 0.0f, 0.f);
    glVertex3f(  size / 2, -size / 2, -size / 2);
    glVertex3f(  size / 2, -size / 2,  size / 2);
    glVertex3f(  size / 2,  size / 2,  size / 2);
    glVertex3f(  size / 2,  size / 2, -size / 2);
    // нижняя грань
    glColor3f(0.f, 1.0f, 0.f);
    glVertex3f( -size / 2, -size / 2, -size / 2);
    glVertex3f( -size / 2, -size / 2,  size / 2);
    glVertex3f(  size / 2, -size / 2,  size / 2);
    glVertex3f(  size / 2, -size / 2, -size / 2);
    // верхняя грань
    glColor3f(0.f, 1.0f, 1.f);
    glVertex3f( -size / 2, size / 2, -size / 2);
    glVertex3f( -size / 2, size / 2,  size / 2);
    glVertex3f(  size / 2, size / 2,  size / 2);
    glVertex3f(  size / 2, size / 2, -size / 2);
    // задняя грань
    glColor3f(1.f, 0.0f, 1.f);
    glVertex3f( -size / 2, -size / 2, -size / 2);
    glVertex3f(  size / 2, -size / 2, -size / 2);
    glVertex3f(  size / 2,  size / 2, -size / 2);
    glVertex3f( -size / 2,  size / 2, -size / 2);
    // передняя грань
    glColor3f(1.f, 1.0f, 0.f);
    glVertex3f( -size / 2, -size / 2,  size / 2);
    glVertex3f(  size / 2, -size / 2,  size / 2);
    glVertex3f(  size / 2,  size / 2,  size / 2);
    glVertex3f( -size / 2,  size / 2,  size / 2);
    glEnd();
}

void printQubeMini(GLFWwindow *window) {

    glLoadIdentity();
    glRotatef(115.f, 1.f, 0.5f, 0.f);

    glTranslatef(0.7f, 0.4f, 0.2f);

    drowCubeSimple(window, sizeMini);

    glPopMatrix();
}

void printQubeBig(GLFWwindow *window) {

    glLoadIdentity();

    glRotatef(degree + 115.f, 1.f, 0.5f, 0.f);

    float fi = 45.f;
    float te = 35.26f;

    GLfloat m[4][4] = {
            {cos(fi), 0.0f, sin(fi), 0.0f},
            {sin(fi)*sin(te), cos(te), -cos(fi)*sin(te), 0.0f},
            {sin(fi)*cos(te), -sin(te), -cos(fi)*cos(te), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
    };
    glMultMatrixf(&m[0][0]);

    glPushMatrix();
    drowCubeSimple(window, sizeBig);

    glPopMatrix();
}


void display(GLFWwindow *window) {

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    printQubeMini(window);

    printQubeBig(window);

}

int main(int argc, char const *argv[]) {
    if (!glfwInit()) {
        exit(1);
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Lab 2", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key);

    while (!glfwWindowShouldClose(window)) {
        display(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


