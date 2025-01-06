# cpp primer 差缺补漏

## 第2章 变量和基本类型

### 2.1 变量类型

### 2.2 赋值和初始化

变量
int x = 42;

这是初始化，而不是赋值
> 初始化不是赋值，初始化的含义是创建变量时赋予其一个初始值，而赋值的含义是把对象的当前值擦除，而以一个新值来替代。

列表初始化：

``` cpp

int x{42};


long double ld = 3.1415926;
int a{ld},b = {ld}
// 错误:转换未执行，因 为存在丢失信息的危险
int c(ld), d = 1d; //转换执行，且确实丢失了部分值
```

#### 2.2.2 变量声明和定义的关系

**声明和定义**

> 声明(declaration)使得名字 为程序所知，一 个文件如果想使用别处定义的名字则必须包含对那个名字的声明。而定义 (definition )负责创建与名字关联的实体。

变量只能被定义一次，**但是可以被多次声明**

``` cpp
extern int a; // 使用 extern 去声明一个变量
extern int a = 42;
// main1.cpp:5:16: error: declaration of block scope identifier with linkage cannot have an initializer
//     extern int a =42;
```

#### 2.2.3 标识符

C++的标识符 (identifier )由字母、数字和下画 线组成，其中必须以字母或下画线开 头。标识符的长度没有限制，但是对大小写字母敏感。

### 2.3  复合类型

复合类型就是 “指针” “引用”
这个引用一般讲的就是左值引用,区分于右值引用

```cpp
int a = 42;
// 引用 val 只是 a 的另一个名字
int &val = a;
// const 引用无法用来修他所指向的那个值
const int& const_val_ref = val;

// 引用无法再次的绑定，只能一步到位,所以引用必须有初始化
int &val1; // 报错 引用必须被初始化
```

```cpp
#include <iostream>
using namespace std;
int main()
{
    int a = 42;
    int &a_ref = a;
    cout << a << endl;     // 42
    cout << a_ref << endl; // 42
    a_ref = 24;
    cout << a << endl;     // 24
    cout << a_ref << endl; // 24


// todo
        // 不能定义引用的引用 ?
    int & a_ref_ref = a_ref;
    cout << a_ref_ref<<endl;
    return 0;
}
```

```cpp
// 指针
// 定义一个指针
int* p = 0; // nullptr null 等价

```

知识点：

- 无论是 & 还是 *，在申明的时候 都是修饰 identifier 的。
所以应该这么写

```cpp
int *p,*r;
int v1 = 123;
int &r = v1,&r2=v2;
```

- 记得初始化所有的指针。

### 2.4 const

```cpp
// a 不可以被修改
const int a = 42;
// 对a 的 reference 只能是 const referrence
int& ref_a = a; // error
const int& ref_a_const  = a; // right
// const int& 只能限制不能通过引用的方式去修改原来的值。但是可以通过其他方式修改
// 想要通过指针的方式去修改 const 也是不行的
int * p = &a; // 错误 const int * 不能赋值给 int*
const int* p = &a;
```

总结： const 修饰的引用和指针就是限制使用引用和指针的人无法以引用或者指针的方式去修改原始值。

顶层 const 和底层 const

底层 const: 不能改变 某个者指针指向的值
顶层 const： 不能改变 某个指针本身的值

```cpp
int val =42;
// 不能改变 p1 的值 const 修饰的是 int*  （顶层）
int *const p1 = &val; 

// 不能修改 v2 const 修饰的是 int (顶层)
const int v2 = 32;
// 不能通过p2 修改 V2. const int * 是一个整体 (底层)
const int* p2 = &v2;
// 
const int& ref = v2; // 引用的const 都是底层
```

#### constexpr

一般来说，如果你认定变量是一个常量表达式，那就把它声明成const expr 类型。

### 2.5 类型别名

1. using type1 = TypeName;
2. typedef double wages;

## 第3章 字符串 向量 数组

## 第4章 表达式

### 左右值
>
> 当一个对象被用作右值的时候，用的是对象的值（内容）​；当对象被用作左值的时候，用的是对象的身份（在内存中的位置）​。

## 第6章 函数

### 函数 scope

```cpp

int f(){
    string s;
    /** ... */
    return s; // 错误，s 会在离开函数之后销毁
}

// 形参也是一种局部对象，离开函数之后会被销毁

// 使用 static （局部静态对象）可以脱离函数的作用域
int f2(){
    static size_t call_cnt = 0;
    call_cnt++;
    return call_cnt;
}

```

> **函数原型： 【返回类型】函数名 【函数形参列表】**

### 数组传参

```cpp
void print(const int* arr);
void print(const int[]);
void print(const int[10]); 
// const int*  const 修饰 int 代表指针指向的数据不能被修改
// 上面三种等价
// 可以使用引用的方式去传参
void print(int (&arr)[]);
```

### initializer_list

```cpp
void printList(std::initializer_list<int> list) {
    // 使用范围 for 循环遍历列表
    for (const auto& element : list) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

printList({1, 2, 3, 4, 5});




```

### 可变传参

在 C++ 中，可变参数（Variadic Arguments）允许函数接受任意数量的参数。这是通过使用 `std::initializer_list`、`std::tuple`、可变参数模板（Variadic Templates）或 C 风格的 `va_list` 来实现的。以下是对这些方法的详细解释：

#### 1. 使用 `std::initializer_list`

- **特点**：
  - 适用于传递相同类型的可变数量参数。
  - 需要包含 `<initializer_list>` 头文件。
  - 参数在函数内部表现为一个列表，可以使用范围 `for` 循环或迭代器遍历。
- **代码示例**：

    ```cpp
    #include <iostream>
    #include <initializer_list>

    // 接收一个 std::initializer_list<int> 作为参数
    void printList(std::initializer_list<int> list) {
        for (const auto& element : list) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    int main() {
        printList({1, 2, 3, 4, 5});
        printList({10, 20, 30});
        return 0;
    }
    ```

- **解释**：
  - `printList` 函数接收一个 `std::initializer_list<int>` 作为参数。在 `main` 函数中，使用 `{}` 来传递一组元素，这些元素会自动转换为 `std::initializer_list<int>`。

#### 2. 使用 `std::tuple` 和 `std::make_tuple`

- **特点**：
  - 适用于传递不同类型的固定数量参数。
  - 需要包含 `<tuple>` 头文件。
  - 可以使用 `std::get<N>(tuple)` 来访问元组中的元素，其中 `N` 是元素的索引。
- **代码示例**：

    ```cpp
    #include <iostream>
    #include <tuple>

    // 接收一个 std::tuple 作为参数
    void printTuple(const std::tuple<int, std::string, double>& tup) {
        std::cout << std::get<0>(tup) << " " << std::get<1>(tup) << " " << std::get<2>(tup) << std::endl;
    }

    int main() {
        auto t1 = std::make_tuple(1, "hello", 3.14);
        printTuple(t1);
        return 0;
    }
    ```

- **解释**：
  - `printTuple` 函数接收一个 `std::tuple<int, std::string, double>` 作为参数。
  - `std::make_tuple` 用于创建一个元组，包含不同类型的元素。
  - `std::get<N>(tup)` 用于从元组中提取元素，`N` 是元素的索引。

#### 3. 可变参数模板（Variadic Templates）

- **特点**：
  - 适用于传递不同类型和数量的参数。
  - 利用模板参数包（Template Parameter Pack）和函数参数包（Function Parameter Pack）来实现。
  - 通常使用递归展开参数包。
- **代码示例**：

    ```cpp
    #include <iostream>

    // 递归终止函数
    void print() {}

    // 可变参数模板函数
    template <typename T, typename... Args>
    void print(T first, Args... args) {
        std::cout << first << " ";
        print(args...);
    }

    int main() {
        print(1, "hello", 3.14, 2.71);
        return 0;
    }
    ```

- **解释**：
  - `print` 函数是一个可变参数模板函数，`T` 表示第一个参数，`Args...` 表示参数包。
  - 函数体中先打印第一个参数，然后递归调用 `print` 函数，将参数包中的元素依次传递给下一个 `print` 调用。
  - 递归终止函数 `print()` 是空函数，当参数包为空时调用。

#### 4. C 风格的 `va_list`

- **特点**：
  - 适用于 C++ 中的 C 兼容部分，通常用于传递不同类型和数量的参数。
  - 需要包含 `<cstdarg>` 头文件。
  - 使用 `va_start`、`va_arg` 和 `va_end` 宏来操作可变参数。
  - 通常需要一个参数来指示参数的数量或一个结束标记。
- **代码示例**：

    ```cpp
    #include <iostream>
    #include <cstdarg>

    // 接收一个整数参数和可变数量的整数参数
    void print(int count,...) {
        va_list args;
        va_start(args, count);
        for (int i = 0; i < count; ++i) {
            std::cout << va_arg(args, int) << " ";
        }
        va_end(args);
        std::cout << std::endl;
    }

    int main() {
        print(3, 1, 2, 3);
        return 0;
    }
    ```

- **解释**：
  - `va_list` 是一个存储可变参数的类型。
  - `va_start(args, count)` 初始化 `args`，`count` 是参数的数量。
  - `va_arg(args, int)` 从 `args` 中获取下一个 `int` 类型的参数。
  - `va_end(args)` 清理参数列表。

### 返回数组指针的函数

```cpp
int (*func(int i))[10];
```

func（int i）表示调用func函数时需要一个int类型的实参。·

（＊func（int i）​）意味着我们可以对函数调用的结果执行解引用操作。

（＊func（int i）​）​[10]表示解引用func的调用将得到一个大小是10的数组。

 int （＊func（int i）​）​[10]表示数组中的元素是int类型。

c++11的简化写法：

```cpp
// 尾置返回优化
auto func(int i)->int(*)[10];
// decltype
int arr[5] = {01,2,3,4};
decltype(arr)* func(int x);
```

### 函数重载

函数重载必须在形参上有所区别，单单是返回值不一样会有二义性错误

底层 const 也可以看作形参的差异

```cpp
int func(Account&);
int func(const Account&);
// 这个重载没有二义性
```

**在C++语言中，名字查找发生在类型检查之前。**

### 函数指针和指针函数

函数指针：
函数指针的声明只需要将函数原型中的函数名 改成 (*指针名)

如：
int add(int,int); ===> int (*pAdd)(int,int);
这个括号很重要 区别了 返回 int 指针，还是返回 一个 函数

int (*p)(int, int) = add;
add 前面的&是可选的，是等价的

```cpp
#include <iostream>
using namespace std;

int add(int a, int b) {
    return a + b;
}

int main() {
    // 定义函数指针 p，指向 add 函数
    int (*p)(int, int) = add;
    // 通过函数指针调用 add 函数
    int result = p(2, 3);
    cout << result << endl;
    return 0;
}

```

指针函数：

```cpp

#include <iostream>
using namespace std;

int* createArray(int size) {
    int* arr = new int[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }
    return arr;
}

int main() {
    int* ptr = createArray(5);
    for (int i = 0; i < 5; ++i) {
        cout << ptr[i] << " ";
    }
    delete[] ptr;  // 记得释放动态分配的内存
    return 0;
}
```

函数指针的一个实际用处：c 语言实现多态

```c
#include <iostream>
using namespace std;

// 定义 Animal 结构体并使用 typedef 定义别名
typedef struct AnimalStruct {
    void (*eat)(struct AnimalStruct*);
} Animal;

// 定义 Dog 结构体并使用 typedef 定义别名
typedef struct DogStruct {
    Animal super;
    int age;
} Dog;

// 定义 dogEat 函数
void dogEat(Animal* animal) {
    Dog* dog = (Dog*)animal;
    cout << "dog eat " << dog->age << endl;
}

int main() {
    // 多态：用 base 的指针去调用 derive 的方法
    Dog dog;
    dog.super.eat = dogEat;
    dog.age = 3;
    dog.super.eat((Animal*)&dog);
    return 0;
}

```

### 其他

形参默认值

```cpp
double getArea(int w = 100,int h=100);
// 可以直接这么调用
getArea(); // 等价于getArea(100,100);
```

## 第七章 类

[c++ 面向对象设计 csdn 总结](https://blog.csdn.net/weixin_44368437/article/details/117563488)

## 第 13章 拷贝控制

1. 拷贝构造函数
2. 拷贝赋值运算符
3. 移动构造函数
4. 析构函数
5. 移动赋值运算符

```cpp
//1. 拷贝构造函数
class Foo{
    Foo();
    Foo(const Foo&); //拷贝构造函数
// 2 拷贝赋值运算符
    Foo& operater=(const Foo&);
}



```

构造函数和相应的运算符重载是相对应的，要一起定义

**default** **delete**

```cpp
//1. 拷贝构造函数
class Foo{
    Foo();
    Foo(const Foo&) = default; // 使用默认的合成拷贝构造函数
    // Foo(const Foo&) = delete; //禁止拷贝构造
    // 移动构造函数
    Foo(Foo&&)noexcept;
    //

// 2 拷贝赋值运算符
    Foo& operator=(const Foo& rhs);

    Foo& operator=(const Foo&& rhs);

    

    // 
}



```

析构函数无法被 delete

管理类外资源的类必须定义拷贝控制成员

一旦一个类需要析构函数，那么它几乎肯定也需要一个拷贝构造函数和一个拷贝赋值运算符。

- 如果将一个对象赋予它自身，赋值运算符必须能正确工作。
- 大多数赋值运算符组合了析构函数和拷贝构造函数的工作。
- 在移动操作之后，移后源对象必须保持有效的、可析构的状态，但是用户不能对其值进行任何假设。
- 只有当一个类没有定义任何自己版本的拷贝控制成员，且它的所有数据成员都能移动构造或移动赋值时，编译器才会为它合成移动构造函数或移动赋值运算符。

### 右值引用

 //todo
>
```cpp
int  i = 42; 
int &r = 42; // 错误 42 是一个 rv 不可以绑定到 lr
int &r = i; // 正确
int &&r = i*42; // 正确
const int& r= i*42;// 正确 const 引用可以绑定到rv

int&& rr = std::move(i); // std::move 获取一个左值的右值引用
//我们可以销毁一个移后源对象，也可以赋予它新值，但不能使用一个移后源对象的值

```
