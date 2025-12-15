#pragma once

/* OpenGL window constants */
#define WINDOW_X 300
#define WINDOW_Y 300

#define WINDOW_W 600		// window's width
#define WINDOW_H 720		// window's height

#define BOUNDARY_X (WINDOW_W-40)/2
#define BOUNDARY_Y (WINDOW_H)/2





/* GAME constants */
#define NUM_ROW 36
#define NUM_COL 28

#define BLOCK_SIZE 20
#define MOVE_SPEED 1

// map boundary
#define LEFT_BOUNDARY -(WINDOW_W-40)/2
#define RIGHT_BOUNDARY (WINDOW_W-40)/2
#define TOP_BOUNDARY (WINDOW_H)/2
#define BOTTOM_BOUNDARY -(WINDOW_H)/2