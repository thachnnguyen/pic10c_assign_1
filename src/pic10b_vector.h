#include <iostream>   // std::ostream, std::cout
#include <type_traits>
#include <typeinfo>
#include <cmath>

/**
 * In order to use the certain operators, the type must implement these operations:
 * - +=, !=, *, +, <, <=, >, >=,
 *
 */
namespace Pic10b {
	template<typename T>
	class vector {
	private:
		T* the_data;
		size_t the_size;
		size_t the_capacity;

		static const int INIT_CAP = 10;


	public:
		// The big 4 
		vector();
		vector(const vector&);
		vector& operator=(const vector&);
		vector& operator+=(const vector&);
		~vector();

		// Other members [public]
		bool empty() const;
		size_t size() const;
		size_t capacity() const;
		T front() const;
		T back() const;
		T at(size_t index) const;
		T& operator[](size_t index);
		T operator[](size_t index) const;
		void dump_data_to(std::ostream& out) const;
		void dump_data() const;
		void push_back(T new_value);
		void pop_back();

	private:
		//Other members [private]
		void reserve(size_t new_capacity);

	}; // end Pic10b::vector


	/*************************** THE BIG 4 ***************************/
	template<typename T>
	vector<T>::vector() : the_data(nullptr), the_size(0), the_capacity(INIT_CAP) {

		the_data = new T[the_capacity];

		std::cout << "xxxxxxxxxx Default constructor called\n";
	}
	template<typename T>
	vector<T>::vector(const vector& source)
		: the_data(nullptr), the_size(source.the_size),
		the_capacity(source.the_capacity) {

		the_data = new T[the_capacity];

		// Deep copy of internal array
		for (size_t i = 0; i < the_size; ++i) {
			the_data[i] = source.the_data[i];
		}

		std::cout << "xxxxxxxxxx Copy constructor called\n";
	}
	template<typename T>
	vector<T>& vector<T>::operator=(const vector& rhs) {
		if (this != &rhs) {     // Self-assignment?
			// Release old memory and request more 
			delete[] the_data;
			the_data = new T[rhs.the_capacity];

			// Shallow copy non-pointers
			the_size = rhs.the_size;
			the_capacity = rhs.the_capacity;

			// Deep copy internal array
			for (size_t i = 0; i < the_size; ++i)
				the_data[i] = rhs.the_data[i];
		}

		std::cout << "xxxxxxxxxx Assignment operator called\n";
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator+=(const Pic10b::vector<T>& v1) {
		for (size_t i = 0; i < the_size; ++i)
			the_data[i] = v1.the_data[i] + the_data[i];
		return *this;
	}

	template<typename T>
	vector<T>::~vector() {
		delete[] the_data;

		std::cout << "xxxxxxxxxx Destructor called\n";
	}

	/** *********************** OTHER MEMBERS *********************** **/
	template<typename T>
	bool vector<T>::empty() const {
		return the_size == 0;
	}
	template<typename T>
	size_t vector<T>::size() const {
		return the_size;
	}
	template<typename T>
	size_t vector<T>::capacity() const {
		return the_capacity;
	}
	template<typename T>
	T vector<T>::front() const {
		return *the_data;
	}
	template<typename T>
	T vector<T>::back() const {
		return *(the_data + the_size - 1);
	}
	template<typename T>
	T vector<T>::at(size_t index) const {
		if (index < the_size)
			return the_data[index];
		return the_data[0];
	}
	template<typename T>
	T& vector<T>::operator[](size_t index) {
		return the_data[index];
	}
	template<typename T>
	T vector<T>::operator[](size_t index) const {
		return the_data[index];
	}
	template<typename T>
	void vector<T>::dump_data_to(std::ostream& out) const {
		out << "Vector (dump): ";
		for (size_t i = 0; i < the_capacity; ++i)
			out << the_data[i] << ' ';
		out << '\n';
	}
	template<typename T>
	void vector<T>::dump_data() const {
		dump_data_to(std::cout);
	}

	template<typename T>
	void vector<T>::push_back(T new_value) {
		if (the_size == the_capacity)
			reserve(the_capacity + 1);     // `the_data` is reassigned

		the_data[the_size++] = new_value;
	}

	// This implementation does not shrink the vector (ever)
	template<typename T>
	void vector<T>::pop_back() {
		if (the_size > 0)
			--the_size;
	}

	template<typename T>
	void vector<T>::reserve(size_t new_capacity) {
		if (new_capacity > the_capacity) {
			if (new_capacity <= 2 * the_capacity)
				new_capacity = 2 * the_capacity;

			T* old_location = the_data;

			the_data = new T[new_capacity];
			the_capacity = new_capacity;

			for (size_t i = 0; i < the_size; ++i)
				the_data[i] = old_location[i];

			delete old_location;
		}
	}

} // end Pic10b namespace



/** ************************ OTHER FUNCTIONS ************************ **/
template<typename T>
std::ostream& operator<<(std::ostream& out, const Pic10b::vector<T>& v) {
	//std::cout << "***********  typeid " << typeid(v[0]).name() << std::endl;
	//(typeid(v[0]).name() == "string") ? out << "[" : out << "{";
	(std::is_same<T, std::string>::value) ? out << "[ " : out << "{";
	for (size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i < v.size() - 1) out << ", ";
	}
	//(typeid(v[0]).name() == "string") ? out << "]" : out << "}";
	(std::is_same<T, std::string>::value) ? out << " ]" : out << "}";
	return out;
}

template<typename T>
void print_vector(const Pic10b::vector<T>& v) {
	if (v.empty())
		std::cout << "Vector is empty\n";
	else
		std::cout << "Vector (contents): " << v << '\n';
}

template<typename T>
Pic10b::vector<T>  operator*(const T& c, const Pic10b::vector<T>& v) {
	//static_assert(std::is_arithmetic<T>::value, "Numeric required.");
	Pic10b::vector<T> v1;
	for (size_t i = 0; i < v.size(); ++i)
		v1.push_back(c * v[i]);
	return v1;
}

// specialized for string
template <>
Pic10b::vector<std::string> operator*<std::string>(const std::string& c, const Pic10b::vector<std::string>& v) {
	Pic10b::vector<std::string> v1;
	for (size_t i = 0; i < v.size(); ++i)
		v1.push_back(c + v[i]);
	return v;
}

template<typename T>
Pic10b::vector<T> operator*(const Pic10b::vector<T>& v, const T& c) {
	static_assert(std::is_arithmetic<T>::value, "Numeric required.");
	Pic10b::vector<T> v1;
	for (size_t i = 0; i < v.size(); ++i)
		v1.push_back(v[i] * c);
	return v1;
}

// specialized for string
template <> Pic10b::vector<std::string> operator*<std::string>(const Pic10b::vector<std::string>& v,
	const std::string& c) {
	Pic10b::vector<std::string> v1;
	for (size_t i = 0; i < v.size(); ++i)
		v1.push_back(v[i] + c);
	return v;
}

template<typename T>
T operator*(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	T total = 0;
	for (size_t i = 0; i < vec1.size(); ++i)
		total += vec1[i] * vec2[i];
	return total;
}

// specialize for string
template<>
std::string operator*<std::string>(const Pic10b::vector<std::string>& vec1, 
	const Pic10b::vector<std::string>& vec2) {
	std::string v;
	size_t sm_size;
	const Pic10b::vector<std::string> *big_vec;

	if (vec1.size() < vec2.size()) {
		sm_size = vec1.size();
		big_vec = &vec2;
	}
	else {
		sm_size = vec2.size();
		big_vec = &vec1;
	}
	size_t i = 0;
	for (; i < sm_size; ++i)
		v += (vec1[i] + vec2[i]);
	for (; i < big_vec->size(); ++i)
		v += (*big_vec)[i];

	return v;
}

template<typename T>
Pic10b::vector<T> operator+(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	Pic10b::vector<T> v;
	size_t sm_size;
	const Pic10b::vector<T> *big_vec;

	if (vec1.size() < vec2.size()) {
		sm_size = vec1.size();
		big_vec = &vec2;
	}
	else {
		sm_size = vec2.size();
		big_vec = &vec1;
	}
	size_t i = 0;
	for (; i < sm_size; ++i)
		v.push_back(vec1[i] + vec2[i]);
	for (; i < big_vec->size(); ++i)
		v.push_back((*(big_vec))[i]);

	return v;
}

template<typename T>
Pic10b::vector<T> operator+(const Pic10b::vector<T>& v, const T& c) {
	Pic10b::vector<T> v1;
	for (size_t i = 0; i < v.size(); ++i)
		v1.push_back(v[i] + c);
	return v1;
}


template<typename T>
bool operator==(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	if (vec1.size() != vec2.size()) return false;
	for (size_t i = 0; i < vec1.size(); ++i)
		if (vec1[i] != vec2[i]) return false;
	return true;
}

template<typename T>
bool operator!=(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	return !(vec1 == vec2);
}

template<typename T>
bool operator<(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	return sqrt(vec1*vec1) < sqrt(vec2*vec2);
}

template<typename T>
bool operator<=(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	return !(vec1 > vec2);
}

template<typename T>
bool operator>(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	return sqrt(vec1*vec1) > sqrt(vec2*vec2);
}

template<typename T>
bool operator>=(const Pic10b::vector<T>& vec1, const Pic10b::vector<T>& vec2) {
	return !(vec1 < vec2);
}


/*************************** THE DRIVER **************************/
int main1() {

	using Pic10b::vector;
	using std::cout;

	std::cout << "Create & display empty vector (v1)\n";
	vector<int> v1;
	print_vector(v1);
	v1.dump_data();

	int size = 15;
	std::cout << "\nPopulate & display vector with " << size << " entries (v1)\n";
	for (int i = 1; i <= size; ++i)
		v1.push_back(i);
	print_vector(v1);

	std::cout << "\nCopy non-empty vector, pop back last entry & display (v2)\n";
	vector<int> v2(v1);
	v2.pop_back();
	print_vector(v2);

	std::cout << "\nReassign vector (v1 = v2) & display\n";
	v1 = v2;
	print_vector(v1);

	std::cout << "\nDump contents of vectors (v1,v2)\n";
	v1.dump_data();
	v2.dump_data();

	return 0;
}

/**
	OUTPUT:

		Create & display empty vector (v1)
		Vector is empty
		Vector (dump): 0 0 0 0 0 0 0 0 0 0

		Populate & display vector with 15 entries (v1)
		Vector (contents): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15

		Copy non-empty vector, pop back last entry & display (v2)
		Vector (contents): 1 2 3 4 5 6 7 8 9 10 11 12 13 14

		Reassign vector (v1 = v2) & display
		Vector (contents): 1 2 3 4 5 6 7 8 9 10 11 12 13 14

		Dump contents of vectors (v1,v2)
		Vector (dump): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0 0 0 0 0
		Vector (dump): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0 0 0 0 0

*/
