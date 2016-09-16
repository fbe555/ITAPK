#include <glob.h>

#ifndef ARRAY_TEMPLATE_MYARRAY_H
#define ARRAY_TEMPLATE_MYARRAY_H
namespace felhak {

    template<typename T, size_t s>
    class MyArray{};

    template<typename T>
    class MyArray_iterator : public std::iterator<std::random_access_iterator_tag, T>{
    private:
        T* _ptr;
    public:
        MyArray_iterator();
        MyArray_iterator(T* ptr);
        template <typename U, size_t s>
        MyArray_iterator(const MyArray<U, s>& myArray);
        template <typename U>
        MyArray_iterator(const MyArray_iterator<U>& it);
        MyArray_iterator& operator++();
        MyArray_iterator& operator--();
        MyArray_iterator& operator++(int);
        MyArray_iterator& operator--(int);
        MyArray_iterator& operator+(int i);
        MyArray_iterator& operator-(int i);

        bool operator==(const MyArray_iterator& rhs);
        bool operator!=(const MyArray_iterator& rhs);
        bool operator<(const MyArray_iterator& rhs);
        bool operator<=(const MyArray_iterator& rhs);
        bool operator>(const MyArray_iterator& rhs);
        bool operator>=(const MyArray_iterator& rhs);

        T& operator*();
        T& operator->();
        T& operator[](int i);
    };

    template<typename T, size_t s>
    class MyArray {
    private:
        T* ptr;

        typedef T value_type;

        MyArray(){
            array = new T[s];
        }

        template<typename U>
        MyArray(const MyArray<U, s>& orig){
            array = new T[s];
            T* i = begin();
            U* j = orig.begin();
            while(i != end()){
                *i = *j;
                i++;
                j++;
            }
        }

        template <typename U>
        MyArray<T, s>& operator=(const MyArray<U, s>& orig){
            T* i = begin();
            U* j = orig.begin();
            while(i != end()){
                *i = *j;
                i++;
                j++;
            }
            return *this;
        }

        ~MyArray() {
            delete [] array;
        }

        void fill(const T & value) {
            for(T* i = begin(); i != end(); i++){
                *i = value;
            }
        }

        MyArray_iterator<T> *begin() const{
            return MyArray_iterator<T>(array);
        }

        MyArray_iterator<const T> *cbegin() const{
            return MyArray_iterator<const T>(array);
        }

        MyArray_iterator<T> *end() const{
            return MyArray_iterator<T>(array+_size);
        }

        MyArray_iterator<const T> *cend() const{
            return MyArray_iterator<T>(array+_size);
        }

        T& operator[](int const i) {
            return *(array+i);
        }

        size_t size() const {
            return _size;
        }

    private:
        size_t _size = s;
        T* array;
    };
}

#endif //ARRAY_TEMPLATE_H