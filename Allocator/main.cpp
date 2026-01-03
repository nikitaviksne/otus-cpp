#include "MyAllocator.h"
#include "MyContainer.h"
#include <map>
#include <algorithm>
#include <iostream>

template<typename T1, typename T2, typename C, typename A>
void print_out(const std::map<T1, T2, C, A> & m)
{
	for (const auto& pair : m)
		std::cout << pair.first <<"\t" << pair.second<<  std::endl;
}
template <typename T, typename A>
void print_out(const MyContainer<T, A> &cont)
{
	for (auto it=cont.begin(); it!=cont.end(); it++)
		std::cout << *it << "\n";
}

int main()
{
	//создание RuntimeBlockAllocator 
	auto alloc = make_block_allocator<std::pair<const int, int>>(10);
	std::map<const int, int, std::less<int>, 
             RuntimeBlockAllocator<std::pair<const int, int>, 10> > myMap(std::less<int>{});
	//запоняем контейнеры
	for (int iii =0; iii < 10; iii++)
	{
		if ( (iii==0) || (iii == 1))
		{
			myMap[iii] = 1;
			continue;
		}
		myMap[iii] = myMap[iii-1] * iii;
	}
	printf("Выводим контейнер myMap с аллокатором, в качетсве шаблонного параметра передано кол-во элементов\n");
	print_out(myMap);

	MyContainer<int> justContainer;
	for (int iii=0; iii<10; ++iii)
		justContainer.push_back(iii);
	
	MyContainer <int, RuntimeBlockAllocator<int, 10> > customContainer;
	
	for (int iii=0; iii<10; ++iii)
		customContainer.push_back(iii);

	std::cout << "Выводим свой контейнер без аллокатора" <<"\n";
	print_out(justContainer);
	std::cout << "Выводим свой контейнер с аллокатором" <<"\n";
	print_out(customContainer);
	return 0;
}
