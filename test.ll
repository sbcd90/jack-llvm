; ModuleID = 'Module'
source_filename = "Module"
target triple = "arm64-apple-darwin23.2.0"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @add(i32 %0, i32 %1) {
entry:
  %add = add i32 %1, %0
  ret i32 %add
}

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @0, i32 5)
  %1 = call i32 @add(i32 1, i32 7)
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @1, i32 %1)
  ret i32 0
}