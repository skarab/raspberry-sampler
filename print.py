import sys
from Adafruit_LED_Backpack import SevenSegment

display = SevenSegment.SevenSegment()
display.begin()
display.clear()
if len(sys.argv)==2:    
    display.print_number_str(sys.argv[1])
display.write_display()
