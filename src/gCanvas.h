/*
 * gCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: Noyan Culum
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gModel.h"
#include "gCamera.h"
#include "gSkyBox.h"
#include "gLight.h"
#include "gFont.h"

#include <array>


class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();
	void windowResized(int w, int h);

	void showNotify();
	void hideNotify();

private:
	static const int KEY_NONE = 0, KEY_W = 1, KEY_S = 2,KEY_A = 4, KEY_D = 8, KEY_LEFT_SHIFT = 16,
	KEY_LEFT_CONTROL = 32;
	static const int CAMERANUM = 4;

	enum {
		CAM_TOPBACK, CAM_BACK, CAM_FRONT, CAM_BACK_REVERSE
	};

	gApp* root;

	void moveCameras();
	void movePlane();
	void resetCameras();

	gModel airport;
	gModel plane;
	gFont camfont;
	gSkybox sky;
	gLight sun;

	std::array<gCamera, CAMERANUM> cam;

	int keystate;
	float planeangle;
	int activecam;
	int previouscam;
	std::string camnames[CAMERANUM];
};

#endif /* GCANVAS_H_ */
