#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

// Шейдерные программы
const GLchar* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)";

const GLchar* fragmentShaderSource = R"(
    #version 330 core
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)";

#include <GL/glew.h>
#include <GL/glut.h>

// Функция для компиляции и связывания шейдера
GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource)
{
    // Создание вершинного шейдера
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Проверка наличия ошибок при компиляции вершинного шейдера
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Ошибка при компиляции вершинного шейдера: %s\n", infoLog);
    }

    // Создание фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Проверка наличия ошибок при компиляции фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Ошибка при компиляции фрагментного шейдера: %s\n", infoLog);
    }

    // Создание шейдерной программы и связывание шейдеров
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Проверка наличия ошибок при связывании шейдеров
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Ошибка при связывании шейдерной программы: %s\n", infoLog);
    }

    // Удаление шейдеров после связывания
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Функция отрисовки цилиндра
void drawCylinder()
{
    // ... (ваш код для отрисовки цилиндра, используя примитивы OpenGL)
    // Пример кода отрисовки цилиндра:

    // Здесь должны быть вызовы функций OpenGL для отрисовки цилиндра
    // используйте функции, такие как glVertex3f, glBegin, glEnd и т.д.
}

// Функция отрисовки сцены
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Здесь должны быть вызовы функций OpenGL для отрисовки сцены
    // В данном примере вызывается только функция для отрисовки цилиндра
    drawCylinder();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // Инициализация GLUT и создание окна
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cylinder Shader Example");

    // Инициализация GLEW
    glewInit();

    // Создание и компиляция шейдерной программы
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Установка шейдерной программы
    glUseProgram(shaderProgram);

    // Регистрация функций обратного вызова
    glutDisplayFunc(renderScene);

    // Запуск основного цикла GLUT
    glutMainLoop();

    return 0;
}


