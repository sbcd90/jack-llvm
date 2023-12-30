; ModuleID = 'Module'
source_filename = "Module"
target triple = "arm64-apple-darwin23.2.0"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %z = alloca i32, align 4
  %y = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 1, ptr %x, align 4
  store i32 4, ptr %y, align 4
  store i32 5, ptr %z, align 4
  %0 = load ptr, ptr %z, align 8
  %1 = call i32 (ptr, ...) @printf(ptr @0, ptr %0)
  ret i32 0
}