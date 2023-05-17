#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

using std::sin, std::cos;

float alpha = 15.f;
float beta = 48.f;

int segments = 10;

float cylinderRadius = 0.2f; // Радиус цилиндра
float cylinderVelocity = 0.1f; // Скорость цилиндр

float screenWidth = 1.0f;
float cylinderPositionX = screenWidth / 2.0f; // Начальная позиция цилиндра по оси X
float velocity = 0.001f;



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
        else if (key == GLFW_KEY_D) {
            velocity += 0.0001;
        }
        else if (key == GLFW_KEY_A) {
            velocity -= 0.0001;
        }
        else if (key == GLFW_KEY_R) {
            cylinderRadius += 0.01f;
        }
        else if (key == GLFW_KEY_F) {
            cylinderRadius -= 0.01f;
        }
        else if (key == GLFW_KEY_Q) {
            velocity = 0.0f;
        }
        else if (key == GLFW_KEY_Z) {
            velocity = 0.001f;
        }
        else if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    }
}



void draw_cylinder(float radius, float height, int segments, float xPosition) {
    float segmentAngle = 2.0f*M_PI / segments;

    glBegin(GL_TRIANGLE_STRIP); // начинаем отрисовку вершин

    //Вершины для цилиндра
    for (int i = 0; i <= segments; i++) {
        float x = radius * cos(i * segmentAngle) + xPosition;
        float y = radius * sin(i * segmentAngle);
        float z = 0.0f;
        glVertex3f(x, y, z); // добавляем вершину на линию, параллельную оси Z
        glColor3f(0.f, i / 10.0, 1.f);
        glVertex3f(x, y, height);  // добавляем вершину на линию высот
        glVertex3f(x, y, 0.0f); // добавляем вершину для верхнего круга

        glVertex3f(x, y, 0.0f); // добавляем вершину для верхнего круга
        glColor3f(0.f, 0.0f, 1.f);
        glVertex3f(x, y, height);  // добавляем вершину для нижнего круга
    }

    glEnd(); // заканчиваем отрисовку вершин
}

void updateCylinderPosition()
{
    // Изменяем позицию цилиндра на основе скорости и времени
    cylinderPositionX += velocity;

    // Проверяем, достиг ли цилиндр левой границы экрана
    if (cylinderPositionX - cylinderRadius <= 0.0f)
    {
        // Цилиндр ударился об левую границу, меняем направление
        velocity = std::abs(velocity); // Вправо
    }

    // Проверяем, достиг ли цилиндр правой границы экрана
    if (cylinderPositionX + cylinderRadius >= 1.0)
    {
        // Цилиндр ударился об правую границу, меняем направление
        velocity = -std::abs(velocity); // Влево
    }
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

    updateCylinderPosition();

    std::cout << cylinderPositionX << "\n";

    draw_cylinder(cylinderRadius, 0.6, segments, cylinderPositionX);

    glfwSwapBuffers(window);
    glfwPollEvents();
}


int main() {

    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "lab6", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        exit(1);
    }


    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key);

    GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat diffusion[] = {0.0, 1.0, 0.0, 0.0};
    GLfloat ambient[] = {0.0, 0.0, 1.0, 0.0};
    GLfloat specular[] = {0.0, 1.0, 0.0, 0.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusion);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    while (!glfwWindowShouldClose(window)) {
        display(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

