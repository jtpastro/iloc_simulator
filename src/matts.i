L2: loadI 3 => r1
mult r1,r1 => r3
add r1,r1 => r3
loadI 1024 => r4
store r3 => r4
br => L1
L1: loadI 3 => r3
store r3 => r4
output 1024
