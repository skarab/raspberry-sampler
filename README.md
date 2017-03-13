# raspberry-sampler
Sampler for the Raspberry 2 & 3

Key concept is to have hardware knobs to live-edit sample,
envelop, start-end / looping points, filtering a bit...

Features:
- store samples from network connection (shared samba folder)
- handle multiple banks (folders)
- usb midi input
- attach a sample to a midi note (channel & note, optionnaly velocity may be defined in the xml)
- load/save bank parameters to xml
- live edit the sample with knobs
- 7-bars screen displaying edited values and infos (IP, state, ...)
- raspberry is running on gentoo, i'll provide the SD card image and schematics
