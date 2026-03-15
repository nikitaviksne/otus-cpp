#include <iostream>
#include <iomanip>
#include <memory>
#include "Matrix.h"
#include "NMatrix.h"

int main(){
	Matrix<int, 0> matrix;
	for(int i=0; i<10; i++)
	{
		matrix[i][i] = i; //главная диагональ
		matrix[i][9-i] = 9-i;//побочная диагональ
	}
	//Вывод фрагмента [1,1] -- [8,8]
	std::cout << "Фрагмент [1,1] -- [8,8]:\n";
	for(int i=1; i<9; i++)
	{
		for(int j=1; j<9; j++){
			std::cout << matrix[i][j] << (j ==8? "": " ");
		}
		std::cout << std::endl;
	}

	std::cout << "Кол-во занятых ячеек сейчас: " << matrix.size() << std::endl;
	
	//Вывод ячеек и их позиций
	std::cout << "Все занятые ячейки:\n";
	for (auto const& [pos, value] : matrix) {
		std::cout << "[" << pos.first << "][" << pos.second << "] = " << value << std::endl;
	}
	(((matrix[100][100]) = 314) = 0) = 159;
	std::cout << "Каноническая форма оператора присвоения matrix[100][100] = " << matrix[100][100] << std::endl;


	//N-мерный вариант для 3 мерной матрицы
	NMatrix<int, 0, 3> matrix3d;
	for(int i=0; i<10; i++)
		for(int j=0; j<10; j++)
			for(int k=0; k<10; k++)
			{
				matrix3d[i][j][k] = 10*(i + j) + k;
			}
	std::cout << "Для N-мертной матрицы (N=3) сейчас занято " << matrix3d.size() << " элементов\n";
	(((matrix3d[100][100][100]) = 299) = 0) = 792;
	
	
	//Вывод ячеек и их позиций
	std::cout << "Все занятые ячейки:\n";
	for (auto const& [coords, value] : matrix3d){
		matrix3d.print_coords(coords);
		std::cout << " = " << value << std::endl;
	}

	std::unique_ptr<Matrix<int, 0>> m = std::make_unique<Matrix<int, 0>>(); 
	(*m)[0][0] = 12;  
	auto proxy = (*m)[0];  
	//m.reset(); // ещё один вариант m.release();
	m.release(); // ещё один вариант m.release();
	std::cout << proxy[0] << std::endl;
	std::cout << m << std::endl;
	//std::cout << (*m)[0][0] << std::endl;
	return 0;
}
