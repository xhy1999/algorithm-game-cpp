#pragma once
#include <iostream>
#include "CubeMapEntity.h"

using namespace std;

struct CubeState {
	CubeMapEntity map;
	int stepNum;
	//����trueʱ,˵��this�����ȼ�����other
	bool operator < (const CubeState& other) const {
		return stepNum > other.stepNum;
	}
};

//һάħ����ں���
void cube_1d_main();