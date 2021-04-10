from pynput.keyboard import Key, Controller
import numpy as np

# BIT_KEY_MAP = {
#     0: Key.up,
#     1: 'a',
#     2: 's',
#     3: 'd',
#     4: 'f',
#     5: 'g'
# }

BIT_KEY_MAP = {
    0: '0',
    1: '1',
    2: '2',
    3: '3',
    4: '4',
    5: '5',
}

class BITS:
    strum  = 0b00000001
    green  = 0b00000010
    red    = 0b00000100
    yellow = 0b00001000
    blue   = 0b00010000
    orange = 0b00100000

def get_state(bt_data: int):
    """
    Returns a list of binary states for various buttons
    """
    strum = (bt_data & BITS.strum) // (BITS.strum)
    green = (bt_data & BITS.green) // (BITS.green)
    red = (bt_data & BITS.red) // (BITS.red)
    yellow = (bt_data & BITS.yellow) // (BITS.yellow)
    blue = (bt_data & BITS.blue) // (BITS.blue)
    orange = (bt_data & BITS.orange) // (BITS.orange)
    return np.array([strum, green, red, yellow, blue, orange], dtype=np.bool)