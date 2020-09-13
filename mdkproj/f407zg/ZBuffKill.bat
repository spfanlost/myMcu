del *.hex /s
copy obj\*.hex .\*.hex
rd /Q /S DebugConfig 
rd /Q /S obj
rd /Q /S list
del *.uvguix.* /s
del *.uvgui.* /s
del *.scvd /s
del *.orig /s
exit
