#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

using std::sin, std::cos;

float alpha = 15.f;
float beta = 48.f;

int segments = 10;


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
        else if (key == GLFW_KEY_W) {
            segments += 5;
        }
        else if (key == GLFW_KEY_S) {
            segments -= 5;
        }
        else if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    }
}

void draw_cylinder(float radius, float height, int segments) {
    float segmentAngle = 2.0f*M_PI / segments;

    glBegin(GL_TRIANGLE_STRIP); // начинаем отрисовку вершин

    //Вершины для цилиндра
    for (int i = 0; i <= segments; i++) {
        float x = radius * cos(i * segmentAngle);
        float y = radius * sin(i * segmentAngle);
        float z = 0.0f;
        glVertex3f(x, y, z); // добавляем вершину на линию, параллельную оси Z
        glColor3f(0.f, i / 10.0, 1.f);
        glVertex3f(x, y, height);  // добавляем вершину на линию высот
    }

    // Вершины для кругов (верх и низ)
    for (int i = 0; i <= segments; i++) {
        float x = radius * cos(i * segmentAngle);
        float y = radius * sin(i * segmentAngle);

        glVertex3f(x, y, 0.0f); // добавляем вершину для верхнего круга
        glColor3f(0.f, 0.0f, 1.f);
        glVertex3f(x, y, height);  // добавляем вершину для нижнего круга
    }

    glEnd(); // заканчиваем отрисовку вершин
}

void display(GLFWwindow* window) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glRotatef(beta * 50.f, 1.0, 0.0, 0.0);
    glRotatef(alpha * 50.f, 0.0, 1.0, 0.0);

    draw_cylinder(0.5, 0.6, segments);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {

    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(860, 860, "lab3", NULL, NULL);

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
