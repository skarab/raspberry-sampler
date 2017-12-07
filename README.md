# raspberry-sampler
Sampler based on Raspberry Pi 3 B

It's a sampler with bunch of knobs, dedicated to live-edit samples stored in a usb key. It's easily linked to other instruments using usb-midi.

-----------------------------------

Features

- 8 knobs, 4 buttons, 1 play button, 1 parameters switch, 3 leds, mini jack output
- stereo & polyphonic, up to 8 voices
- 44100Hz 16bits realtime playback & editing
- WAV support through the usb key, with live-plug support
- usb midi input, supporting multiple devices
- one shot, loop and instrument play modes
- per-sample stereo amplifier, formant, distortion & bitcrusher filters
- global equalizer, low pass moog, high pass, resonance & noise filters

-----------------------------------

Links

http://www.kstorm.org

https://www.youtube.com/watch?v=qFAxQr971z0

https://www.youtube.com/watch?v=87chFwOwE38

-----------------------------------

Manual


![alt text](https://raw.githubusercontent.com/skarab/raspberry-sampler/master/schema.png)

(0) display current edited value

Bank :

(1) select bank, basically a folder full of wavs in the main samples/ directory

    The bank 0 is the global bank : first sample contains the global parameters, main volume, filters, ... next samples are the current used samples (attached to midi), note this list is updated when switching bank so you can detach/re-attach used samples.

(2) lights up if bank is loaded

(3) load or unload the bank - if on global bank : stop all samples

(4) save the bank (all parameters & midi settings) - if on global bank : detach all samples


Sample :

(5) select sample, a 16bits 44100Hz WAV file inside the bank folder

(6) switch mode :

   0. one shot   : just play it til the end (default)
   1. loop       : one press to start the loop, one press to stop it
   2. instrument : use whole midi device, pitch linked to notes

(7) midi attach/detach :
   - tap it one time, then the next received midi key is attached.
     Note: a sample has a single attachment.
     If you switch another control before a note is received, it just cancel the tap.
   - tap it two time to detach it

    The green LED displays if the sample is attached.

(8) play the sample, the red LED displays if the sample is playing

(9) select parameters to assign to the (10) knobs :

 Global params:

    0.  Volume        Pan            Stereo          EQ Low          EQ Medium         EQ High
    1.  LP CutOff     LP Resonance   HP CutOff       HP Resonance    Dry/Wet           Resonance
    2.  Noise         Noise Texture  Clicks          Clicks Texture  Clicks Pitch      Noise Mixer

 Sample params:

    0.  Volume        Pan            Stereo          Formant         Distortion        BitCrusher
    1.  Start         Env Attack     Env Release     Stop            Pitch Semi-Tone   Pitch Fine-Tune
    2.  Loop Start    Loop Attack    Loop Release    Loop Stop       Silence           Legato


-----------------------------------

Hardware parts :
 - Raspberry 3 B
 - SD card
 - 7-segment LED HT16K33 Backpack
 - x8 rotary encoder (Electronics AB 9302540010), remove the guts for the parameters knobs (smoother)
 - 1 switch - 3 positions
 - 3 led
 - 4 buttons
 - 1 play button
 - cables, proto board, cobbler

-----------------------------------

USB Key

It should contains a root folder named samples, and some folders (banks) containing the 44100Hz 16bits wav files.

-----------------------------------

SD Card

It requires a 16GB micro SDHC card, the image is HERE. Just put it using kind of :

dd bs=4M if=backup.img of=/dev/sdb

It's a gentoo custom kernel, root filesystem is mounted as read-only, so no worry about brutals/usuals power off.

-----------------------------------

Manual build

I build it using custom gentoo kernel, using openrc & alsalibs, external usb soundcard/midi enabled.
It requires bcm2835 libs (emerge -a bcm2835), dont forget to pull submodules .
ssh, samba are installed on the image.
