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
	plane.boom(-0.15f);
	plane.dolly(7.0f);
	plane.truck(-5.0f);
	plane.pan(PI);
	cam[CAM_BACK].pan(PI);
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
}


void gCanvas::update() {
	cam[CAM_BACK].pan(-planeangle);
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

	cam[CAM_BACK].setPosition(plane.getPosition());
	cam[CAM_BACK].pan(planeangle);
	cam[CAM_BACK].boom(2.0f);
	cam[CAM_BACK].dolly(6.0f);
}


void gCanvas::draw() {
	cam[CAM_BACK].begin();
	enableDepthTest();
	sun.enable();
	sky.draw();
	airport.draw();
	plane.draw();
	sun.disable();
	disableDepthTest();
	cam[CAM_BACK].end();
}


void gCanvas::keyPressed(int key) {
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
