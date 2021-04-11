all:u_thrdpool u_lst

u_thrdpool:u_thrdpool.cc ../thrdpool/thrdpool.cc
	g++ -g  $^ -o u_thrdpool -std=c++11 -pthread

u_lst:u_lst.cc
	g++ -g -D DEBUG $^ -o u_lst -std=c++11 -pthread