import time
import os
import sys
from Adafruit_LED_Backpack import SevenSegment

display = SevenSegment.SevenSegment()
display.begin()

for line in sys.stdin:
    display.clear()
    display.print_number_str(line[:-1])
    display.write_display()
