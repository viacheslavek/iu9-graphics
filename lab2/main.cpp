#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <array>

using std::sin, std::cos;

float statick_theta = 0.6f;
float statick_phi = 0.7f;
float theta = 0.65f;
float phi= 0.74f;
float alpha = 0.f;
float beta = 0.f;
bool flag = false;

const GLfloat* get_dimetric_matrix(GLfloat theta, GLfloat phi) {
    return new GLfloat[16] {
            cos(phi), sin(phi)*sin(theta), sin(phi)*cos(theta), 0,
            0, cos(theta), -sin(theta), 0,
            sin(phi), -cos(phi)*sin(theta), -cos(phi)*cos(theta), 0,
            0, 0, 0, 1
    };
}

const GLfloat* get_turn_matrix(GLfloat statick_theta, GLfloat statick_phi ) {
    return new GLfloat[16] {
            cos(statick_phi), sin(statick_phi )*sin(statick_theta), sin(statick_theta)*cos(statick_theta), 0,
            0, cos(statick_theta), -sin(statick_theta), 0,
            sin(statick_phi), -cos(phi)*sin(statick_theta), -cos(phi)*cos(statick_theta), 0,
            0, 0, 0, 1
    };
}

void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_RIGHT) {
            alpha += 0.1;
        }
        else if (key == GLFW_KEY_LEFT) {
            alpha -= 0.1;
        }
        else if (key == GLFW_KEY_UP) {
            beta += 0.1;
        }
        else if (key == GLFW_KEY_DOWN) {
            beta -= 0.1;
        }
        else if (key == GLFW_KEY_Q) {
            flag = !flag;
        }

        else if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    }
}

void draw_cube_simple(float size) {
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

float get_theta(float xx, float yx) {
    float fz = sqrt(xx*xx + yx*yx) / sqrt(2);
    return asin(fz);
}

float get_phi(float xy, float yy) {
    float fz = sqrt(xy*xy + yy*yy);
    fz = fz / sqrt(2 - fz*fz);
    return asin(fz);
}

void display (GLFWwindow* window) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);

    const GLfloat* m2 = get_turn_matrix(theta, phi);
    const GLfloat* translet_matrix = new GLfloat[16] {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0.8, 0.8, 0, 1
    };

    glMultMatrixf(translet_matrix);
    glMultMatrixf(m2);
    draw_cube_simple(0.2);

    glPopMatrix();
    glPushMatrix();
    if (flag) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    theta = get_theta(0.7, 0.7);
    phi = get_phi(0.7, 0.7);

    const GLfloat* m = get_dimetric_matrix(theta + beta, phi + alpha);
    glMatrixMode(GL_PROJECTION);
    glMultMatrixf(m);
    draw_cube_simple(0.7);

    glfwSwapBuffers(window);
    glfwPollEvents();
    delete[] m;
    delete[] translet_matrix;
    delete[] m2;
}

int main() {

    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(860, 860, "lab2", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key);

    while (!glfwWindowShouldClose(window)) {
        display(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
