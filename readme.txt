Wrapper program to fix MKVTOMP4 issues with E-AC-3 audio

mkvtomp4 uses eac3to to convert a .wav file extracted by ffmpeg into an .ac3
file - except the extension for some files is .E-AC-3. This alternate extension
caused eac3to fail. 

eac3-wrapper temporarily changes the .E-AC-3 extension to .ac3 and invokes
eac3to with the temporary filename. A .exe file is required to do this as the
.exe is invoked explicitly by mkvtomp4 so an overriding batch file doesn't work.

Great, it worked, with eac3-wrapper in place I am able to get a result file with
the .E-AC-3 extension created by eac3to.

Now it appears that the output from eac3to is given to 'qaac' and it fails with
"ERROR: Not available input file format" so I guess it uses the extension to 
determine the file type rather than actually examining the content.

So 'qaac' also needs to be wrapped!


Started as an Eclipse CDT (Photon) project using cygwin64 and gcc. The target machine does
not have space to install cygwin64, plus the system command appears to start a cygwin shell
which might cause all sorts of issues, especially bearing in mind the idiot behaviour of
cygwin with respect to windows pathnames! The last Visual Studio I had installed on my
home system was VS2005 - a bit out of date - and since there is now a free version of
Visual Studio I thought I'd give it a go. Took ages to actually get the environment
installed - who would have thought that selecting the desktop C++ 'workflow' would not
automatically install the C/C++ include files and libraries!!!! However the code compiled
and linked first time! It ran on the target system without needing any redist to be
installed.

Deploy (rename the real binaries to xxx_wrpd.exe first!!)
copy /Y "E:\Development\workspace\c-cpp\eac3-wrapper\Release\eac3-wrapper.exe" "E:\Development\utils\MkvToMp4_0.224\Tools\eac3to\eac3to.exe"
copy /Y "E:\Development\workspace\c-cpp\eac3-wrapper\Release\eac3-wrapper.exe" "E:\Development\utils\MkvToMp4_0.224\Tools\qaac\qaac.exe"

Examples of the mkvtomp4 command lines
Using arapper
..\Tools\eac3to\eac3to  "C:\Temp\test_audio_2.wav" "C:\Temp\test_audio_2_new.E-AC-3" -100ms  -progressnumbers
..\Tools\qaac\qaac  --quality 2 --ignorelength --tvbr 127 "C:/Temp/True Detective 3x08 Now Am Found_internal_audio_2_new.E-AC-3" -o "C:/Temp/True Detective 3x08 Now Am Found_internal_audio_2_new.m4a"

Not using wrapper
..\Tools\eac3to\eac3to_wrpd  "C:\Temp\test_audio_2.wav" "C:\Temp\test_internal_audio_2_new.E-AC-3.flac" -100ms  -progressnumbers
..\Tools\qaac\qaac_wrpd  --quality 2 --ignorelength --tvbr 127 "C:/Temp/test_internal_audio_2_new.E-AC-3.flac" -o "C:/Temp/test_internal_audio_2_new.m4a"

