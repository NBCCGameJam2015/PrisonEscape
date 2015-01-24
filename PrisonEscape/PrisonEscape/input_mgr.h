#ifndef _INPUT_H_
#define _INPUT_H_

#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <functional>

#include <Windows.h>
#include <iostream>

class input_mgr
{
private:
	std::mutex vector_mutex;
	std::mutex map_mutex;
	std::thread P1;
	std::vector<int> input_list;
	std::map<int, std::function<void()>> Key_Action_Map;
	POINT curPos;
	BOOL result;

protected:
	bool thread_running = false;
	inline void Run_Action(int key_input_value);

public:
	void Register_Action(int key_input_value, std::function<void()> action);
	void ProcessQueue();
	
	void Add(int key_input_value);

	void MouseInput();

	void Start();
	void Stop();
	
};

#endif
