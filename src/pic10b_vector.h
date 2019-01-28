#include <iostream>   // std::ostream, std::cout

namespace Pic10b{
	template<typename T>
    class vector{
      private:
        T* the_data;
        size_t the_size;
        size_t the_capacity;

        static const int INIT_CAP = 10;


      public:
        // The big 4 
        vector();
        vector( const vector& );
        vector& operator=( const vector& );
        ~vector();

        // Other members [public]
        bool empty() const;
        size_t size() const;
        size_t capacity() const;
        T front() const;
        T back() const; 
        T at( size_t index ) const;
        T& operator[]( size_t index );
        T operator[]( size_t index ) const; 
        void dump_data_to( std::ostream& out ) const;
        void dump_data() const;
        void push_back( T new_value );
        void pop_back();

      private:
        //Other members [private]
        void reserve( size_t new_capacity );

    }; // end Pic10b::vector


    /** ************************* THE BIG 4 ************************* **/
	template<typename T>
    vector<T>::vector() 
      : the_data(nullptr), the_size(0), the_capacity(INIT_CAP) {

        the_data = new T[the_capacity];
    }

	template<typename T>
	vector<T>::vector( const vector& source )
      : the_data(nullptr), the_size(source.the_size), 
        the_capacity(source.the_capacity) {

        the_data = new double[the_capacity];

        // Deep copy of internal array
        for ( int i = 0 ; i < the_size ; ++i ){
            the_data[i] = source.the_data[i];
        }
    }

	template<typename T>
	vector<T>& vector<T>::operator=( const vector& rhs ) {
        if ( this != &rhs ) {     // Self-assignment?
            // Release old memory and request more 
            delete[] the_data;
            the_data = new double[rhs.the_capacity];

            // Shallow copy non-pointers
            the_size = rhs.the_size;
            the_capacity = rhs.the_capacity;

            // Deep copy internal array
            for ( int i = 0 ; i < the_size ; ++i )
                the_data[i] = rhs.the_data[i];
        }
        return *this;
    }

	template<typename T>
	vector<T>::~vector(){
        delete[] the_data;
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
	T vector<T>::at( size_t index ) const {
        if ( index < the_size )
            return the_data[index];
        return the_data[0];
    }

	template<typename T>
	T& vector<T>::operator[]( size_t index ){
        return the_data[index];
    }
 
	template<typename T>
	T vector<T>::operator[]( size_t index ) const {
        return the_data[index];
    }

	template<typename T>
	void vector<T>::dump_data_to( std::ostream& out ) const {
        out << "Vector (dump): ";
        for ( size_t i = 0 ; i < the_capacity ; ++i )
            out << the_data[i] << ' ';
        out << '\n';
    }
 
	template<typename T>
	void vector<T>::dump_data() const {
        dump_data_to( std::cout );
    }


	template<typename T>
	void vector<T>::push_back( T new_value ){
        if ( the_size == the_capacity ) 
            reserve( the_capacity + 1 );     // `the_data` is reassigned

        the_data[the_size++] = new_value;
    }

    // This implementation does not shrink the vector (ever)
	template<typename T>
	void vector<T>::pop_back(){
        if ( the_size > 0 )
            --the_size;
    }


	template<typename T>
	void vector<T>::reserve( size_t new_capacity ){
        if ( new_capacity > the_capacity ) {
            if ( new_capacity <= 2 * the_capacity )
                new_capacity = 2 * the_capacity;

            double* old_location = the_data;

            the_data = new double[new_capacity];
            the_capacity = new_capacity;

            for ( size_t i = 0 ; i < the_size ; ++i )
                the_data[i] = old_location[i];

            delete old_location;
        }
    }

} // end Pic10b namespace



/** ************************ OTHER FUNCTIONS ************************ **/
template<typename T>
std::ostream& operator<<( std::ostream& out, const Pic10b::vector<T>& v ){
    for ( size_t i = 0 ; i < v.size() ; ++i )
        out << v[i] << ' ';
    return out;
}

template<typename T>
void print_vector( const Pic10b::vector<T>& v ){
    if ( v.empty() )
        std::cout << "Vector is empty\n";
    else
        std::cout << "Vector (contents): " << v << '\n' ;
} 



/** ************************* THE DRIVER ************************ **/
int main(){

    using Pic10b::vector;
    using std::cout;

    cout << "Create & display empty vector (v1)\n";
    vector<int> v1;
    print_vector(v1);
    v1.dump_data();

    int size = 15;
    cout << "\nPopulate & display vector with " << size << " entries (v1)\n";
    for ( int i = 1 ; i <= size ; ++i )
        v1.push_back(i);
    print_vector(v1);

    cout << "\nCopy non-empty vector, pop back last entry & display (v2)\n";
    vector<int> v2(v1);
    v2.pop_back();
    print_vector(v2);

    cout << "\nReassign vector (v1 = v2) & display\n";
    v1 = v2;
    print_vector(v1);

    cout << "\nDump contents of vectors (v1,v2)\n";
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

**/
