@echo off

REM convert WAV files to headerless versions
echo ---------------------------------------------------------------------
echo Stripping WAV file headers
echo ---------------------------------------------------------------------
FOR %%F IN (xm\samples\*.wav) DO util\riffstrip.exe %%F

REM convert headerless WAV files to EWF format
echo ---------------------------------------------------------------------
echo Converting samples to EWF
echo ---------------------------------------------------------------------
util\pcm2ewf.exe xm\samples\snare1.wav.stripped res\ewf\snare1.ewf
util\pcm2ewf.exe xm\samples\bassdrum1.wav.stripped res\ewf\bassdrum1.ewf

REM convert VGI instruments to EIF instruments
echo ---------------------------------------------------------------------
echo Converting VGI patches to EIF
echo ---------------------------------------------------------------------
util\vgi2eif.exe xm\inst\bass1.vgi res\eif\bass1.eif
util\vgi2eif.exe xm\inst\sqchords1.vgi res\eif\sqchords1.eif
util\vgi2eif.exe xm\inst\sawlead.vgi res\eif\sawlead.eif

REM clean up
echo ---------------------------------------------------------------------
echo Cleaning up...
echo ---------------------------------------------------------------------
del xm\samples\snare1.wav.stripped
del xm\samples\bassdrum1.wav.stripped

REM Now for the actual arrangements
echo ---------------------------------------------------------------------
echo Beginning xm2esf conversions
echo ---------------------------------------------------------------------
cd util
xm2esf ..\xm\bgm1\bgm1.xif ..\res\bgm\bgm1.esf
pause 