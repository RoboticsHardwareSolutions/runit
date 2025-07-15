import sys, os

import pylink
from pylink import JLink
import time
import re
from dataclasses import dataclass


def remove_ansi_colors(text: str) -> str:
    """Remove ANSI color codes from text."""
    return re.sub(r"\x1b\[[0-9;]*m", "", text)


def run_tests_by_rtt(jlink: JLink, duration: float = 0.0) -> None:
    has_error = False
    try:
        jlink.rtt_start()
        start_time = time.time()
        while True:
            elapsed = time.time() - start_time
            if duration > 0.0 and elapsed >= duration:
                break
            response = jlink.rtt_read(0, 1024)
            if response:
                resp_text = remove_ansi_colors(bytes(response).decode("utf-8", errors="ignore"))
                # Extract report info
                match = re.search(
                    r'REPORT\s*\|\s*File:\s*(.*?)\s*\|\s*Test case:\s*(.*?)\s*\|\s*Passes:\s*(\d+)\s*\|\s*Failures:\s*(\d+)',
                    resp_text
                )
                if match:
                    file_path = match.group(1)
                    test_case = match.group(2)
                    passed = match.group(3)
                    failed = match.group(4)
                    print(f"Test result: {passed} passed, {failed} failed (File: {file_path}, Test case: {test_case})")
                    if failed != '0':
                        has_error = True
                elif "All tests passed successfully!" in resp_text:
                    has_error = False
                    print("All tests passed successfully!")
                else:
                    exp_fail_match = re.search(r"Expected (\d+) failures, but got (\d+)", resp_text)
                    if exp_fail_match:
                        expected = exp_fail_match.group(1)
                        got = exp_fail_match.group(2)
                        print(f"Expected {expected} failures, but got {got}")
                        has_error = True
                    else:
                        print(f"::error::Output:\n{resp_text}")
                        has_error = True

    except Exception as e:
        print(f"Error RTT: {e}")
    finally:
        jlink.rtt_stop()
    return has_error

def rtt_device_by_usb(jlink_serial: int, mcu: str) -> None:
    jlink = pylink.JLink()
    jlink.open(serial_no=jlink_serial)

    if jlink.opened():
        jlink.set_tif(pylink.enums.JLinkInterfaces.SWD)
        jlink.connect(mcu)
        has_error = run_tests_by_rtt(jlink, 10.0)

    jlink.close()

    return has_error

def get_args():
    if len(sys.argv) < 3 or not sys.argv[1].strip() or not sys.argv[2].strip():
        raise ValueError("Usage: python units.py <jlink_serial> <mcu>")
    jlink_serial = int(sys.argv[1].strip())
    mcu = sys.argv[2].strip()
    return jlink_serial, mcu

def main():
    try:
        jlink_serial, mcu = get_args()
        if rtt_device_by_usb(jlink_serial, mcu):
            sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
