# shared ptr for c language

A shared ptr and weak ptr implementation for C language.


## Requirements
* gcc or clang compiler (required)
* make (required)

## 简介
  用C模拟了boost的shared ptr与weak ptr，利用编译器的cleanup属性模拟C++ RAII.

  ```bash
  make debug;
  ./a.out;
  # 肉眼看下malloc free的输出是否配对...
  ```

