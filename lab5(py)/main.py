import math
import glfw
from OpenGL.GL import *
from PolygonCut import *

window_width = 500
window_height = 500

coord_x_cut = 0
coord_y_cut = 0
vertices_cut = []
flag = 0

upright_cut = [0, 0]
down_left_cut = [window_width, window_height]

coord_x_polygon = 0
coord_y_polygon = 0
vertices_polygon = []

upright_polygon = [0, 0]
down_left_polygon = [window_width, window_height]

buffer = bytearray(window_width * 2 * window_height * 2 * 3)

for clear in range(0, 1000 * 1000):
    buffer[clear * 3] = 0
    buffer[clear * 3 + 1] = 0
    buffer[clear * 3 + 2] = 0


def cursor_pos(window, x_pos, y_pos):
    global coord_x_cut, coord_y_cut, coord_x_polygon, coord_y_polygon
    if flag == 0:
        coord_x_cut = round(x_pos)
        coord_y_cut = round(window_height - y_pos)
    elif flag == 1:
        coord_x_polygon = round(x_pos)
        coord_y_polygon = round(window_height - y_pos)


def draw_polygon(coord_x, coord_y, upright, down_left, vertices):
    if coord_x > upright[0]:
        upright[0] = coord_x

    if coord_x < down_left[0]:
        down_left[0] = coord_x

    if coord_y > upright[1]:
        upright[1] = coord_y

    if coord_y_cut < down_left[1]:
        down_left[1] = coord_y

    draw_pixel(coord_x, coord_y)
    vertices.append([coord_x, coord_y])
    print(coord_x, coord_y, flag)
    print(vertices, flag)
    if len(vertices) > 1:
        print(vertices[len(vertices) - 2][0], vertices[len(vertices) - 2][1], vertices[len(vertices) - 1][0],
              vertices[len(vertices) - 1][1])
        drawline(vertices[len(vertices) - 2][0], vertices[len(vertices) - 2][1],
                 vertices[len(vertices) - 1][0],
                 vertices[len(vertices) - 1][1])


result = [[217, 223], [149, 322], [191, 392], [261, 391], [312, 311], [254, 221]]


def draw_all_polygon_on_vertices(vertices):
    for i in range(len(vertices)):
        draw_polygon(vertices[i][0], vertices[i][1], upright_cut, down_left_cut, vertices)


def mouse_button(window, button, action, mods):
    global coord_x_cut, coord_y_cut, flag, upright_cut, down_left_cut, vertices_cut, \
        coord_x_polygon, coord_y_polygon, upright_polygon, down_left_polygon, vertices_polygon

    if flag == 2:
        return
    if action == glfw.PRESS:
        if button == glfw.MOUSE_BUTTON_LEFT:
            if flag == 0:
                print("draw_polygon cut")
                draw_polygon(coord_x_cut, coord_y_cut, upright_cut, down_left_cut, vertices_cut)
            if flag == 1:
                print("draw_polygon polygon")
                draw_polygon(coord_x_polygon, coord_y_polygon, upright_polygon, down_left_polygon, vertices_polygon)


def key_callback(window, key, scancode, action, mods):
    global vertices_cut, flag, coord_x_cut, coord_y_cut, down_left_cut, upright_cut, \
        coord_x_polygon, coord_y_polygon, vertices_polygon, upright_polygon, down_left_polygon
    if action == glfw.PRESS:
        if key == glfw.KEY_ENTER:
            if flag == 0:
                drawline(vertices_cut[len(vertices_cut) - 1][0], vertices_cut[len(vertices_cut) - 1][1],
                         vertices_cut[0][0], vertices_cut[0][1])
                flag = 1
            elif flag == 1:
                flag = 2
                drawline(vertices_polygon[len(vertices_polygon) - 1][0],
                         vertices_polygon[len(vertices_polygon) - 1][1], vertices_polygon[0][0], vertices_polygon[0][1])
            else:
                coord_x_cut = 0
                coord_y_cut = 0
                vertices_cut = []
                flag = 0
                down_left_cut = [window_width, window_height]
                upright_cut = [0, 0]

                coord_x_polygon = 0
                coord_y_polygon = 0
                vertices_polygon = []

                upright_polygon = [0, 0]
                down_left_polygon = [window_width, window_height]

                for i in range(0, 1000 * 1000):
                    buffer[i * 3] = 0
                    buffer[i * 3 + 1] = 0
                    buffer[i * 3 + 2] = 0
        if key == glfw.KEY_F:
            # cut_string = get_str_clockwise(vertices_cut)
            # polygon_string = get_str_clockwise(vertices_polygon)
            # result_string = PolyClipping(cut_string, polygon_string, False)
            # TODO: строю по полученной результирующей строке "x y x y" новые многоугольники
            draw_all_polygon_on_vertices(result)


# TODO: сделать функцию, которая список вершин преобразует в список вершин по часовой стрелке,
#  а потом переводит это в строчный формат вида "x y x y ..."
def get_str_clockwise(arr_coordinate):
    return ""


def draw_pixel(xx, y):
    if flag == 0:
        colour = 100
    else:
        colour = 255

    index = round(xx) * 2 * 3 + round(y) * 2 * 1000 * 3

    for i in range(index, index + 6):
        buffer[i] = colour

    for i in range(index + 3000, index + 3006):
        buffer[i] = colour


def drawline(x1, y1, x2, y2):
    length = math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))
    vector = ((x2 - x1) / length, (y2 - y1) / length)

    while True:
        x1 += vector[0]
        y1 += vector[1]
        draw_pixel(x1, y1)
        if math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < 1:
            break


def main():
    if not glfw.init():
        return

    window = glfw.create_window(window_width, window_height, "lab5", None, None)

    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_cursor_pos_callback(window, cursor_pos)
    glfw.set_mouse_button_callback(window, mouse_button)
    glfw.set_key_callback(window, key_callback)
    glClearColor(0, 0, 1, 1)

    while not glfw.window_should_close(window):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glDrawPixels(1000, 1000, GL_RGB, GL_UNSIGNED_BYTE, buffer)
        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.destroy_window(window)
    glfw.terminate()


main()
