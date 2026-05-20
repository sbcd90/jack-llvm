jack-llvm
=========

A simple project to integrate Coursera's Jack programming language from **Nand to Tetris** course with LLVM backend.

## Getting Started

- Sample Jack program

```
class Main {

   method void add(int a, int b) {
        var int c;
        let c = a + b;
        do Output.printInt(c);
        return;
   }
   function void main() {
        var Main main;
        let main = Main.newInstance();

        do main.add(1, 4);
        return;
   }
}
```

- Generate LLVM IR
```
mkdir dev
cd dev
cmake ..
make
cd src
./jack1 ../../Main.jack
```

- Paste LLVM IR in `test.ll` file and compile and run and test
```
clang test.ll -L/opt/homebrew/lib -lgc
./a.out
```
