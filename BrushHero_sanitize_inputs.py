import bluetooth
from pynput.keyboard import Key, Controller
from brushhero_utils import get_state, BIT_KEY_MAP
import sys
import numpy as np
from typing import List



def printStatus(device_status: List[int]):
    """
    Prints an annotated version of the device status
    """
    print(f"S: {device_status[0]}   G: {device_status[1]}   R: {device_status[2]}   " +
          f"Y: {device_status[3]}   B:{device_status[4]}   O: {device_status[5]}")


if __name__ == '__main__':
    # This is the MAC address of our bluetooth device
    # Ideally we would like a better means of finding the device we want,
    # but that wasn't our focus for this project
    BT_ADDR = '98:D3:C1:FD:B9:07'
    PORT = 1

    # Device discovery
    print("Scanning for BT connections")
    devices = bluetooth.discover_devices()
    if BT_ADDR not in devices:
        print(f"Could not find BT device {BT_ADDR}")
        print("The program will now exit\n")
        sys.exit(1)
    print("BT device found!\n")

    # Device connection
    print("Connecting to BT device")
    try:
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((BT_ADDR, PORT))
    except Exception:
        print("Failed to connect to the device")
        print("Make sure the device is already paired")
        print("The program will now exit\n")
        sys.exit(1)
    print("BT device connected!\n")

    keyboard = Controller()

    curr_state = np.array( # Holds received state of input
        [0,0,0,0,0,0],
        dtype=np.bool,
    )            
    activated = np.array(  # 1 if recently pressed
        [0,0,0,0,0,0],
        dtype=np.bool,
    )
    deactivated = np.array( # 1 if recently deactivated
        [0,0,0,0,0,0],
        dtype=np.bool,
    )
    deactivated_count = np.array([0,0,0,0,0,0])
    deactivated_threshold = np.array([2,4,4,4,4,4])
    pressed = np.array([0,0,0,0,0,0], dtype=np.bool)

    t = True

    # Main loop
    while True:
        # Get binary data from BT device
        data = sock.recv(1)
        curr_state = get_state(ord(data))

        # Consider past values to prevent noisy signals
        # Sometimes the values for buttons would drop for a message or two
        #
        # This algorithm helps prevent that by keeping track of which buttons were
        # recently released. The corresponding key is actually only released after 
        # its signal has not been activated for a specified amount of time
        #
        deactivated |= ~curr_state & activated # Add recently deactivated
        deactivated &= ~curr_state             # Remove recently activated

        deactivated_count += deactivated & (~curr_state & activated) # Increment deactivated
        deactivated_count *= ~curr_state # Stop counting if button was pressed

        activated |= curr_state # Update activated

        # Mark old inputs as deactivated
        activated &= ~(deactivated_count >= deactivated_threshold)

        # Unmark as deactivated if threshold surpassed
        deactivated &= ~(deactivated_count >= deactivated_threshold)
        # Clear count on buttons that aren't deactivated
        deactivated_count *= deactivated

        # Get input to send to
        input_to_send = curr_state | (deactivated & (deactivated_count < deactivated_threshold))

        printStatus(input_to_send)
        for idx, val in enumerate(input_to_send):
            if pressed[idx] and not val:
                # Button has become deactivated
                keyboard.release(BIT_KEY_MAP[idx])
                pressed[idx] = False
            if not pressed[idx] and val:
                # Button has become activated
                keyboard.press(BIT_KEY_MAP[idx])
                pressed[idx] = True

