#include <queue>
#include <string>
#include <set>
#include <iostream>
#include "Cube1D.h"
#include "ArrayUtils.h"
#include "CubeMapEntity.h"

using namespace std;

priority_queue<CubeState> open_queue;
set<string> calc_path;


const int MAX_STEP = 8;

// A*算法解决游戏
void solve() {
	while (!open_queue.empty()) {
		CubeState state = open_queue.top();
		open_queue.pop();
		if (state.stepNum > MAX_STEP) {
			continue;
		}
		state.map.printMap();
		if (state.map.isGoalState()) {
			cout << "发现结果,总步数:" + to_string(state.stepNum) << endl;
			cout << "操作:" + state.map.op << endl;
			state.map.printMap();
			return;
		}
		/*if (state.map.upLeft(false)) {
			CubeMapEntity newMap = state.map;
			newMap.upLeft(true);
			if (calc_path.find(newMap.strHashCode()) == calc_path.end()) {
				calc_path.insert(newMap.strHashCode());
				CubeState state = { newMap, state.stepNum + 1 };
				open_queue.push(state);
			}
		}*/
	}
}


void cube_1d_main() {
	CubeMapEntity map = CubeMapEntity(6, true);
	map.printMap();
	CubeState state = { &map, 0 };
	open_queue.push(state);
	CubeState state2 = open_queue.top();
	state2.map.printMap();
	solve();
	cout << "计算总条数:" << endl;
}