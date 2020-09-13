SET PATH=C:\Keil_v5\ARM\ARMCLANG\Bin;C:\windows;C:\windows\system32;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Python38\Scripts\;C:\Program Files\Python38\;C:\Program Files (x86)\AMD APP\bin\x86_64;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files\Microsoft VS Code\bin;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Program Files\TortoiseSVN\bin;C:\MyProgramFiles\codeblocks\MinGW\bin;C:\Program Files\Git\cmd;C:\Program Files (x86)\Atmel\sam-ba_2.18\drv\;C:\Program Files (x86)\Atmel\sam-ba_2.18
SET ARMCC6_ASMOPT=--diag_suppress=9931
SET ARMCC6_CCOPT=--diag_suppress=9931
SET ARMCC6_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32H750VBTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=h750vb_flash
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmAsm" --Via ".\obj\startup_stm32h750xx._ia"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\system_stm32h7xx.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\adc.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\gpio.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\lcd.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\led.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\common_cmd.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\isr.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\retarget.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\uart.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\blinky.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\obj\h750vb_flash.lnp"
