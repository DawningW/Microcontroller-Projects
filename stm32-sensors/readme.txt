使用stm32从各种传感器上读取数据

基于中景园STM32F407ZGT6开发板, 但使用了HAL库, 因此应该能够容易地移植到其他stm32单片机上

连线:
DHT DATA B3
QMC5883 I2C1 (SCL B6 SDA B7)
MPU6500 I2C2 (SCL F1 SDA F0)

上位机程序使用Python+OpenGL开发, 请见Monitor目录

用到的第三方库:
PySerial
PIL
PyGLM
glfw
PyOpenGL

PyOpenGL请使用这里的, pip中的不包含第三方DLL: https://www.lfd.uci.edu/~gohlke/pythonlibs/#pyopengl
