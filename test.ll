; ModuleID = 'Module'
source_filename = "Module"
target triple = "arm64-apple-darwin23.2.0"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @0, i32 5)
  ret i32 0
}