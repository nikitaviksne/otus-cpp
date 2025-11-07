#include <stdio.h>
#include <cstdint>
#include <vector>
#include <iostream>

int main()
{	
	
	printf("Программа фильтрации IP адресов\n");
	int res (10);
	std::vector<int> arrIp;
	int ip;
	while (true)
	{
		uint8_t f1,f2,f3,f4;
		printf("Введите IP: ");
		res = scanf("%d.%d.%d.%d%*s%*s\n", &f1, &f2, &f3, &f4);
		if (res<=0) break;
		ip = (uint32_t) f1 + (f2<<8) + (f3<<16) + (f4<<24);
		printf("было считано %d считанный IP-адресс: %d.%d.%d.%d или %u\n", res, f1,f2,f3,f4, ip);
		//организуем сразу отсортированный массив по принципу бинарного поиска (или сортировка слиянием?)
		int size = arrIp.size();
		static int low; //нижняя граница поиска
		static int up; // верхняя граница поиска
		if (size == 0)
		{
			arrIp.insert(arrIp.begin(), ip);
			low = 0;
			up = 1; //arrIp.size();
		}
		else
		{	
			if (ip < arrIp[low]) //если новый элемент меньше предыдущего, который я добавил в массив, то рассматриваем правую часть
			{
				up = arrIp.size();
			}
			else
			{
				low = 0;
			}
			while (low < up)
			{
				int index = (low+up)/2;
				if (arrIp[index] == ip)//нашли равный элемен
				{
					arrIp.insert(arrIp.begin() + index, ip);
					break;
				}
				if (arrIp[index] < ip) //значит надо вставлять в левую часть
				{
					up = index - 1;
					continue;
				}
				if (arrIp[index] > ip) //значит надо вставлять в правую часть от текущего индекса
				{
					low = index + 1;
					continue;
				}
			}//while
			//после этого цикла, если у меня такого же ip не будет найдено в массиве, то возникнет ситуация, low >= up, а значит вставлять надо на позицию up+1 = low
			arrIp.insert(arrIp.begin() + low, ip);
		}//else (if (size==0) )
		printf("size = %d; low = %d; up = %d\n",size, low, up);
		//Вывод вектора
		for(int iii = 0; iii < arrIp.size(); iii++) printf("%u\n", arrIp[iii]);
	}// while true (input)
	//for (auto itr = arrIp.begin(); itr<arrIp.end(); itr++) std::cout << itr <<"\n";
	return 0;
}
