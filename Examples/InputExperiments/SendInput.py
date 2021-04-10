import ctypes
import win32con
import string

# This is from https://stackoverflow.com/questions/11906925/python-simulate-keydown
# No need to reinvent the wheel, so I will keep it (mostly) as is
#
# The stackoverflow post had a bunch of constants defined, but I have removed them
# MOST of the constants were already specified in win32con, but it did have a few extra
# If for some reason those codes are needed, I think I'll know what I need though.

LONG = ctypes.c_long
DWORD = ctypes.c_ulong
ULONG_PTR = ctypes.POINTER(DWORD)
WORD = ctypes.c_ushort

class MOUSEINPUT(ctypes.Structure):
    _fields_ = (('dx', LONG),
                ('dy', LONG),
                ('mouseData', DWORD),
                ('dwFlags', DWORD),
                ('time', DWORD),
                ('dwExtraInfo', ULONG_PTR))

class KEYBDINPUT(ctypes.Structure):
    _fields_ = (('wVk', WORD),
                ('wScan', WORD),
                ('dwFlags', DWORD),
                ('time', DWORD),
                ('dwExtraInfo', ULONG_PTR))

class HARDWAREINPUT(ctypes.Structure):
    _fields_ = (('uMsg', DWORD),
                ('wParamL', WORD),
                ('wParamH', WORD))

class _INPUTunion(ctypes.Union):
    _fields_ = (('mi', MOUSEINPUT),
                ('ki', KEYBDINPUT),
                ('hi', HARDWAREINPUT))

class INPUT(ctypes.Structure):
    _fields_ = (('type', DWORD),
                ('union', _INPUTunion))



INPUT_MOUSE = 0
INPUT_KEYBOARD = 1
INPUT_HARDWARE = 2

def Input(structure):
    if isinstance(structure, MOUSEINPUT):
        return INPUT(INPUT_MOUSE, _INPUTunion(mi=structure))
    if isinstance(structure, KEYBDINPUT):
        return INPUT(INPUT_KEYBOARD, _INPUTunion(ki=structure))
    if isinstance(structure, HARDWAREINPUT):
        return INPUT(INPUT_HARDWARE, _INPUTunion(hi=structure))
    raise TypeError('Cannot create INPUT structure!')

def MouseInput(flags, x, y, data):
    return MOUSEINPUT(x, y, data, flags, 0, None)

def KeybdInput(code, flags):
    return KEYBDINPUT(code, code, flags, 0, None)

def HardwareInput(message, parameter):
    return HARDWAREINPUT(message & 0xFFFFFFFF,
                         parameter & 0xFFFF,
                         parameter >> 16 & 0xFFFF)

def Mouse(flags, x=0, y=0, data=0):
    return Input(MouseInput(flags, x, y, data))

def Keyboard(code, flags=0):
    return Input(KeybdInput(code, flags))

def Hardware(message, parameter=0):
    return Input(HardwareInput(message, parameter))

################################################################################


UPPER = frozenset('~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:"ZXCVBNM<>?')
LOWER = frozenset("`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./")
ORDER = string.ascii_letters + string.digits + ' \b\r\t'
ALTER = dict(zip('!@#$%^&*()', '1234567890'))

def keyboard_stream(string):
    mode = False
    for character in string.replace('\r\n', '\r').replace('\n', '\r'):
        if mode and character in LOWER or not mode and character in UPPER:
            yield Keyboard(win32con.VK_SHIFT, mode and win32con.KEYEVENTF_KEYUP)
            mode = not mode
        character = ALTER.get(character, character)
        if character in ORDER:
            code = ord(character.upper())
        else:
            continue
            raise ValueError('String is not understood!')
        yield Keyboard(code)
        yield Keyboard(code, win32con.KEYEVENTF_KEYUP)
    if mode:
        yield Keyboard(win32con.VK_SHIFT, win32con.KEYEVENTF_KEYUP)


# def delayed_press(character):
#     character = ALTER.get(character, character)
#     if character in ORDER:
#         code = ord(character.upper())
#     else:
#         continue
#         raise ValueError('String is not understood!')

#     yield Keyboard(win32con.VK_SHIFT)

def SendInput(*inputs):
    """
    Inputs the provided inputs into the event buffer

    inputs  : The inputs to put into the input stream
    """
    nInputs = len(inputs)
    LPINPUT = INPUT * nInputs
    pInputs = LPINPUT(*inputs)
    cbSize = ctypes.c_int(ctypes.sizeof(INPUT))
    return ctypes.windll.user32.SendInput(nInputs, pInputs, cbSize)


def CreateMouseInputs(flags, locs):
    """
    Creates mouse inputs for each location using the specified flags

    flags   : Flag(s) to set in events
    locs    : (x,y) pairs to use in events
    """
    return [Mouse(flags, *loc) for loc in locs]


if __name__ == '__main__':
    import time
    time.sleep(20)
    while True:
        for input_ in keyboard_stream('A'):
            time.sleep(3)
            SendInput(input_)