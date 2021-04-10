import bluetooth


PORT = 1
hc06 = bluetooth.discover_devices()[0] # Assumes only one discoverable device

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
service = bluetooth.find_service(address=hc06) # Returns a blank list for now

sock.connect((hc06, PORT))
sock.send(b'Input your bytestring here')
