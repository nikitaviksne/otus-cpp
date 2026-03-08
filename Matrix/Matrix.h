#include <map>
#include <vector>

//Header for Template class infinity Matrix

template <typename T, T DefaultValue=0 >
class Matrix
{
	std::map<std::pair<int,int>, T> _data;
public:
	class ConstElementProxy {
		const Matrix& matrix;
		int x,y;
	public:
		ConstElementProxy(const Matrix& m, int r, int c) : matrix(m), x(r), y(c) {};
		operator T() const {
			auto it = matrix._data.find({x,y});
			return (it != matrix._data.end())? it->second: DefaultValue;
		}
	};
	class ConstRowProxy {
		const Matrix& matrix;
		int x;
	public:
		ConstRowProxy(const Matrix& m, int r) : matrix(m), x(r) {};
		ConstElementProxy operator[](int y) const {
			return ConstElementProxy(matrix, x,y);
		}
	};
	class ElementProxy {
		Matrix& matrix;
		int x,y;
	public:
		
		ElementProxy(Matrix& m, int r, int c): matrix(m), x(r), y(c) {};
		//Чтение
		operator T() const {
			return static_cast<T>(ConstElementProxy(matrix, x,y));
		}
		
		//Запись
		ElementProxy& operator=(const T& value) {
			if (value == DefaultValue) {
				matrix._data.erase({x,y});
			} 
			else{
				matrix._data[{x,y}] = value;
			}
			return *this;
		}
		
		ElementProxy& operator=(const ElementProxy& other) {
			return *this= static_cast<T>(other);
		}
	};
	
	class RowProxy {
		Matrix& matrix;
		int x;
	public:
		RowProxy(Matrix& m, int r): matrix(m), x(r) {};
		ElementProxy operator[](int y){
			return ElementProxy(matrix, x, y);
		}
	};
	
	RowProxy operator[](int x) {
		return RowProxy(*this, x);
	}

	ConstRowProxy operator[](int x) const {
		return ConstRowProxy(*this, x);
	}
	std::size_t size() const //метод получения кол-ва реально занятых ячеек
	{
		return _data.size();
	}
	
	//итераторы для прохода по занятым ячейкам
	auto begin() {return _data.begin();};
	auto end() {return _data.end();};
	
	
};
