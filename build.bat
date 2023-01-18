@set parameters=test.aiko /debug


@cl Aiko.cpp /DEBUG -Iinclude /Zi /O2 /oaikof.exe /link LLVM-C.lib
@del *.obj
@echo ------------------------------------------------------------
@echo RUN aikof %parameters%

@aikof %parameters%