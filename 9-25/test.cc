#include "Task.hpp"
#include "pthreadpoll.hpp"

int main()
{
	srand((unsigned int)time(nullptr));
	ThreadPoll<Task>* tp = new ThreadPoll<Task>; //线程池
	tp->ThreadPollInit(); //初始化线程池当中的线程
	const char* op = "+-*/%";
	//不断往任务队列塞计算任务
	while (true){
		sleep(1);
		int x = rand() % 100;
		int y = rand() % 100;
		int index = rand() % 5;
		Task task(x, y, op[index]);
		tp->Push(task);
	}
	return 0;
}
