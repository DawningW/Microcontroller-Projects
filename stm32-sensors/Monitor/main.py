#!/usr/bin/env python3
# coding=utf-8

import sys
import time
from threading import Thread
import serial
import serial.tools.list_ports
from serial.serialutil import SerialException
from PIL import Image
import glm
from OpenGL.GL import *
from OpenGL.GL.shaders import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import glfw

FPS = 30
vertexShaderSource = """
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoordIn;
out vec2 texCoord;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    texCoord = vec2(texCoordIn.x, 1.0f - texCoordIn.y);
}
"""
fragmentShaderSource = """
#version 330 core
in vec2 texCoord;
out vec4 color;
uniform sampler2D myTexture;
void main()
{
    // color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    color = texture(myTexture, texCoord);
}
"""

cube = None
dataText = None
device = None
receive_thread = None
temperature = 0.0
humidity = 0.0

def eulerToRotationMatrix(roll, pitch, yaw):
    """
    ZXY顺规, Z-Roll, X-Pitch, Y-Yaw, 单位角度
    参考: https://zhuanlan.zhihu.com/p/45404840
    """
    c1 = glm.cos(glm.radians(yaw))
    s1 = glm.sin(glm.radians(yaw))
    c2 = glm.cos(glm.radians(pitch))
    s2 = glm.sin(glm.radians(pitch))
    c3 = glm.cos(glm.radians(roll))
    s3 = glm.sin(glm.radians(roll))
    matrix = glm.mat4(c1*c3+s1*s2*s3, c3*s1*s2-c1*s3, c2*s1, 0,
                      c2*s3, c2*c3, -s2, 0,
                      c1*s2*s3-s1*c3, s1*s3+c1*c3*s2, c1*c2, 0,
                      0, 0, 0, 1)
    return matrix

class Cube(object):
    def __init__(self, a = 1.0, b = 1.0, c = 1.0):
        # 编译着色器
        self.shaderProgram = compileProgram(
            compileShader(vertexShaderSource, GL_VERTEX_SHADER),
            compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER)
        )
        self.transformIndex = glGetUniformLocation(self.shaderProgram, "transform")
        # self.textureIndex = glGetUniformLocation(self.shaderProgram, "myTexture")
        # 链接顶点属性
        a2, b2, c2 = a / 2.0, b / 2.0, c / 2.0
        vertices = [
            -a2, -c2, -b2, 0.0, 0.0,
            a2, -c2, -b2, 1.0, 0.0,
             a2, c2, -b2, 1.0, 1.0,
             a2, c2, -b2, 1.0, 1.0,
             -a2, c2, -b2, 0.0, 1.0,
             -a2, -c2, -b2, 0.0, 0.0,
             
             -a2, -c2, b2, 0.0, 0.0,
             a2, -c2, b2, 1.0, 0.0,
             a2, c2, b2, 1.0, 1.0,
             a2, c2, b2, 1.0, 1.0,
             -a2, c2, b2, 0.0, 1.0,
             -a2, -c2, b2, 0.0, 0.0,

             -a2, c2, b2, 1.0, 0.0,
             -a2, c2, -b2, 1.0, 1.0,
             -a2, -c2, -b2, 0.0, 1.0,
             -a2, -c2, -b2, 0.0, 1.0,
             -a2, -c2, b2, 0.0, 0.0,
             -a2, c2, b2, 1.0, 0.0,

             a2, c2, b2, 1.0, 0.0,
             a2, c2, -b2, 1.0, 1.0,
             a2, -c2, -b2, 0.0, 1.0,
             a2, -c2, -b2, 0.0, 1.0,
             a2, -c2, b2, 0.0, 0.0,
             a2, c2, b2, 1.0, 0.0,

             -a2, -c2, -b2, 0.0, 1.0,
             a2, -c2, -b2, 1.0, 1.0,
             a2, -c2, b2, 1.0, 0.0,
             a2, -c2, b2, 1.0, 0.0,
             -a2, -c2, b2, 0.0, 0.0,
             -a2, -c2, -b2, 0.0, 1.0,

             -a2, c2, -b2, 0.0, 1.0,
             a2, c2,-b2, 1.0, 1.0,
             a2, c2, b2, 1.0, 0.0,
             a2, c2, b2, 1.0, 0.0,
             -a2, c2, b2, 0.0, 0.0,
             -a2, c2,-b2, 0.0, 1.0
        ]
        # vertexData = glm.array([glm.float32(vertex) for vertex in vertices])
        vertexData = glm.array(glm.float32, *vertices)
        vertexBuffer = glGenBuffers(1) # VBO 顶点缓冲数据
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer)
        glBufferData(GL_ARRAY_BUFFER, vertexData.nbytes, vertexData.ptr, GL_STATIC_DRAW) # 只有array有这个nbytes和ptr
        self.vertexArray = glGenVertexArrays(1) # VAO 顶点数组对象
        glBindVertexArray(self.vertexArray)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), ctypes.c_void_p(0)) # 最后一个参数必须强制转换为void*
        # https://github.com/mcfletch/pyopengl/issues/3
        glEnableVertexAttribArray(0)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), ctypes.c_void_p(3 * sizeof(GLfloat)))
        glEnableVertexAttribArray(1)
        glBindVertexArray(0) # 用完别忘解绑
        # 设置纹理贴图
        image = Image.open("box.png")
        width, height = image.size
        raw_image = image.tobytes("raw", "RGBA")
        self.texture = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, self.texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) # 重复边缘
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) # 临近插值
        # glPixelStorei(GL_UNPACK_ALIGNMENT, 1) # 默认4字节对齐, 可以改成1或者用RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_image)
        # glPixelStorei(GL_UNPACK_ALIGNMENT, 4)
        glBindTexture(GL_TEXTURE_2D, 0)
        # 初始化一下变换矩阵
        self.rorate(0, 0, 0)

    def draw(self):
        glEnable(GL_DEPTH_TEST)
        glUseProgram(self.shaderProgram)
        # glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D, self.texture)
        # glUniform1i(self.textureIndex, 0);
        glBindVertexArray(self.vertexArray)
        trans = eulerToRotationMatrix(self.roll, self.pitch, self.yaw)
        glUniformMatrix4fv(self.transformIndex, 1, GL_FALSE, glm.value_ptr(trans))
        glDrawArrays(GL_TRIANGLES, 0, 36)
        glBindVertexArray(0)
        glBindTexture(GL_TEXTURE_2D, 0)
        glUseProgram(0)
        glDisable(GL_DEPTH_TEST)

    def rorate(self, pitch, roll, yaw):
        """
        pitch(x): 俯仰角
        roll (z): 横滚角
        yaw  (y): 偏航角
        此处xyz轴是对于OpenGL所用坐标系(右手系)来说的
        """
        self.pitch = pitch
        self.roll = roll
        self.yaw = yaw

class Text(object):
    def __init__(self):
        self.setText("")

    def draw(self):
        glPushMatrix()
        glColor3f(1.0, 1.0, 1.0)
        width = glGetIntegerv(GL_VIEWPORT)[2]
        textWidth = 0
        for c in self.text:
            textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, ord(c))
        glRasterPos2f(-textWidth / width, 0.8)
        for c in self.text:
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ord(c))
        glPopMatrix()

    def setText(self, text):
        self.text = text

def main():
    global device, receive_thread
    print("Welcome to use stm32-sensors monitor, written by WC.")
    while True:
        ports = list(serial.tools.list_ports.comports())
        if len(ports) == 0:
            input("No available serials, press enter to refresh...")
        else:
            print("Available serials:")
            for i in range(len(ports)):
                print("{}. {}".format(i + 1, ports[i]))
            try:
                num = int(input("Select number: "))
                if num <= 0 or num > len(ports): continue
                device = serial.Serial(ports[num - 1].device, 115200)
                break
            except ValueError:
                continue
    print("Selected {}, start to receive data.".format(device.name))
    receive_thread = Thread(target = receive, daemon = True)
    receive_thread.start()
    # Initialize the library
    glutInit(sys.argv)
    if not glfw.init():
        return
    # Create a windowed mode window and its OpenGL context
    glfw.window_hint(glfw.SAMPLES, 8)
    window = glfw.create_window(600, 600, "stm32 sensors monitor", None, None)
    if not window:
        glfw.terminate()
        return
    # Make the window's context current
    glfw.make_context_current(window)
    # Initialize resources
    init()
    # Loop until the user closes the window
    while not glfw.window_should_close(window) and device:
        t = time.perf_counter()
        # Change viewport to actual size
        width, height = glfw.get_framebuffer_size(window)
        glViewport(0, 0, width, height)
        # Render here, e.g. using pyOpenGL
        render()
        # Swap front and back buffers
        glfw.swap_buffers(window)
        # Poll for and process events
        glfw.poll_events()
        # Wait some time
        wait = 1 / FPS - (time.perf_counter() - t)
        if wait > 0: time.sleep(wait)
    glfw.terminate()

def receive():
    global device
    while device is not None:
        try:
            data = device.readline()[:-2]
            data = data.decode("UTF-8")
            if data[0] == 'A':
                posture = [int(a) for a in data[1:].split(' ')]
                if cube is not None:
                    cube.rorate(posture[0], -posture[1], posture[2]) # 传感器用的是左手坐标系
            elif data[0] == 'T':
                global temperature
                temperature = float(data[1:])
            elif data[0] == 'H':
                global humidity
                humidity = float(data[1:])
                if dataText is not None:
                    dataText.setText("Temperature: %.1f°C     Humidity: %.1f%%" % (temperature, humidity))
            elif data[0] == 'M':
                print("Compass: " + data[1:])
            elif data[0] == 'C':
                print("Acceleration: " + data[1:])
            elif data[0] == 'G':
                print("Gyroscope: " + data[1:])
            else:
                print("Log info:", data)
        except SerialException as error:
            device = None
            print("Serial Error:", error)
            print("Stopping monitor")

def init():
    glEnable(GL_MULTISAMPLE)
    global cube, dataText
    cube = Cube()
    dataText = Text()

def render():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    cube.draw()
    dataText.draw()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit(0)
