# Dark Magic of C++
A brain-damaged PhD student decided to create m2pi bin edges in
compile-time. Here is the result

## Requirements
A C++17 compiler with non-deprecated features of C89 such as `#include`
without guards (who knows if that will be deprecated).

## Example of usage
```
$ ls
ctb.hpp  pack_inds.cpp	README.md
$ c++ pack_inds.cpp 
$ ./a.out 
     0.279      0.32      0.36       0.4      0.44      0.48      0.52      0.56
       0.6      0.62      0.64      0.66      0.68       0.7      0.72      0.74
      0.76      0.77     0.772     0.774     0.776     0.778      0.78     0.782
     0.784     0.786     0.788      0.79     0.792       0.8      0.82      0.84
      0.86      0.88       0.9      0.92      0.96         1      1.04      1.08
      1.12      1.16       1.2      1.24      1.28      1.32      1.36       1.4
      1.44      1.48      1.52      1.56       1.6      1.64      1.68      1.72
      1.76       1.8
Size of table: 58
     0.279      0.32      0.36       0.4      0.44      0.48      0.52      0.56
       0.6      0.64      0.68      0.72      0.76       0.8      0.84      0.88
      0.92      0.93      0.94      0.95      0.96      0.97      0.98      0.99
         1      1.01      1.02      1.03      1.04      1.05      1.06      1.07
      1.08      1.12      1.16       1.2      1.24      1.28      1.32      1.36
       1.4      1.44      1.48      1.52      1.56       1.6      1.64      1.68
      1.72      1.76       1.8
Size of table: 51
```
