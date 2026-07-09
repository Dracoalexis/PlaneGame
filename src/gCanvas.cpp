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
	airport.loadModel("militarybase.obj");
	plane.loadModel("planewithoutwheels.obj");
	pbackleftwheel.loadModel("planeleftbackwheel.obj");
	pbackrightwheel.loadModel("planerightbackwheel.obj");
	pfrontwheel.loadModel("planefrontwheel.obj");
	camfont.loadFont("ACES07_Regular.ttf", 20);
	speedfont.loadFont("ACES07_Regular.ttf", 20);
	altfont.loadFont("ACES07_Regular.ttf", 20);
	stallfont.loadFont("ACES07_Regular.ttf", 20);
	plane.boom(-0.15f);
	plane.dolly(1.0f);
	plane.truck(-5.0f);
	plane.pan(PI / 2);
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
	sun.setPosition(0.0f, 1000.0f, 200.0f);
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
	airport.scale(0.03);
	sky.pan(PI);
	keystate = KEY_NONE;
	camnames[CAM_TOPBACK] = "TOP BACK";
	camnames[CAM_BACK] = "BACK";
	camnames[CAM_FRONT] = "FRONT";
	camnames[CAM_BACK_REVERSE] = "REVERSE";
	camnames[CAM_ACTION] = "ACTION";
	speed = 0.0f;
	maxspeed = 1.2f;
	acceleration = 0.0016f;
	deceleration = 0.0002f;
	wheeltiltangle = 0.0f;
	maxwheeltiltangle = PI / 2;
	fallrate = 0.0f;
	fallacceleration = 0.0006f;
	topbackcameradistance = 6.0f;
}


void gCanvas::update() {
	//resetRotations();
	movePlane();
	moveCameras();
	gLogi("Hýz " + std::to_string(speed));
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
	if(fallrate > 0.06f) stallfont.drawText("STALL WARNING", (getWidth() - stallfont.getStringWidth("STALL WARNING")) / 2, getHeight() - 200);
}

void gCanvas::moveCameras() {
	cam[CAM_TOPBACK].setPosition(plane.getPosition());
	cam[CAM_TOPBACK].setOrientation(plane.getOrientation());
	cam[CAM_TOPBACK].boom(2.0f);
	cam[CAM_TOPBACK].dolly(topbackcameradistance);

    cam[CAM_BACK].setPosition(plane.getPosition());
    cam[CAM_BACK].setOrientation(plane.getOrientation());
    cam[CAM_BACK].boom(1.0f);
    cam[CAM_BACK].dolly(5.0f);

	cam[CAM_FRONT].setPosition(plane.getPosition());
	cam[CAM_FRONT].setOrientation(plane.getOrientation());
	cam[CAM_FRONT].boom(1.0f);

	cam[CAM_BACK_REVERSE].setPosition(plane.getPosition());
	cam[CAM_BACK_REVERSE].setOrientation(plane.getOrientation());
	cam[CAM_BACK_REVERSE].pan(PI);
	cam[CAM_BACK_REVERSE].boom(1.0f);
	cam[CAM_BACK_REVERSE].dolly(-3.0f);

	cam[CAM_ACTION].setPosition(plane.getPosition());
	cam[CAM_ACTION].setOrientation(plane.getOrientation());
	cam[CAM_ACTION].pan(PI);
	cam[CAM_ACTION].boom(1.0f);
	cam[CAM_ACTION].truck(0.8f);
	cam[CAM_ACTION].dolly(1.5f);
}

void gCanvas::movePlane() {
	if(keystate & KEY_LEFT_SHIFT) {
		topbackcameradistance += 0.008f;
		speed += acceleration;
		if(topbackcameradistance > 7.5f) topbackcameradistance = 7.5f;
		if(speed > maxspeed) speed = maxspeed;
	}
	else if(keystate & KEY_LEFT_CONTROL) {
		speed -= deceleration * 15;

		if(speed < 0.0f)
			speed = 0.0f;
	}
	else {
		topbackcameradistance -= 0.008f;
		speed -= deceleration;
		if(topbackcameradistance < 6.0f) topbackcameradistance = 6.0f;
		if(speed < 0.0f) speed = 0.0f;
	}
	if(keystate & KEY_Q) {
		plane.pan(0.0030f);
	}
	else if(keystate & KEY_E) {
		plane.pan(-0.0030f);
	}
	if(plane.getPosY() > 1.0f || speed >= 0.300f) {
		if(keystate & KEY_W) {
			plane.tilt(-0.0090f);
		}
		else if(keystate & KEY_S) {
			plane.tilt(0.0090f);
		}
	}
	if(keystate & KEY_A) {
		plane.roll(0.0190f);
		//plane.pan(0.0060f);
	}
	else if(keystate & KEY_D) {
		plane.roll(-0.0190f);
		//plane.pan(-0.0060f);
	}
	plane.dolly(-speed);
	//Fall mechanic
	glm::vec3 forward = plane.getOrientation() * glm::vec3(0.0f, 0.0f, -1.0f);
	speed -= forward.y * 0.001f;
	if(speed < 0.300f && forward.y > 0.0f && plane.getPosY() > 1.0f) {
		fallrate += fallacceleration;
		plane.move(0.0f, -fallrate, 0.0f);
	}
	else {
		fallrate = 0.0f;
	}
	//Wheel pivot calculation
	glm::quat planeorientation = plane.getOrientation();
	glm::vec3 pivot = plane.getPosition() + planeorientation * glm::vec3(-0.13f, 0.8f, 1.8f);
	glm::vec3 wheeloffset =  glm::vec3(0.0f, -0.8f, 0.0f);
	if(plane.getPosY() >= 12.0f) {
	    wheeltiltangle -= 0.02f;
	    if(wheeltiltangle <= -maxwheeltiltangle)
	        wheeltiltangle = -maxwheeltiltangle;
	}
	else {
		wheeltiltangle += 0.02f;
		if(wheeltiltangle >= 0.0f) {
			wheeltiltangle = 0.0f;
		}
	}
	glm::quat wheelrotation = glm::angleAxis(-wheeltiltangle, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 wheelposition = pivot + planeorientation * (wheelrotation * wheeloffset);
	pbackleftwheel.setPosition(wheelposition);
	pbackleftwheel.setOrientation(planeorientation * wheelrotation);

	glm::vec3 rightpivot = plane.getPosition() + planeorientation * glm::vec3(0.25f, 0.8f, 1.9f);
	glm::vec3 rightwheeloffset = glm::vec3(0.0f, -0.8f, 0.0f);
	glm::quat rightwheelrotation = glm::angleAxis(-wheeltiltangle * 0.8f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 rightwheelposition =rightpivot + planeorientation * (rightwheelrotation * rightwheeloffset);
	pbackrightwheel.setPosition(rightwheelposition);
	pbackrightwheel.setOrientation(planeorientation * rightwheelrotation);

	glm::vec3 frontpivot = plane.getPosition() + planeorientation * glm::vec3(0.0f, 0.8f, 0.1f);
	glm::vec3 frontwheeloffset = glm::vec3(0.0f, -0.6f, 0.0f);
	glm::quat frontwheelrotation = glm::angleAxis(wheeltiltangle, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 frontwheelposition = frontpivot + planeorientation * (frontwheelrotation * frontwheeloffset);
	pfrontwheel.setPosition(frontwheelposition);
	pfrontwheel.setOrientation(planeorientation * frontwheelrotation);
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
