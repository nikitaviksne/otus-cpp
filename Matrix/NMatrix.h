#include <map>
#include <vector>

template<typename T, T DefaultValue, std::size_t N=2>
class NMatrix{
	std::map<std::vector<int>, T> _data;
public:
	
	//Базовый Proxy
	template<std::size_t Dim>
	class Proxy{
		NMatrix& matrix;
		std::vector<int> coords;
	public:
		Proxy(NMatrix& m, std::vector<int> c): matrix(m), coords(std::move(c)) {}
		//Рекурсия
		auto operator[] (int index){
			std::vector<int> next_coords = coords;
			next_coords.push_back(index);
			if constexpr (Dim > 1)
			{
				return Proxy<Dim -1>(matrix, std::move(next_coords));
			}
			else
			{
				return ElementProxy(matrix, std::move(next_coords));
			}
		}
	};
	//финальный прокси для ччейки
	class ElementProxy{
		NMatrix& matrix;
		std::vector<int> coords;
	public:
		ElementProxy(NMatrix& m, std::vector<int> c): matrix(m), coords(std::move(c)) {};
		operator T() const {
			auto it = matrix._data.find(coords);
			return (it != matrix._data.end())? it->second : DefaultValue;
		}
		ElementProxy& operator=(const T& value){
			if (value == DefaultValue){
				matrix._data.erase(coords);
			}
			else
			{
				matrix._data[coords] = value;
			}
			return *this;
		}

		ElementProxy& operator= (const ElementProxy& other)
		{
			return *this = static_cast<T> (other);
		}
	};
	auto operator[] (int index){
		return Proxy<N-1>(*this, {index});
	}
	std::size_t size() const //метод получения кол-ва реально занятых ячеек
	{
		return _data.size();
	}
	//итераторы для прохода по занятым ячейкам
	auto begin() {return _data.begin();};
	auto end() {return _data.end();};
	
	void print_coords(const std::vector<int>& coords) {
		for(int c: coords){
			std::cout << "[" << c << "]";
		}
	}
};



