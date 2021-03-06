
//required files included
#include<WinSock2.h>
#include <chrono>

#include <SDL.h>

#include "KinectInit.h"

#include "ImageProcess.h"
#include "Evaluations.cpp"
#include "SDLRender.cpp"
#include "DataSending.h"
#include "ImageRecog.h"
#include "FaceEye.cpp"
#include"DepthAnalysis.h"
#include<map>


using namespace std::chrono;
using namespace std;
typedef steady_clock Clock;

//A test datatype to convert pixel buffer to opencv::MAT
ImageProcess Image;


void assignImage(UINT32 *pixelbuffer) {
	Image.bufferInit(pixelbuffer);
	Image.convert();
}
#undef main


int main()
{
	//initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	//create a window
	SDL_Window* window = SDL_CreateWindow(
		"title", 100, 100, IMG_W, IMG_H, 0);
	if (window == nullptr)
		return 1;

	//create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		return 2;

	//create a texture
	SDL_Texture* texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		IMG_W, IMG_H);
	if (texture == nullptr)
		return 3;

	SDL_Window* window2 = SDL_CreateWindow(
		"title", 100, 100, IMG_W, IMG_H, 0);
	if (window2 == nullptr)
		return 1;

	//create a renderer
	SDL_Renderer* renderer2 = SDL_CreateRenderer(
		window2, -1, SDL_RENDERER_ACCELERATED);
	if (renderer2 == nullptr)
		return 2;

	//create a texture
	SDL_Texture* texture2 = SDL_CreateTexture(
		renderer2, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		IMG_W, IMG_H);
	if (texture2 == nullptr)
		return 3;

	SDL_Window* window3 = SDL_CreateWindow(
		"title", 100, 100, CIMG_W, CIMG_H, 0);
	if (window3 == nullptr)
		return 1;

	//create a renderer
	SDL_Renderer* renderer3 = SDL_CreateRenderer(
		window3, -1, SDL_RENDERER_ACCELERATED);
	if (renderer3 == nullptr)
		return 2;

	//create a texture
	SDL_Texture* texture3 = SDL_CreateTexture(
		renderer3, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		CIMG_W, CIMG_H);
	if (texture3 == nullptr)
		return 3;

	//allocating pixel buffers
	UINT32* pixelBuffer = new UINT32[IMG_W * IMG_H];
	if (pixelBuffer == nullptr)
		return 4;

	UINT32* pixelBuffer2 = new UINT32[IMG_W * IMG_H];
	if (pixelBuffer2 == nullptr)
		return 4;

	UINT32* pixelBuffer3 = new UINT32[CIMG_W * CIMG_H];
	if (pixelBuffer3 == nullptr)
		return 4;
	UINT32* pixelBuffer4 = new UINT32[IMG_W * IMG_H];
	if (pixelBuffer4 == nullptr)
		return 4;


	//Camera space point buffer
	CameraSpacePoint *Coord = new CameraSpacePoint[CIMG_W*CIMG_H];

	//clear the buffers to prevent problems with overflow etc
	memset(pixelBuffer, 0, IMG_W * IMG_H * 4);
	memset(pixelBuffer2, 0, IMG_W * IMG_H * 4);
	memset(pixelBuffer3, 0, CIMG_W * CIMG_H * 4);
	memset(pixelBuffer4, 0, IMG_W * IMG_H * 4);
	memset(Coord, 0, CIMG_W * CIMG_H);

	//draw pixel buffer to the screen
	DrawPixelbuffer(texture, renderer, pixelBuffer);
	DrawPixelbuffer(texture2, renderer2, pixelBuffer2);
	DrawColorPixelbuffer(texture3, renderer3, pixelBuffer3);

	//application with different features
	KinectSensor app;
	KinectSensor app2;
	KinectSensor app3;
	//KinectSensor app4;

	//Initialising the buffers within the class declarations
	app.SetPixelBuffer(pixelBuffer);
	app2.SetPixelBuffer(pixelBuffer2);
	app3.SetPixelBuffer(pixelBuffer3);
	//app4.SetPixelBuffer(pixelBuffer4);
	app3.InitCameraSpacePoint(Coord);

	//initiallising the apps with different features
	//Default- Depth
	//1 - Greyscale
	//2 - Color 
	app.init();
	app2.init(1);
	app3.init(2);
	//app4.init();

	//Declarations and initialisations
	DepthStreamDisplay display;
	auto lastTime = Clock::now();
	//OpenCV(argc, argv);
	//contour(argc, argv);

	Image = ImageProcess();
	detect obj_detect;
	//FaceEye face_det;

	bool running = true;
	float tolerance = 0.010;

	int counter = 0;
	while (running)
	{
		//get events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				//pressing the cross or pressing escape will quit the application
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					running = false;
				break;

			default: //ignore other events for this demo
				break;
			}
		}

		//calculate delta time
		const auto now = Clock::now();
		const auto duration = duration_cast<microseconds>(now - lastTime);
		const float deltaTime = duration.count() / 1000000.0f;
		lastTime = now;

		//update the application
		app.draw();
		app2.draw();
		bool map = app3.colordraw(app.getBuffer());
		//app4.draw();


		//display.init(app4.getBuffer(),app4.DepthBufferMaxActual,app4.DepthBufferMinActual);
		//display.eval();

		//draw pixel buffer to the screen
		//printSquare(pixelBuffer);
		//printSquare(pixelBuffer2);
		//printSquare(pixelBuffer3,Point(350,200),50);

		//Display pixel buffer on screen
		DrawPixelbuffer(texture2, renderer2, pixelBuffer2);

		//display colour buffer on screen
		DrawColorPixelbuffer(texture3, renderer3, pixelBuffer3);

		//displa colour buffer on screen using opencv
		assignImage(pixelBuffer3);

		//recognition of object using opencv, HAAR training and cascade classification
		obj_detect.recog(pixelBuffer3);

		//Analysis of depth data
		//DepthAnalysis DA = DepthAnalysis(obj_detect.ret_P1(), obj_detect.ret_P2(), obj_detect.ret_P3(), obj_detect.ret_P4());
		//DA.convertDepth();
		//DA.rectangulate(pixelBuffer);


		//pixel buffer for depth data displayed
		DrawPixelbuffer(texture, renderer, pixelBuffer);
		Point p1(350, 200);
		Point p2(400, 250);

		//coordinate distance approximation
		CameraSpacePoint p_s = { 0,0,0 };
		if (map) {
			p_s = ApproxCoords(Coord, p1, p2);
		}
		//printf("TEST: %f %f %f \n", p_s.X, p_s.Y, p_s.Z);

		//conditions that depict object wasnt recognised
		if (obj_detect.ret_P1().x == -1 || obj_detect.ret_P1().y == -1)
			continue;
		if (obj_detect.ret_P2().x == -1 || obj_detect.ret_P2().y == -1)
			continue;
		if (obj_detect.ret_P3().x == -1 || obj_detect.ret_P3().y == -1)
			continue;
		if (obj_detect.ret_P4().x == -1 || obj_detect.ret_P4().y == -1)
			continue;
		/*
		CameraSpacePoint ap1= ApproxCoords(Coord, obj_detect.ret_P1(), obj_detect.ret_P2());
		CameraSpacePoint ap2 = ApproxCoords(Coord, obj_detect.ret_P3(), obj_detect.ret_P4());

		//printing the data
		printf("1: %f %f %f \n", ap1.X, ap1.Y, ap1.Z);
		printf("2: %f %f %f \n", ap2.X, ap2.Y, ap2.Z);
		*/

		//Approximation based on the midpoint of the identified rectangl for the object to avoid confusion
		CameraSpacePoint apm1 = Coord[960 * (obj_detect.ret_P1().y + obj_detect.ret_P2().y) + obj_detect.ret_P1().x / 2 + obj_detect.ret_P2().x / 2];
		CameraSpacePoint apm2 = Coord[960 * (obj_detect.ret_P3().y + obj_detect.ret_P4().y) + obj_detect.ret_P3().x / 2 + obj_detect.ret_P4().x / 2];
		
		//converting to centimetre
		apm1.X *= 1000; apm1.Y *= 1000;  apm1.Z *= 1000;
		apm2.X *= 1000;  apm2.Y *= 1000;  apm2.Z *= 1000;
		
		//displaying the values
		printf("1MP: %f %f %f \n", apm1.X, apm1.Y, apm1.Z);
		printf("2MP: %f %f %f \n", apm2.X, apm2.Y, apm2.Z);
		
		//sending data across
		char *data = new char[80];
		
		//if the data is sent program exits
		bool Sent = false;
		//condition to send the data only if the coordinates are viable to send
			if (apm1.X == INFINITY || apm1.Y == INFINITY || apm1.Z == INFINITY
				|| apm1.X == 0 || apm1.Y == 0 || apm1.Z == 0
				|| apm1.X == -INFINITY || apm1.Y == -INFINITY || apm1.Z == -INFINITY
				|| apm2.X == INFINITY || apm2.Y == INFINITY || apm2.Z == INFINITY
				|| apm2.X == 0 || apm2.Y == 0 || apm2.Z == 0
				|| apm2.X == -INFINITY || apm2.Y == -INFINITY || apm2.Z == -INFINITY)
			{
				continue;
			}
			else {
				stringify(apm1, apm2, data);
				counter++;
			}
			if (counter >=3) {
				sendData to_Kuka;
				Sent = to_Kuka.DataSend(data);
			}
			if (Sent) {
				break;
			}
	
	}

	
	//clean up
	app.end();
	app2.end();
	app3.end();
	//app4.end();
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture2);
	SDL_DestroyRenderer(renderer2);
	SDL_DestroyWindow(window2);
	SDL_DestroyTexture(texture3);
	SDL_DestroyRenderer(renderer3);
	SDL_DestroyWindow(window3);
	SDL_Quit();
	printf("Information Sent");
	_getch();
	return 0;
}


