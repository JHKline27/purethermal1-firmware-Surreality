# Read serial data from PureThermal1 running Lepton3 firmware
# format: SYNC(4 bytes) + FRAME(160*120*2 bytes)
# Each pixel is 16 bits, but only 14 bits are used (0-16383)
# Sync word is 0xDEADBEEF (from uart_task.c)
# Store frames as numpy arrays of uint16, then reshape to 120x160
# Once a frame is read, it is converted to 8-bit and displayed using OpenCV

import serial, numpy as np, cv2

# ==== CONFIG ====
PORT = ""  #Dont really know yet
BAUD = 921600        # found in project config as "USART_DEBUG_SPEED", but the one set in main.c is 115200 
WIDTH, HEIGHT = 160, 120   
SYNC_WORD = b'\xDE\xAD\xBE\xEF'  #from uart_task.c

# =================
FRAME_SIZE = WIDTH * HEIGHT * 2  # 2 bytes per pixel (16-bit)
ser = serial.Serial(PORT, BAUD, timeout=1)

print(f"Listening on {PORT} at {BAUD} baud...")

def find_sync():
    """Scan serial stream until sync word is found."""
    buffer = b''
    while True:
        buffer += ser.read(1)
        if buffer.endswith(SYNC_WORD):
            return

while True:
    # Wait for sync word
    find_sync()

    # Read one frame
    raw = ser.read(FRAME_SIZE)
    if len(raw) != FRAME_SIZE:
        print("Frame incomplete, resyncing...")
        continue

    # Convert to numpy array (16-bit pixels)
    frame16 = np.frombuffer(raw, dtype=np.uint16).reshape((HEIGHT, WIDTH))

    # Convert 14-bit range -> 8-bit for OpenCV
    frame8 = cv2.convertScaleAbs(frame16, alpha=(255.0/16383.0))

    # Apply thermal colormap (for fun)
    colored = cv2.applyColorMap(frame8, cv2.COLORMAP_INFERNO)

    # Show both
    cv2.imshow("Raw14", frame8)
    cv2.imshow("Thermal", colored)

    if cv2.waitKey(1) == 27:  # ESC to quit
        break

ser.close()
cv2.destroyAllWindows()
