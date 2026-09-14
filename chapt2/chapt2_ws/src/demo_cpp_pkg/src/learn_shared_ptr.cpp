#include <iostream>
#include <memory>

int main(){
    auto ptr1 = std::make_shared<std::string>("This is a shared pointer example.");
    std::cout << "ptr1的引用计数为:" << ptr1.use_count() << ",指向的内存地址为:" << ptr1.get() << std::endl;
    auto ptr2 = ptr1;
    std::cout << "ptr1的引用计数为:" << ptr1.use_count() << ",指向的内存地址为:" << ptr1.get() << std::endl;
    std::cout << "ptr2的引用计数为:" << ptr2.use_count() << ",指向的内存地址为:" << ptr2.get() << std::endl;
    ptr1.reset();
    std::cout << "ptr1的引用计数为:" << ptr1.use_count() << ",指向的内存地址为:" << ptr1.get() << std::endl;
    std::cout << "ptr2的引用计数为:" << ptr2.use_count() << ",指向的内存地址为:" << ptr2.get() << std::endl;
    return 0;
}