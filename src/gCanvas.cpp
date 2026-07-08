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
	plane.loadModel("planewithoutwheels.obj");
	pbackleftwheel.loadModel("planeleftbackwheel.obj");
	pbackrightwheel.loadModel("planerightbackwheel.obj");
	pfrontwheel.loadModel("planefrontwheel.obj");
	camfont.loadFont("ACES07_Regular.ttf", 20);
	speedfont.loadFont("ACES07_Regular.ttf", 20);
	altfont.loadFont("ACES07_Regular.ttf", 20);
	plane.boom(-0.15f);
	plane.dolly(7.0f);
	plane.truck(-5.0f);
	plane.pan(PI);
	pbackleftwheel.pan(PI);
	pbackrightwheel.pan(PI);
	pfrontwheel.pan(PI);
	activecam = CAM_TOPBACK;
	cam[CAM_TOPBACK].pan(PI);
	cam[CAM_BACK].pan(PI);
	cam[CAM_FRONT].pan(PI);
	sun.setSpecularColor(8, 8, 8);
	sun.setAmbientColor(255, 255, 255);
	sun.setDiffuseColor(210, 210, 210);
	sun.setPosition(0.0f, 200.0f, 200.0f);
	sun.setAttenuation(1.0f, 0.001f, 0.0000000001f);
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
	horizontalplaneangle = 0.0f;
	verticalplaneangle = 0.0f;
	lateralplaneangle = 0.0f;
	maxlateralplaneangle = PI / 4;
	camnames[CAM_TOPBACK] = "TOP BACK";
	camnames[CAM_BACK] = "BACK";
	camnames[CAM_FRONT] = "FRONT";
	camnames[CAM_BACK_REVERSE] = "REVERSE";
	camnames[CAM_ACTION] = "ACTION";
	speed = 0.0f;
	maxspeed = 1.2f;
	acceleration = 0.0012f;
	deceleration = 0.0002f;
}


void gCanvas::update() {
	resetCameras();
	resetRotations();
	movePlane();
	moveCameras();
	gLogi("Hýz" + std::to_string(speed));
	gLogi("Yükseklik " + std::to_string(plane.getPosY()));
}


void gCanvas::draw() {
	cam[activecam].begin();
	enableDepthTest();
	sun.enable();
	sky.draw();
	airport.draw();
	plane.draw();
	pbackleftwheel.draw();
	pbackrightwheel.draw();
	pfrontwheel.draw();
	sun.disable();
	disableDepthTest();
	cam[activecam].end();
	setColor(52, 235, 101);
	camfont.drawText("CAM : " + camnames[activecam], getWidth() - 250, getHeight() - 200);
	speedfont.drawText("SPEED : " + std::to_string(static_cast<int>(speed * 1000)), getWidth() / 2 - 300, getHeight() / 2);
	altfont.drawText("ALT : " + std::to_string(static_cast<int>(plane.getPosY() * 10)), getWidth() / 2 + 200, getHeight() / 2);

}

void gCanvas::moveCameras() {
	cam[CAM_TOPBACK].setPosition(plane.getPosition());
	cam[CAM_TOPBACK].roll(lateralplaneangle);
	cam[CAM_TOPBACK].pan(horizontalplaneangle);
	cam[CAM_TOPBACK].tilt(verticalplaneangle);
	cam[CAM_TOPBACK].boom(2.0f);
	cam[CAM_TOPBACK].dolly(6.0f);

	cam[CAM_BACK].setPosition(plane.getPosition());
	cam[CAM_BACK].roll(lateralplaneangle);
	cam[CAM_BACK].pan(horizontalplaneangle);
	cam[CAM_BACK].tilt(verticalplaneangle);
	cam[CAM_BACK].boom(1.0f);
	cam[CAM_BACK].dolly(5.0f);

	cam[CAM_FRONT].setPosition(plane.getPosition());
	cam[CAM_FRONT].roll(lateralplaneangle);
	cam[CAM_FRONT].pan(horizontalplaneangle);
	cam[CAM_FRONT].tilt(verticalplaneangle);
	cam[CAM_FRONT].boom(1.0f);

	cam[CAM_BACK_REVERSE].setPosition(plane.getPosition());
	cam[CAM_BACK_REVERSE].roll(lateralplaneangle);
	cam[CAM_BACK_REVERSE].pan(horizontalplaneangle);
	cam[CAM_BACK_REVERSE].tilt(-verticalplaneangle);
	cam[CAM_BACK_REVERSE].boom(1.0f);
	cam[CAM_BACK_REVERSE].dolly(-3.0f);

	cam[CAM_ACTION].setPosition(plane.getPosition());
	cam[CAM_ACTION].roll(verticalplaneangle);
	cam[CAM_ACTION].pan(horizontalplaneangle);
	cam[CAM_ACTION].tilt(-lateralplaneangle);
	cam[CAM_ACTION].pan(PI / 2);
	cam[CAM_ACTION].boom(1.0f);
	cam[CAM_ACTION].truck(0.8f);
	cam[CAM_ACTION].dolly(1.5f);
}

void gCanvas::resetCameras() {
	cam[CAM_TOPBACK].tilt(-verticalplaneangle);
	cam[CAM_TOPBACK].pan(-horizontalplaneangle);
	cam[CAM_TOPBACK].roll(-lateralplaneangle);

	cam[CAM_BACK].tilt(-verticalplaneangle);
	cam[CAM_BACK].pan(-horizontalplaneangle);
	cam[CAM_BACK].roll(-lateralplaneangle);

	cam[CAM_FRONT].tilt(-verticalplaneangle);
	cam[CAM_FRONT].pan(-horizontalplaneangle);
	cam[CAM_FRONT].roll(-lateralplaneangle);

	cam[CAM_BACK_REVERSE].tilt(verticalplaneangle);
	cam[CAM_BACK_REVERSE].pan(-horizontalplaneangle);
	cam[CAM_BACK_REVERSE].roll(-lateralplaneangle);

	cam[CAM_ACTION].pan(-PI / 2);
	cam[CAM_ACTION].tilt(lateralplaneangle);
	cam[CAM_ACTION].pan(-horizontalplaneangle);
	cam[CAM_ACTION].roll(-verticalplaneangle);
}

void gCanvas::resetRotations() {
	pbackleftwheel.roll(-lateralplaneangle);
	pbackleftwheel.tilt(-verticalplaneangle);
	pbackleftwheel.pan(-horizontalplaneangle);

	pbackrightwheel.roll(-lateralplaneangle);
	pbackrightwheel.tilt(-verticalplaneangle);
	pbackrightwheel.pan(-horizontalplaneangle);

	pfrontwheel.roll(-lateralplaneangle);
	pfrontwheel.tilt(-verticalplaneangle);
	pfrontwheel.pan(-horizontalplaneangle);

	plane.roll(-lateralplaneangle);
	plane.tilt(-verticalplaneangle);
	plane.pan(-horizontalplaneangle);
}

void gCanvas::movePlane() {

	if(keystate & KEY_LEFT_SHIFT) {
		speed += acceleration;
		if(speed > maxspeed) speed = maxspeed;
	} else if(keystate & KEY_LEFT_CONTROL) {
		speed -= deceleration * 8;
		if(speed < 0.0f) speed = 0.0f;
	}
	else {
		speed -= deceleration;
		if(speed < 0.0f) speed = 0.0f;
	}
	if(keystate & KEY_Q) {
		horizontalplaneangle += 0.0030f;
	}
	else if(keystate & KEY_E) {
		horizontalplaneangle -= 0.0030f;
	}
	//if(speed >= 0.300000) {
		if(keystate & KEY_W) {
			verticalplaneangle -= 0.0030f;
		}
		else if(keystate & KEY_S) {
			verticalplaneangle += 0.0030f;
		}
		if(keystate & KEY_A) {
			lateralplaneangle += 0.0090f;
		}
		else if(keystate & KEY_D) {
			lateralplaneangle -= 0.0090f;
		}
	//}
	plane.pan(horizontalplaneangle);
	plane.tilt(verticalplaneangle);
	plane.roll(lateralplaneangle);
	plane.dolly(-speed);

	pbackleftwheel.setPosition(plane.getPosition());
	pbackleftwheel.pan(horizontalplaneangle);
	pbackleftwheel.tilt(verticalplaneangle);
	pbackleftwheel.roll(lateralplaneangle);
	pbackleftwheel.dolly(1.8f);
	pbackleftwheel.truck(-0.13f);
	pbackrightwheel.setPosition(plane.getPosition());
	pbackrightwheel.pan(horizontalplaneangle);
	pbackrightwheel.tilt(verticalplaneangle);
	pbackrightwheel.roll(lateralplaneangle);
	pbackrightwheel.dolly(1.9f);
	pbackrightwheel.truck(0.25f);
	pfrontwheel.setPosition(plane.getPosition());
	pfrontwheel.pan(horizontalplaneangle);
	pfrontwheel.tilt(verticalplaneangle);
	pfrontwheel.roll(lateralplaneangle);
	pfrontwheel.boom(0.2f);
	pfrontwheel.dolly(0.1f);
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
	case G_KEY_Q:
		pressedkey = KEY_Q;
		break;
	case G_KEY_E:
		pressedkey = KEY_E;
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
		if(activecam == 4) activecam = 0;
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
	case G_KEY_Q:
		pressedkey = KEY_Q;
		break;
	case G_KEY_E:
		pressedkey = KEY_E;
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
