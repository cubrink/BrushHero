import bluetooth


PORT = 1
hc06 = bluetooth.discover_devices()[0] # Assumes only one discoverable device

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
service = bluetooth.find_service(address=hc06) # Returns a blank list for now

sock.connect((hc06, PORT))
while True:
    data = sock.recv(1024)
    if len(data) == 0: 
        break
    print(f"received [{data}] (Length = {len(data)})")
