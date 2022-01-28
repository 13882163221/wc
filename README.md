<style type="text/css">
    h1 { counter-reset: h2counter; }
    h2 { counter-reset: h3counter; }
    h3 { counter-reset: h4counter; }
    h4 { counter-reset: h5counter; }
    h5 { counter-reset: h6counter; }
    h6 { }
    h2:before {
      counter-increment: h2counter;
      content: counter(h2counter) ".\0000a0\0000a0";
    }
    h3:before {
      counter-increment: h3counter;
      content: counter(h2counter) "."
                counter(h3counter) ".\0000a0\0000a0";
    }
    h4:before {
      counter-increment: h4counter;
      content: counter(h2counter) "."
                counter(h3counter) "."
                counter(h4counter) ".\0000a0\0000a0";
    }
    h5:before {
      counter-increment: h5counter;
      content: counter(h2counter) "."
                counter(h3counter) "."
                counter(h4counter) "."
                counter(h5counter) ".\0000a0\0000a0";
    }
    h6:before {
      counter-increment: h6counter;
      content: counter(h2counter) "."
                counter(h3counter) "."
                counter(h4counter) "."
                counter(h5counter) "."
                counter(h6counter) ".\0000a0\0000a0";
    }
</style>

# C++ : 模板编程

## 非类型模板参数(Nontype Template Parameter)

### 非类型类模板参数(Nontype Class Template Parameter)

使用者自己定义,但是每个模板实例都是不同的类型;也可以为模板参数提供默认值；
Example:

```c++
template<int MAXSIZE=100>
class array{
    private:
        int num[MAXSIZE];
    public:
        void print(){
            std::cout << sizeof(num) / sizeof(int) << std::endl;
        }
};
array<60> int60arrray;
array<> defaultArray;
int main(int argc, const char *argv[])
{
    int60arrray.print(); /* 60 */
    defaultArray.print(); /* 100 */
    return 0;
}
```

### 非类型函数模板参数(Nontype Function Template Parameter)

Example:

```c++
template<int count>
int sumUp(int x) {
    return x + count;
}
int main(int argc, const char *argv[])
{
    std::cout << sumUp<10>(60) <<std::endl; /* 70 */
    return 0;
}
```

### Restrictions

```c++
/* double, float或者类类型*/
template<double count> /* error : A non-type template parameter cannot have type 'double' before C++20 */

/* 字符串常量*/
template<char const* name>
class test{

};
test<"hello"> t; /* error : "hello" is not a valid template argument for type 'const char*' because string literals can never be used in this context*/

/* 全局指针 */
template<char const* name>
class test{

};

char const* s = "hello";
test<s> t;/* error : 's' is not a valid template argument because 's' is a variable, not the address of a variable*/

/* 全局数组 这样是可以的 */
template<char const* name>
class test{

};

char const s[] = "hello";
test<s> t;
```

## 类型模板参数(Type Template Parameter)
