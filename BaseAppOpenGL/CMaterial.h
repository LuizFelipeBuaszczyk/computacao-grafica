#pragma once
class CMaterial
{
public:
	CMaterial();
	CMaterial(
		float _rA, float _gA, float _bA, float _aA,
		float _rD, float _gD, float _bD, float _aD,
		float _rS, float _gS, float _bS, float _aS,
		float _s);

	~CMaterial();

	float rA; float gA; float bA; float aA;
	float rD; float gD; float bD; float aD;
	float rS; float gS; float bS; float aS;
	float s;
};
