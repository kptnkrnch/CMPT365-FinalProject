Steps to Compile and Run:
Due to Size Limitations, in order to compile the program, you will need to download some .lib files for OpenCV 3.0
Run Steps:
	1. Next, go into the supplied Dlls folder and copy these dll files into the Source/CMPT365-A2 folder
	2. Run the exe for the program
	3. Click the Open Video button to open a video
	4. Once a video has been opened, press the play button in order to play the video as a sound.
	5. To stop the video, press the “Stop” button.
	
Compiling Steps:
	1. go to http://opencv.org/downloads.html and download OpenCV Version 3.0 for Windows
	2. run the exe file to extract the contents
	3. once extracted, go into the opencv/build/x86/vc11/lib folder and copy these files into the Project's Source/CMPT365-A2/lib folder:
		opencv_ts300.lib
		opencv_ts300d.lib
		opencv_world300.lib
		opencv_world300d.lib
	4. next, go into the opencv/build/x86/vc11/staticlib folder and copy these files into the Project's Source/CMPT365-A2/lib folder:
		opencv_core300.lib
		opencv_core300d.lib
		opencv_imgproc300.lib
		opencv_imgproc300d.lib
		opencv_highgui300.lib
		opencv_highgui300d.lib
		opencv_ml300.lib
		opencv_ml300d.lib
		opencv_video300.lib
		opencv_video300d.lib
		opencv_features2d300.lib
		opencv_features2d300d.lib
		opencv_calib3d300.lib
		opencv_calib3d300d.lib
		opencv_objdetect300.lib
		opencv_objdetect300d.lib
		opencv_flann300.lib
		opencv_flann300d.lib
	5. In Visual Studio 2012, right click on the CMPT365-A2 project from the solution explorer and click on "properties"
	6. Under the properties window, go to the linker section and edit the "Additional Library Directories" section to include the Project's Source/CMPT365-A2/lib folder
	6. Next, go into the supplied Dlls folder and copy these dll files into the Source/CMPT365-A2 folder
	7. Finally, run the program in Debug mode to compile and run the application
	8. See "Run Steps" 3 through 5 for how to operate the program
