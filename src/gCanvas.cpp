/*
 * gCanvas.cpp
 *
 *  Created on: May 6, 2020
 *      Author: Noyan Culum
 */

#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	gLogi("TEST") << "Constructor started";

	this->root = root;

	gLogi("TEST") << "Constructor finished";
}


gCanvas::~gCanvas() {
}


void gCanvas::setup() {
	airport.loadModel("biggermap.obj");
	plane.loadModel("plane.obj");
	camfont.loadFont("ACES07_Regular.ttf", 20);
	plane.boom(-0.15f);
	plane.dolly(7.0f);
	plane.truck(-5.0f);
	plane.pan(PI);
	activecam = CAM_TOPBACK;
	cam[CAM_TOPBACK].pan(PI);
	cam[CAM_BACK].pan(PI);
	cam[CAM_FRONT].pan(PI);
	sun.setSpecularColor(8, 8, 8);
	sun.setAmbientColor(255, 255, 255);
	sun.setDiffuseColor(210, 210, 210);
	sun.setPosition(0.0f, 80.0f, 80.0f);
	std::vector<std::string> texturepaths;
	texturepaths.push_back("skymap/right.jpg");
	texturepaths.push_back("skymap/left.jpg");
	texturepaths.push_back("skymap/top.jpg");
	texturepaths.push_back("skymap/bottom.jpg");
	texturepaths.push_back("skymap/front.jpg");
	texturepaths.push_back("skymap/back.jpg");
	sky.loadTextures(texturepaths);
	sky.scale(20.0f);
	sky.pan(PI);
	keystate = KEY_NONE;
	planeangle = 0.0f;
	camnames[CAM_TOPBACK] = "TOP BACK";
	camnames[CAM_BACK] = "BACK";
	camnames[CAM_FRONT] = "FRONT";
	camnames[CAM_BACK_REVERSE] = "REVERSE";
}


void gCanvas::update() {
	resetCameras();
	movePlane();
	moveCameras();
}


void gCanvas::draw() {
	cam[activecam].begin();
	enableDepthTest();
	sun.enable();
	sky.draw();
	airport.draw();
	plane.draw();
	sun.disable();
	disableDepthTest();
	cam[activecam].end();
	setColor(52, 235, 101);
	camfont.drawText("CAM : " + camnames[activecam], getWidth() - 250, getHeight() - 200);
}

void gCanvas::moveCameras() {
	cam[CAM_TOPBACK].setPosition(plane.getPosition());
	cam[CAM_TOPBACK].pan(planeangle);
	cam[CAM_TOPBACK].boom(2.0f);
	cam[CAM_TOPBACK].dolly(6.0f);

	cam[CAM_BACK].setPosition(plane.getPosition());
	cam[CAM_BACK].pan(planeangle);
	cam[CAM_BACK].boom(1.0f);
	cam[CAM_BACK].dolly(5.0f);

	cam[CAM_FRONT].setPosition(plane.getPosition());
	cam[CAM_FRONT].pan(planeangle);
	cam[CAM_FRONT].boom(1.0f);

	cam[CAM_BACK_REVERSE].setPosition(plane.getPosition());
	cam[CAM_BACK_REVERSE].pan(planeangle);
	cam[CAM_BACK_REVERSE].boom(1.0f);
	cam[CAM_BACK_REVERSE].dolly(-3.0f);
}

void gCanvas::resetCameras() {
	cam[CAM_TOPBACK].pan(-planeangle);
	cam[CAM_BACK].pan(-planeangle);
	cam[CAM_FRONT].pan(-planeangle);
	cam[CAM_BACK_REVERSE].pan(-planeangle);
}
void gCanvas::movePlane() {
	plane.pan(-planeangle);
	if(keystate & KEY_A) {
		planeangle += 0.0030f;
	}
	else if(keystate & KEY_D) {
		planeangle -= 0.0030f;
	}
	plane.pan(planeangle);
	if(keystate & KEY_LEFT_SHIFT) {
		plane.dolly(-0.20f);
	}
	else if(keystate & KEY_LEFT_CONTROL) {
		plane.dolly(0.20f);
	}
}


void gCanvas::keyPressed(int key) {
	if(key == G_KEY_V) {
		previouscam = activecam;
		activecam = CAM_BACK_REVERSE;
	}
	int pressedkey = KEY_NONE;
	switch(key) {
	case G_KEY_W:
		pressedkey = KEY_W;
		break;
	case G_KEY_S:
		pressedkey = KEY_S;
		break;
	case G_KEY_A:
		pressedkey = KEY_A;
		break;
	case G_KEY_D:
		pressedkey = KEY_D;
		break;
	case G_KEY_LEFT_SHIFT:
		pressedkey = KEY_LEFT_SHIFT;
		break;
	case G_KEY_LEFT_CONTROL:
		pressedkey = KEY_LEFT_CONTROL;
		break;
	default:
		break;
	}

	keystate |= pressedkey;
}


void gCanvas::keyReleased(int key) {
	if(key == G_KEY_V) {
		activecam = previouscam;
	}
	else if(key == G_KEY_C) {
		activecam += 1;
		if(activecam == 3) activecam = 0;
	}
	int pressedkey = KEY_NONE;

	switch(key) {
	case G_KEY_W:
		pressedkey = KEY_W;
		break;
	case G_KEY_S:
		pressedkey = KEY_S;
		break;
	case G_KEY_A:
		pressedkey = KEY_A;
		break;
	case G_KEY_D:
		pressedkey = KEY_D;
		break;
	case G_KEY_LEFT_SHIFT:
		pressedkey = KEY_LEFT_SHIFT;
		break;
	case G_KEY_LEFT_CONTROL:
		pressedkey = KEY_LEFT_CONTROL;
		break;
	default:
		break;
	}

	keystate &= ~pressedkey;
}


void gCanvas::charPressed(unsigned int codepoint) {
}


void gCanvas::mouseMoved(int x, int y) {
}


void gCanvas::mouseDragged(int x, int y, int button) {
}


void gCanvas::mousePressed(int x, int y, int button) {
}


void gCanvas::mouseReleased(int x, int y, int button) {
}


void gCanvas::mouseScrolled(int x, int y) {
}


void gCanvas::mouseEntered() {
}


void gCanvas::mouseExited() {
}


void gCanvas::windowResized(int w, int h) {
}


void gCanvas::showNotify() {
}


void gCanvas::hideNotify() {
}
