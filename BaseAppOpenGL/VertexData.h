#pragma once

#define VERTICES 0
#define INDICES 1
#define NUM_BUFFERS 2

class CVector3
{
public:
	float x, y, z;
};

class CIndex4
{
public:
	unsigned int p1, p2, p3, p4;
};

class CIndex3
{
public:
	unsigned int p1, p2, p3;
};

class CColor3
{
public:
	int R, G, B;
};

class CTexCoords2
{
public:
	float u, v;
};

