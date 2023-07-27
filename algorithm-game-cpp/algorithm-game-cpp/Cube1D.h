#pragma once
#include <iostream>
#include "CubeMapEntity.h"

using namespace std;

struct CubeState {
	CubeMapEntity map;
	int stepNum;
	//返回true时,说明this的优先级低于other
	bool operator < (const CubeState& other) const {
		return stepNum > other.stepNum;
	}
};

//一维魔方入口函数
void cube_1d_main();