# KSamp
Hardware audio sampler based on Raspberry Pi 3 B, running on linux

It's a sampler with bunch of knobs, dedicated to live-edit samples stored in a usb key. It's easily linked to other instruments using usb-midi.

-----------------------------------

Features

- 8 knobs, 5 buttons, 1 parameters 3P switch, 3 leds, mini jack output
- stereo & polyphonic, up to 8 voices
- 44100Hz 16bits realtime playback & editing
- WAV support through the usb key, with live-plug support
- usb midi input, supporting multiple devices
- one shot, loop and instrument play modes
- per-sample time & loop selection, stereo amplifier, formant, distortion & bitcrusher filters
- global equalizer, low pass moog, high pass, resonance & noises filters

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

    The LED displays if the sample is attached.

(8) play the sample, the LED displays if the sample is playing

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
 - Raspberry 3 Model B V1.2
 - 16GB micro SD card HC
 - USB key to store samples
 - noise free USB audio card PCM2704
 - Adafruit T-Cobbler+ (for soldering safety)
 - 7-segment LED HT16K33 Backpack
 - x8 rotary encoder (Electronics AB 9302540010), remove the guts on the parameters knobs (smoother)
 - 1 switch - 3 positions
 - 3 led
 - 5 buttons
 - cables, tools, ...

-----------------------------------

USB Key

It should contains a root folder named samples, and some folders (banks) containing the 44100Hz 16bits wav files.
I use FAT32.

-----------------------------------

SD Card

It requires a 16GB micro SDHC card, the image is here : https://downloads.kstorm.org/ksamp.img
Just put it using kind of :

dd bs=4M if=ksamp.img of=/dev/sdb

root filesystem is mounted as read-only, so no worry about brutals/usuals power off.

-----------------------------------

Developer Manual

You can either modify things right on the raspberry (textmode only) or use your laptop with code::blocks via SSH & Samba.
It's based on a custom Gentoo kernel, using openrc, alsalibs, bcm2835, ...
(note: you may use windows since we build on the Pi using SSH...)

Steps :

1. mount the sdcard on your laptop :
 mount /dev/sdx3 /mnt/usb/
 vi /etc/fstab
remove the ",ro" flag to remove the read-only protection (don't forget to add it after changes !)

2. insert the sdcard in the pi
plug the network cable on your box
connect alim, power on

3. grab IP, either connecting to TV or from your box (should bind to 192.168.1.45 but not sure)
connect using ssh :
 ssh -l root 192.168.1.45
 password: sampler

4. stop sampler running
 mv .bash_profile .bash_profile.old
 reboot
 reconnect ssh

5. mount sampler on your latop using samba :
mount //192.168.1.45/sampler sampler/ -o guest

-> then you have sampler the binary, sources/ containing src & code::blocks project so you can modify, and build/run from the ssh window.
git is installed so you may push things.

6. when finished :
 add the ",ro" flag in /etc/fstab
 mv .bash_profile.old .bash_profile
 reboot
