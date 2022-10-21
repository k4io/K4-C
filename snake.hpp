#pragma once
#include <vector>
#include "utils/vector.hpp"

#include <chrono>

namespace sg {
	static int currentscore = 0;
	static int highscore = 0;
	static int gridsize = 20;
	static int maxapples = 2;

	static std::chrono::milliseconds lastinputtime = std::chrono::milliseconds(0);

	std::vector<Vector2> apples = {};

	class Snake {
	public:
		std::vector<Vector2> sp;
		int sz;
		Vector2 lastvel = Vector2(0, 1);
		Vector2 vel = Vector2(0, 0);
		bool alive;
		Snake() {
			sp = { { (float)gridsize / 2, (float)gridsize / 2 },
				{ (float)gridsize / 2, (float)(gridsize / 2) - 1 } };
			sz = 3;
			vel = Vector2(0, 0);

			currentscore = 0;
			lastinputtime = std::chrono::milliseconds(0);
			alive = true;
		}

		//Will only accept input every 1 second
		void DoInput() {
			if (!alive) return;
			auto ctime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());//time(0);
			auto next_input_time = lastinputtime + std::chrono::milliseconds(500);
			if (ctime > next_input_time) {
				if (GetAsyncKeyState(0x57)) //W pressed
					vel.y--;
				else if (GetAsyncKeyState(0x41)) //A pressed
					vel.x--;
				else if (GetAsyncKeyState(0x53)) //S pressed
					vel.y++;
				else if (GetAsyncKeyState(0x44)) //D pressed
					vel.x++;
				UpdateSnake();
				vel = Vector2(0, 0);
				lastinputtime = ctime;
			}
		}

		void CheckAndGenerateApple() {
			if (apples.size() > maxapples) return;
			int x = 0 + (rand() % gridsize);
			int y = 0 + (rand() % gridsize);
			Vector2 v2(x, y);
			for (auto v : sp)
				if (v == v2)
					CheckAndGenerateApple();
			//random, free space
			apples.push_back(v2);
		}

		void UpdateSnake() {
			CheckAndGenerateApple();
			Vector2 head(0, 0);
			std::vector<Vector2> body = {};
			for (size_t i = 0; i < sp.size(); i++)
			{
				if (i == 0)
					head = sp[i];
				else body.push_back(sp[i - 1]);
			}

			if (lastvel.empty())
				lastvel = { 0, 1 };
			if (vel.empty()) //no key was pressed so force velocity in last direction, if none, up
				vel = lastvel;
			else lastvel = vel;
			
			Vector2 new_head = head + vel;
			//if(new_head == sp[1])
			//	new_head = head;
			std::vector<Vector2> new_body = { new_head };
			for (size_t i = 1; i < sp.size(); i++)
				new_body.push_back(sp[i - 1]);

			bool ate = false;
			for (auto v : new_body) {
				for (auto a : apples)
					if (v == a)
					{
						ate = true;
						apples.erase(std::remove(apples.begin(), apples.end(), a), apples.end());
					}
				if (ate)
				{
					currentscore++;
					this->sz++;
					auto spos = new_body[new_body.size() - 2];
					auto lpos = new_body[new_body.size() - 1];
					auto diff = spos - lpos;
					auto npos = lpos - diff;
					new_body.push_back(npos);
					Beep(167, 50);
					break;
				}
			}
			for (auto v2 : sp) {
				if (new_head == v2) //has hit tail
					this->alive = false;
			}
			this->sp = new_body;
			for (auto v : new_body) {
				if (v.x > gridsize
					|| v.y > gridsize
					|| v.x < 0
					|| v.y < 0)
					alive = false;
			}
			if (this->sz > 300)
				alive = false;

			if (!alive) {
				highscore = currentscore;
				apples.clear();
			}
		}
	};
}