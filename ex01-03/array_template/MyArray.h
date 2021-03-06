//
// Created by huxx on 8/29/16.
//

#include <glob.h>

#ifndef ARRAY_TEMPLATE_MYARRAY_H
#define ARRAY_TEMPLATE_MYARRAY_H
namespace felhak {

    template<typename T, size_t s>
    class MyArray {
    public:
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

        T *begin() const{
            return array;
        }

        T *end() const{
            return array+_size;
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

    //Partial specialisation starts here
    template <typename T, size_t s>
    class MyArray<T*, s>{
    public:
        MyArray(){
            array = new T*[s];
            for(T** i = begin(); i != end(); i++)
            {
                *i = nullptr;
            }
        }

        template<typename U>
        MyArray(const MyArray<U, s>& orig){
            array = new T*[s];
            T** i = begin();
            U* j = orig.begin();
            while(i != end()){
                *i = *j;
                i++;
                j++;
            }
        }

        template <typename U>
        MyArray<T*, s>& operator=(const MyArray<U, s>& orig){
            T** i = begin();
            U* j = orig.begin();
            while(i != end()){
                *i = *j;
                i++;
                j++;
            }
            return *this;
        }

        ~MyArray() {
            for(T** i = begin(); i != end(); i++){
                delete *i;
            }
            delete [] array;
        }

        T **begin() const{
            return array;
        }

        T **end() const{
            return array+_size;
        }

        T*& operator[](int i) {
            return *(array+i);
        }

        size_t size() const {
            return _size;
        }

    private:
        size_t _size = s;
        T** array;
    };

}

#endif //ARRAY_TEMPLATE_H
