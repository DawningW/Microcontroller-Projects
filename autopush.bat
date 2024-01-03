@echo off
echo Start pushing up sub projects...
git subtree push --prefix=lightcube lightcube master
git subtree push --prefix=minichatserver minichatserver master
git subtree push --prefix=DebugTool/qt qt master
git subtree push --prefix=DebugTool/android android master
echo Finished!
pause
