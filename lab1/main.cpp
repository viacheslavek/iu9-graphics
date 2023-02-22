#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include <iostream>

float degree = 0.0;
int cnt = 10;

void key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if (key == GLFW_KEY_UP) {
            degree += 0.2f;
        }
        else if (key == GLFW_KEY_DOWN) {
            degree -= 0.2f;
        }
        else if (key == GLFW_KEY_LEFT) {
            cnt -= 2;
        }
        else if (key == GLFW_KEY_RIGHT) {
            cnt += 2;
        }
    }
}

void display(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                         // перемещаем в центр СК
    glRotatef(degree * 50.f, 1.f, 0.f, 0.f);  // отвечает за поворот

    glScalef(0.6, 0.6, 1);                    // изменяем мастштаб

    float x, y;
    float l = 0.5;
    float a = M_PI * 2 / cnt;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.f, 0.0f, 1.f);
    glVertex2f(0.f, 0.f);
    for (int i = -1; i < cnt; i++) {
        x = sin(a * i) * l;
        y = cos(a * i ) * l;
        glColor3f(0.f, i / 10.0, 1.f);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

int main(int argc, char const *argv[]) {
    if (!glfwInit()) {
        exit(1);
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "Lab 1", NULL, NULL);

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

