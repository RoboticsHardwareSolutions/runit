import sys, os

import pylink
from pylink import JLink

def flash_device_by_usb(jlink_serial: int, fw_file: str, mcu: str) -> None:
    jlink = pylink.JLink()
    jlink.open(serial_no=jlink_serial)

    if jlink.opened():
        jlink.set_tif(pylink.enums.JLinkInterfaces.SWD)
        jlink.connect(mcu)
        print(jlink.flash_file(fw_file, 0x08000000))
        jlink.reset(halt=False)

    jlink.close()

def get_args():
    if len(sys.argv) < 4 or not sys.argv[1].strip() or not sys.argv[2].strip() or not sys.argv[3].strip():
        raise ValueError("Usage: python flashing.py <jlink_serial> <mcu> <firmware_file>")
    jlink_serial = int(sys.argv[1].strip())
    mcu = sys.argv[2].strip()
    fw_file = os.path.abspath(sys.argv[3].strip())
    return jlink_serial, mcu, fw_file

def main():
    try:
        jlink_serial, mcu, fw_file = get_args()
        flash_device_by_usb(jlink_serial, fw_file, mcu)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
