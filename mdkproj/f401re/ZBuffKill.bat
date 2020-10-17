del *.hex /s
copy Objects\*.hex .\*.hex
rd /Q /S DebugConfig 
rd /Q /S Objects
rd /Q /S Listings
del *.uvguix.* /s
del *.uvgui.* /s
del *.scvd /s
del *.orig /s
exit
