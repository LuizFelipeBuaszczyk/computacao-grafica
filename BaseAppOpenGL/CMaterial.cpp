#include "CMaterial.h"

CMaterial::CMaterial()
{
	rA = 0.0f; gA = 0.0f; bA = 0.0f; aA = 0.0f;
	rD = 0.0f; gD = 0.0f; bD = 0.0f; aD = 0.0f;
	rS = 0.0f; gS = 0.0f; bS = 0.0f; aS = 0.0f;
	s = 0.0f;
}


CMaterial::CMaterial(
	float _rA, float _gA, float _bA, float _aA,
	float _rD, float _gD, float _bD, float _aD,
	float _rS, float _gS, float _bS, float _aS,
	float _s)
{
	rA = _rA; gA = _gA; bA = _bA; aA = _aA;
	rD = _rD; gD = _gD; bD = _bD; aD = _aD;
	rS = _rS; gS = _gS; bS = _bS; aS = _aS;
	s = _s;
}

CMaterial::~CMaterial()
{

}