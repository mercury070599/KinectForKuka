// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Kinect.h>
#include <conio.h>


//hardcoded image height and depth values
#define Img_W 512
#define Img_H 424

typedef short int int16;

IKinectSensor *Kinect;
IDepthFrameSource *DepthSource;
IDepthFrameReader *DepthReader;
IDepthFrame *DepthFrame;
int check(HRESULT hr);
int check(HRESULT hr) {
	if (FAILED(hr)) {
		printf("%s", "Problem with Sensor");
		_getch();
		exit(10);
	}
	return 0;
}

int main()
{
	printf("%c", 56);
	UINT16 *depthbuffer = new UINT16(512 * 424);
	HRESULT hr=GetDefaultKinectSensor(&Kinect); // Pointer reference to kinect to find any kinect device conected 
	if (FAILED(hr)) {
		printf("%s", "Problem with Sensor Recog");
		_getch();
		exit(10);
	}
	Kinect->Open();//opens the sensor
	hr=Kinect->get_DepthFrameSource(&DepthSource);
	if (FAILED(hr)) {
		printf("%s", "Problem with Sensor FrameSource");
		_getch();
		exit(10);
	}
	hr = DepthSource->OpenReader(&DepthReader);
	if (FAILED(hr)) {
		printf("%s", "Problem with Sensor Reader");
		_getch();
		exit(10);
	}
	hr = DepthReader->AcquireLatestFrame(&DepthFrame);
	if (FAILED(hr)) {
		printf("%s", "Problem with Sensor DepthFrame");
		_getch();
		exit(10);
	}
	hr = DepthFrame->CopyFrameDataToArray(512 * 424, depthbuffer);
	if (FAILED(hr)) {
		printf("%s", "Problem with Sensor DePTHbUFFER");
		_getch();
		exit(10);
	}
	_getch();
    return 0;
}

