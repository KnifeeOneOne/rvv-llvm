// REQUIRES: x86-registered-target
// RUN: %clang -c -DINLINE=inline -target x86_64-unknown-linux-gnu -o - \
// RUN: -emit-interface-stubs -std=gnu89 -xc %s | \
// RUN: FileCheck -check-prefix=CHECK-GNU %s
// RUN: %clang -DINLINE=inline -target x86_64-linux-gnu -O0 -o - -c \
// RUN: -std=gnu89 -xc %s | llvm-nm - | FileCheck -check-prefix=CHECK-GNU %s

// RUN: %clang -c -DINLINE="__attribute__((always_inline))" \
// RUN: -target x86_64-unknown-linux-gnu -o - -emit-interface-stubs -xc %s | \
// RUN: FileCheck -check-prefix=CHECK-GNU %s
// RUN: %clang -DINLINE="__attribute__((always_inline))" \
// RUN: -target x86_64-linux-gnu -O0 -o - -c -xc %s | \
// RUN: llvm-nm - | FileCheck -check-prefix=CHECK-GNU %s

// RUN: %clang -c -DINLINE=inline -target x86_64-unknown-linux-gnu -o - \
// RUN: -emit-interface-stubs -std=c99 -xc %s | \
// RUN: FileCheck -check-prefix=CHECK-STD %s
// RUN: %clang -DINLINE=inline -target x86_64-linux-gnu -O0 -o - -c -std=c99 \
// RUN: -xc %s | llvm-nm - 2>&1 | FileCheck -check-prefix=CHECK-STD %s

// RUN: %clang -c -DINLINE="__attribute__((noinline))" \
// RUN: -target x86_64-unknown-linux-gnu -o - -emit-interface-stubs -std=c99 -xc %s | \
// RUN: FileCheck -check-prefix=CHECK-NOINLINE %s
// RUN: %clang -DINLINE="__attribute__((noinline))" -target x86_64-linux-gnu \
// RUN: -O0 -o - -c -std=c99 -xc %s | llvm-nm - 2>&1 | \
// RUN: FileCheck -check-prefix=CHECK-NOINLINE %s

// RUN: %clang -c -DINLINE="static" -target x86_64-unknown-linux-gnu -o - \
// RUN: -emit-interface-stubs -std=c99 -xc %s | \
// RUN: FileCheck -check-prefix=CHECK-STATIC %s
// RUN: %clang -DINLINE="static" -target x86_64-linux-gnu -O0 -o - -c \
// RUN: -std=c99 -xc %s | llvm-nm - 2>&1 | \
// RUN: FileCheck -check-prefix=CHECK-STATIC %s

// CHECK-GNU-DAG: foo
// CHECK-GNU-DAG: foo.var
// CHECK-NOINLINE-DAG: foo
// CHECK-NOINLINE-DAG: foo.var
// CHECK-STATIC-NOT: foo
// CHECK-STATIC-NOT: foo.var
// CHECK-STD-NOT: foo
#pragma clang diagnostic ignored "-Wstatic-local-in-inline"
INLINE int foo() {
  static int var = 42;
  return var;
}

// RUN: %clang -c -DINLINE=inline -target x86_64-linux-gnu -o - \
// RUN: -emit-interface-stubs \
// RUN: -std=gnu89 -xc %s | FileCheck -check-prefix=CHECK-TAPI %s

// RUN: %clang -c -DINLINE=inline -target x86_64-linux-gnu -o - \
// RUN: -emit-interface-stubs \
// RUN: -std=gnu89 -xc %s | FileCheck -check-prefix=CHECK-SYMBOLS %s
// RUN: %clang -DINLINE=inline -target x86_64-linux-gnu -o - \
// RUN: -c -std=gnu89 -xc %s | llvm-nm - 2>&1 | \
// RUN: FileCheck -check-prefix=CHECK-SYMBOLS %s

// CHECK-TAPI-DAG: foo: { Type: Func }
// CHECK-TAPI-DAG: foo.var: { Type: Object, Size: 4 }
// CHECK-SYMBOLS-DAG: foo
// CHECK-SYMBOLS-DAG: foo.var
#include "inline.h"
