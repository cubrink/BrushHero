from pynput.keyboard import Key, Controller
import time

if __name__ == '__main__':
    keyboard = Controller()
    time.sleep(15)

    while True:
        time.sleep(1)
        keyboard.press('1')
        time.sleep(1)
        keyboard.release('1')

