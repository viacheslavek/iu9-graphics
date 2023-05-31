#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "../lab6/stb_image.h"

using std::sin, std::cos;

float alpha = 15.f;
float beta = 48.f;

int segments = 10;

float cylinderRadius = 0.2f; // Радиус цилиндра
float cylinderVelocity = 0.1f; // Скорость цилиндр

float screenWidth = 1.0f;
float cylinderPositionX = screenWidth / 2.0f; // Начальная позиция цилиндра по оси X
float velocity = 0.001f;

GLuint textureID;

// ОПТИМИЗАЦИИ

GLuint cylinderDisplayList;


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

void loadTexture(const char* filename)
{
    // Загрузка изображения с помощью библиотеки stb_image
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (!image)
    {
        std::cout << "Can't load image" << "\n";
        return;
    }

    std::cout << "image load" << "\n";

    // Генерация и привязка текстуры
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Настройка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка данных текстуры
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Освобождение памяти, выделенной для изображения
    stbi_image_free(image);
}


void draw_cylinder(float radius, float height, int segments, float xPosition) {
    float segmentAngle = 2.0f*M_PI / segments;

    // glNewList(cylinderDisplayList, GL_COMPILE); // |1 оптимизация |


    glBegin(GL_TRIANGLE_STRIP);
    //Вершины для цилиндра
    for (int i = 0; i <= segments; i++) {
        float x = radius * cos(i * segmentAngle) + xPosition;
        float y = radius * sin(i * segmentAngle);
        float z = 0.0f;

        float u = static_cast<float>(i) / segments;
        float v = 0.0f;

        glTexCoord2f(u, v);
        glVertex3f(x, y, z); // добавляем вершину на линию, параллельную оси Z

        glColor3f(0.f, i / 10.0, 1.f);

        glTexCoord2f(u, 1.0f);
        glVertex3f(x, y, height);  // добавляем вершину на линию высот

        glVertex3f(x, y, 0.0f); // добавляем вершину для верхнего круга

        glVertex3f(x, y, 0.0f); // добавляем вершину для верхнего круга
        glColor3f(0.f, 0.0f, 1.f);
        glVertex3f(x, y, height);  // добавляем вершину для нижнего круга
    }

    // glEndList();


    glEnd(); // заканчиваем отрисовку вершин | БЕЗ ОПТИМИЗАЦИЙ |
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

void draw_cylinder2(float radius, float height, int segments, float xPosition) {
    float segmentAngle = 2.0f * M_PI / segments;

    // Создаем массив вершин и текстурных координат
    GLfloat vertices[(segments + 1) * 3 * 2];
    GLfloat texCoords[(segments + 1) * 3 * 2];

    // Заполняем массив вершин и текстурных координат
    for (int i = 0; i <= segments; i++) {

        float u = static_cast<float>(i) / segments;
        float v = 0.0f;

        float x = radius * cos(i * segmentAngle) + xPosition;
        float y = radius * sin(i * segmentAngle);

        // Верхняя точка цилиндра
        vertices[i * 6] = x;
        vertices[i * 6 + 1] = y;
        vertices[i * 6 + 2] = 0.0f;
        texCoords[i * 2] = u;

        // Нижняя точка цилиндра
        vertices[i * 6 + 3] = x;
        vertices[i * 6 + 4] = y;
        vertices[i * 6 + 5] = height;
        texCoords[i * 2 + 1] = v;
        texCoords[i * 2 + 2] = u;
        texCoords[i * 2 + 3] = 1.0f;

    }

    // Включаем использование массива вершин и текстурных координат
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Указываем, как интерпретировать данные вершин и текстурных координат
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    // Привязываем текстуру
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Рисуем цилиндр с использованием массива вершин и текстурных координат
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);

    // Отключаем использование массива текстурных координат
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


int counter = 0;

void display(GLFWwindow* window) {


    // 4 оптимизация: глубинное тестирование

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwWindowHint(GLFW_DEPTH_BITS, 12);

    counter++;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glRotatef(beta * 50.f, 1.0, 0.0, 0.0);
    glRotatef(alpha * 50.f, 0.0, 1.0, 0.0);

    glCallList(cylinderDisplayList); // 1 оптимизация

    updateCylinderPosition();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    draw_cylinder(cylinderRadius, 0.6, segments, cylinderPositionX);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (counter == 100) {
        counter = 0;

        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

        // Вычисление разницы с предыдущим временем
        static std::chrono::high_resolution_clock::time_point previousTime = currentTime;
        std::chrono::duration<double> deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        double deltaTimeSeconds = deltaTime.count();

        // Вывод результатов
        std::cout << "Прошедшее время с предыдущего кадра: " << deltaTimeSeconds << " секунд" << std::endl;
    }

}


int main() {

    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "lab7", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        exit(1);
    }


    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key);

    loadTexture("/Users/slavaruswarrior/Documents/GitHub/iu9-graphics/lab6/texture.jpg");

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

    // Создание и инициализация дисплейного списка 1 оптимизация
    cylinderDisplayList = glGenLists(1);
    if (cylinderDisplayList == 0) {
        std::cerr << "Ошибка при создании дисплейного списка." << std::endl;
        return 1;
    }


    // 3 оптимизация: отсечение задних граней

//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);


    // 5 оптимизация:
    glfwSwapInterval(1);


    while (!glfwWindowShouldClose(window)) {
        glCallList(cylinderDisplayList);
        display(window);
    }

    glDeleteLists(cylinderDisplayList, 1); // 1 оптимизация

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


/// история замеров: все замеры в режиме анимации с отрисовкой текстур и одним и тем же поворотом

/// Обязателно

/// без всего: 1.66657 секунд на 100 отрисовок

/// добавил дисплейные списки: 0.933791 секунд  (1)

/// добавил вершиный массив: 0.85089            (2)

/// Дополнительтно

/// Отсчение задних граней:  0.826137 из минусов: отсекается и та часть, которую хотелось бы видеть  (3)

/// Глубинное тестирование: 0.858455 для глубины 24 пикселя и  0.863929 для глубины 2 пикселя -- странное поведение (4)
/// для 12 пикселей -- 0.77926. В сочетании с Вершиным массивом -- 0.710272

/// двойная буфферизация: 0.85689    (5)


/// хотел еще добавить включение буфера кадра и буфера глубины
//    GLuint framebuffer;
//    glGenFramebuffers(1, &framebuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//
//    GLuint depthBuffer;
//    glGenRenderbuffers(1, &depthBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
//
//    int screenWidth, screenHeight; // Здесь должны быть значения ширины и высоты окна
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
//
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

/// но для этого нужен glut, а он на маке m1 не работает((((





