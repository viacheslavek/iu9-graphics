#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <unistd.h>
#include <vector>

struct Pixel {
    double x;
    double y;
};

int windowWidth = 0;
int windowHeight = 0;
GLubyte borderColor[3] = {255, 255, 0};
GLubyte fillColor[3] = {255, 255, 255};
GLubyte *frame_buffer = new GLubyte[windowWidth * windowHeight];

std::vector<Pixel> pixels = std::vector<Pixel>();
Pixel innerPixel;

bool filled = false;
bool closed = false;

void clearBuffer() {
    for (int i = 0; i < windowHeight * windowWidth * 3; ++i) {
        frame_buffer[i] = 0;
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }

        if (key == GLFW_KEY_Q) {
            pixels.resize(0);
            clearBuffer();
            closed = false;
            filled = false;
        }

        if (key == GLFW_KEY_E) {
            glfwGetCursorPos(window, &innerPixel.x, &innerPixel.y);
            innerPixel.y = windowHeight - innerPixel.y;
            filled = !filled;
        }

        if (key == GLFW_KEY_F) {
            closed = true;
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int btn, int action, int mods) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;

        glfwGetCursorPos(window, &x, &y);
        pixels.push_back({x, windowHeight - y});
    }
}

void setPixel(int x, int y, GLubyte *color) {
    int index = 3 * (x + y * windowHeight);
    frame_buffer[index] = color[0];
    frame_buffer[index + 1] = color[1];
    frame_buffer[index + 2] = color[2];
}

void drawLine(int x1, int y1, int x2, int y2) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    setPixel(x2, y2, borderColor);
    while (x1 != x2 || y1 != y2) {
        setPixel(x1, y1, borderColor);
        int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

void getPixel(const Pixel &p, GLubyte *res) {
    int index = 3 * p.x + 3 * p.y * windowWidth;
    res[0] = frame_buffer[index];
    res[1] = frame_buffer[index + 1];
    res[2] = frame_buffer[index + 2];
}

bool isBoundary(const Pixel &p) {
    GLubyte tmp[3];
    getPixel(p, tmp);
    return (int(tmp[0]) == 255) && (int(tmp[1]) == 255) && (int(tmp[2]) == 0);
}

bool isInner(const Pixel &p) {
    GLubyte tmp[3];
    getPixel(p, tmp);
    return (int(tmp[0]) == 255) && (int(tmp[1]) == 255) && (int(tmp[2]) == 255);
}

bool compareByX(const Pixel &p1, const Pixel &p2) { return p1.x < p2.x; }

bool compareByY(const Pixel &p1, const Pixel &p2) { return p1.y < p2.y; }

Pixel checkLine(Pixel p) {
    Pixel p1 = {p.x, p.y};
    while (true) {
        if (isInner(p1)) {
            continue;
        } else if (isBoundary(p)) {
            break;
        } else {
            break;
        }
        p1.x--;
    }

    return p1;
}

//void fillPolygon() {
//    std::vector<Pixel> stack;
//    stack.push_back(innerPixel);
//
//    while (stack.size() != 0) {
//        Pixel p = stack.back();
//        int xl = p.x;
//        int xr = p.x;
//        int y = p.y;
//
//        stack.pop_back();
//        setPixel(cur.x, cur.y, fillColor);
//        if (!isInner(Pixel{cur.x + 1, cur.y}) &&
//            !isBoundary(Pixel{cur.x + 1, cur.y})) {
//            stack.push_back(Pixel{cur.x + 1, cur.y});
//        }
//        if (!isInner(Pixel{cur.x - 1, cur.y}) &&
//            !isBoundary(Pixel{cur.x - 1, cur.y})) {
//            stack.push_back(Pixel{cur.x - 1, cur.y});
//        }
//        if (!isInner(Pixel{cur.x, cur.y - 1}) &&
//            !isBoundary(Pixel{cur.x, cur.y - 1})) {
//            stack.push_back(Pixel{cur.x, cur.y - 1});
//        }
//        if (!isInner(Pixel{cur.x, cur.y + 1}) &&
//            !isBoundary(Pixel{cur.x, cur.y + 1})) {
//            stack.push_back(Pixel{cur.x, cur.y + 1});
//        }
//        //
//        //
//        // double y = cur.y;
//        // double x = cur.x;
//        // double x1, x2;
//        // Pixel p = {x, y};
//        //
//        // while (true) {
//        //   p.x = x;
//        //   p.y = y;
//        //   if (isBoundary(p)) {
//        //     break;
//        //   } else {
//        //     setPixel(x, y, fillColor);
//        //     x--;
//        //   }
//        // }
//        // y = cur.y;
//        // x = cur.x;
//        //
//        // while (true) {
//        //   p.x = x;
//        //   p.y = y;
//        //   if (isBoundary(p)) {
//        //     break;
//        //   } else {
//        //     setPixel(x, y, fillColor);
//        //     x++;
//        //   }
//        // }
//    }
//}

void display(GLFWwindow *window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (width != windowWidth || height != windowHeight) {
        delete[] frame_buffer;
        windowWidth = width;
        windowHeight = height;
        frame_buffer = new GLubyte[windowWidth * windowHeight * 3];
        for (int i = 0; i < windowHeight * windowWidth * 3; ++i) {
            frame_buffer[i] = 0;
        }
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    if (pixels.size() == 1) {
        setPixel(pixels[0].x, pixels[0].y, borderColor);
    }

    for (int i = 1; i < pixels.size(); ++i) {
        drawLine(pixels[i - 1].x, pixels[i - 1].y, pixels[i].x, pixels[i].y);
    }

    if (pixels.size() > 2 && closed) {
        drawLine(pixels.rbegin()->x, pixels.rbegin()->y, pixels[0].x, pixels[0].y);
        if (filled) {
        }
    }

    glRasterPos2f(0, 0);
    glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE,
                 frame_buffer);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {

    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(500, 500, "lab4", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    for (int i = 0; i < windowHeight * windowWidth; ++i) {
        frame_buffer[i] = 0;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    while (!glfwWindowShouldClose(window))
        display(window);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
