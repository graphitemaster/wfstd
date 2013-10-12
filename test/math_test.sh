#!/bin/sh
CXX=g++
CXXPPC=powerpc-g++
CXXARM=arm-none-linux-gnueabi-g++
PASS_SSE=0
PASS_ALTIVEC=0
PASS_SCALAR=0
PASS_NEON=0
PASS_AUTOVEC=0

echo -n "Building SSE     test for x86_64 -> "
$CXX -I../ -msse2 -DEMUTYPE="\"`uname -a`\"" math_test.cpp -o math_sse
echo "Built: \`math_sse\`     -> using $CXX -I../ -msse test.cpp -o math_sse"

echo -n "Building SCALAR  test for i386   -> "
$CXX -I../ -m32 -DEMUTYPE="\"`uname -a` (running 32bit)\"" -mno-sse -mno-sse2 -mno-sse3 math_test.cpp -o math_scalar
echo "Built: \`math_scalar\`  -> using $CXX -I../ -m32 -mno-sse -mno-sse2 -mno-sse3 test.cpp -o math_scalar"

echo -n "Building AltiVec test for PPC    -> "
$CXXPPC -I../ -DEMUTYPE="\"qemu-ppc -cpu G4 (`qemu --version`)\"" -maltivec -static math_test.cpp -o math_altivec
echo "Built: \`math_altivec\` -> using $CXXPPC -I../ -maltivec -static test.cpp -o math_altivec"

echo -n "Building NEON    test for ARM    -> "
$CXXARM -I../ -DEMUTYPE="\"qemu-arm (`qemu --version`)\"" -mfpu=neon -mfloat-abi=softfp -funsafe-math-optimizations -static math_test.cpp -o math_neon
echo "Built: \`math_neon\`    -> $CXXARM -I../ -mfpu=neon -mfloat-abi=softfp -funsafe-math-optimizations -static test.cpp -o math_neon"

echo -n "Building AUTOVEC test for x86_64 -> "
$CXX -I../ -DEMUTYPE="\"`uname -a`\"" -DWF_STDLIB_MATH_AUTOVECTORIZE math_test.cpp -o math_autovec
echo "Built: \`math_autovec\` -> using $CXX -I../ test.cpp -o math_autovec"

echo "[SSE]     Tests Running ..."
./math_sse > math_sse.log
PASS_SSE=$?

echo "[SCALAR]  Tests Running ..."
./math_scalar > math_scalar.log
PASS_SCALAR=$?

echo "[ALTIVEC] Tests Running ..."
qemu-ppc -cpu G4 ./math_altivec > math_altivec.log
PASS_ALTIVEC=$?

echo "[NEON]    Tests Running ..."
qemu-arm ./math_neon > math_neon.log
PASS_NEON=$?

echo "[AUTOVEC] Tests Running ..."
./math_autovec > math_autovec.log
PASS_AUTOVEC=$?

rm math_sse
rm math_altivec
rm math_scalar
rm math_neon
rm math_autovec

if [ $PASS_SSE     -ne 0 ]; then echo "[SSE]     Tests Failed: check math_sse.log     for details"; else echo "[SSE]     Tests Passed"; rm math_sse.log; fi
if [ $PASS_SCALAR  -ne 0 ]; then echo "[SCALAR]  Tests Failed: check math_scalar.log  for details"; else echo "[SCALAR]  Tests Passed"; rm math_scalar.log; fi
if [ $PASS_ALTIVEC -ne 0 ]; then echo "[ALTIVEC] Tests Failed: check math_altivec.log for details"; else echo "[ALTIVEC] Tests Passed"; rm math_altivec.log; fi
if [ $PASS_NEON    -ne 0 ]; then echo "[NEON]    Tests Failed: check math_neon.log    for details"; else echo "[NEON]    Tests Passed"; rm math_neon.log; fi
if [ $PASS_AUTOVEC -ne 0 ]; then echo "[AUTOVEC] Tests Failed: check math_autovec.log for details"; else echo "[AUTOVEC] Tests Passed"; rm math_autovec.log; fi

echo "Finished!"
